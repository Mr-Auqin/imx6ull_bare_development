/*
 * IMX6ULL GPIO字符设备驱动（大厂风格）
 * 功能：控制GPIO1_IO05（LED）的亮灭
 * 核心：纯寄存器操作，字符设备框架，严格错误处理
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/of.h>
#include <linux/of_address.h>



/* ==================== 1. 宏定义（大厂规范：全大写+模块前缀） ==================== */
// 设备相关
#define IMX6ULL_LED_DEV_NAME    "imx6ull_led"

//寄存器物理地址
#define CCM_CCGR1_Base                          (0x020C406CU)
#define IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03_Base   (0x020E0068U)
#define IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03_Base   (0x020E02F4U)
#define GPIO1_GDIR_Base                         (0x0209C004U)
#define GPIO1_DR_Base                           (0x0209C000U)

//PAD属性 相关操作定义
#define IOMUXC_SW_PAD_CTL_PAD_SRE_MASK           (0x1U)
#define IOMUXC_SW_PAD_CTL_PAD_SRE_SHIFT          (0U)
#define IOMUXC_SW_PAD_CTL_PAD_SRE(x)             (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_PAD_CTL_PAD_SRE_SHIFT)) & IOMUXC_SW_PAD_CTL_PAD_SRE_MASK)
#define IOMUXC_SW_PAD_CTL_PAD_DSE_MASK           (0x38U)
#define IOMUXC_SW_PAD_CTL_PAD_DSE_SHIFT          (3U)
#define IOMUXC_SW_PAD_CTL_PAD_DSE(x)             (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_PAD_CTL_PAD_DSE_SHIFT)) & IOMUXC_SW_PAD_CTL_PAD_DSE_MASK)
#define IOMUXC_SW_PAD_CTL_PAD_SPEED_MASK         (0xC0U)
#define IOMUXC_SW_PAD_CTL_PAD_SPEED_SHIFT        (6U)
#define IOMUXC_SW_PAD_CTL_PAD_SPEED(x)           (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_PAD_CTL_PAD_SPEED_SHIFT)) & IOMUXC_SW_PAD_CTL_PAD_SPEED_MASK)
#define IOMUXC_SW_PAD_CTL_PAD_ODE_MASK           (0x800U)
#define IOMUXC_SW_PAD_CTL_PAD_ODE_SHIFT          (11U)
#define IOMUXC_SW_PAD_CTL_PAD_ODE(x)             (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_PAD_CTL_PAD_ODE_SHIFT)) & IOMUXC_SW_PAD_CTL_PAD_ODE_MASK)
#define IOMUXC_SW_PAD_CTL_PAD_PKE_MASK           (0x1000U)
#define IOMUXC_SW_PAD_CTL_PAD_PKE_SHIFT          (12U)
#define IOMUXC_SW_PAD_CTL_PAD_PKE(x)             (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_PAD_CTL_PAD_PKE_SHIFT)) & IOMUXC_SW_PAD_CTL_PAD_PKE_MASK)
#define IOMUXC_SW_PAD_CTL_PAD_PUE_MASK           (0x2000U)
#define IOMUXC_SW_PAD_CTL_PAD_PUE_SHIFT          (13U)
#define IOMUXC_SW_PAD_CTL_PAD_PUE(x)             (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_PAD_CTL_PAD_PUE_SHIFT)) & IOMUXC_SW_PAD_CTL_PAD_PUE_MASK)
#define IOMUXC_SW_PAD_CTL_PAD_PUS_MASK           (0xC000U)
#define IOMUXC_SW_PAD_CTL_PAD_PUS_SHIFT          (14U)
#define IOMUXC_SW_PAD_CTL_PAD_PUS(x)             (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_PAD_CTL_PAD_PUS_SHIFT)) & IOMUXC_SW_PAD_CTL_PAD_PUS_MASK)
#define IOMUXC_SW_PAD_CTL_PAD_HYS_MASK           (0x10000U)
#define IOMUXC_SW_PAD_CTL_PAD_HYS_SHIFT          (16U)
#define IOMUXC_SW_PAD_CTL_PAD_HYS(x)             (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_PAD_CTL_PAD_HYS_SHIFT)) & IOMUXC_SW_PAD_CTL_PAD_HYS_MASK)


static  void __iomem *CCM_CCGR1 = NULL;
static  void __iomem *IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03 = NULL;
static  void __iomem *IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03 = NULL;
static  void __iomem *GPIO1_GDIR = NULL;
static  void __iomem *GPIO1_DR = NULL;




/* ==================== 2. 私有数据结构体（大厂必写：资源集中管理） ==================== */
struct dtbled_dev {
    dev_t                   devno;          // 设备号
    int                     major;          // 主设备号
    int                     minor;          // 次设备号
    struct cdev             cdev;          // 定义cdev
    // struct class            *class;        // 定义class 指针
    // struct device           *device;       // 定义device 指针
    struct device_node      *node;         // 设备树节点指针
};

// 全局唯一实例（单设备场景）
static struct dtbled_dev dtbled;


/* ==================== 3. 寄存器操作封装（原厂风格：inline函数） ==================== */
static inline void write_reg(void __iomem *MapAddr, u32 val)
{
    writel(val, MapAddr);
}

static inline u32 read_reg(void __iomem *MapAddr)
{
    return readl(MapAddr);
}

/* ==================== 4. LED硬件初始化（底层寄存器配置） ==================== */
static void led_gpio_hw_init(void)
{
    uint32_t RegValue;

    // 1、使能GPIO1时钟,简单粗暴 直接把CCM_CCGR1的值写入0xFFFFFFFF 里面就有GPIO的时钟 包使能的
    write_reg(CCM_CCGR1, read_reg(CCM_CCGR1) | 0xFFFFFFFF);

    //2、配置GPIO1_IO03 复用成 GPIO模式
    RegValue = read_reg(IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03);
    RegValue &= ~(0xFU << 0);
    RegValue |= 0x05U << 0;
    write_reg(IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03,RegValue);
    
    /* 3、配置 GPIO1_IO03 的 IO 属性为 GPIO 输出需要的属性 配置为输出时：pull/keeper 功能可以直接关闭
     *bit 16:0 HYS 关闭
     *bit [15:14]: 00 100K 下拉
     *bit [13]: 0 kepper 功能
     *bit [12]: 0 pull/keeper 功能  disable
     *bit [11]: 0 关闭开路输出
     *bit [7:6]: 10 速度 100Mhz
     *bit [5:3]: 110 R0/6 驱动能力
     *bit [0]: 0 低转换率
     */
    RegValue =  IOMUXC_SW_PAD_CTL_PAD_HYS(0)|\
                IOMUXC_SW_PAD_CTL_PAD_PUS(0)|\
                IOMUXC_SW_PAD_CTL_PAD_PUE(0)|\
                IOMUXC_SW_PAD_CTL_PAD_PKE(0)|\
                IOMUXC_SW_PAD_CTL_PAD_ODE(0)|\
                IOMUXC_SW_PAD_CTL_PAD_SPEED(2)|\
                IOMUXC_SW_PAD_CTL_PAD_DSE(6)|\
                IOMUXC_SW_PAD_CTL_PAD_SRE(0);
    write_reg(IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03,RegValue);

    /* 4、初始化 GPIO, GPIO1_IO03 设置为输出 */
    RegValue = read_reg(GPIO1_GDIR);    
    RegValue |= (1 << 3); /* 设置 GPIO1_IO03 为输出 */
    write_reg(GPIO1_GDIR,RegValue);

    /* 5、设置 GPIO1_IO03 输出高电平，关闭 LED0 */
    RegValue = read_reg(GPIO1_DR);    
    RegValue |= (1 << 3); /* 输出高电平，关闭 LED */
    write_reg(GPIO1_DR,RegValue);

}


/*
 * @description : 打开 LED 灯
 * @param : 无
 * @return : 无
 */
static void led_on(void)
{
    uint32_t RegValue;
    /*设置 GPIO1_IO03 输出低电平，打开 LED0 */
    RegValue = read_reg(GPIO1_DR);    
    RegValue &= ~(1 << 3); /* 输出低电平，打开 LED */
    write_reg(GPIO1_DR,RegValue);
}

/*
 * @description : 关闭 LED 灯
 * @param : 无
 * @return : 无
 */
static void led_off(void)
{
    uint32_t RegValue;
    /*设置 GPIO1_IO03 输出高电平，关闭 LED0 */
    RegValue = read_reg(GPIO1_DR);    
    RegValue |= (1 << 3); /* 输出高电平，关闭 LED */
    write_reg(GPIO1_DR,RegValue);
}


/* ==================== 5. 字符设备操作函数（标准接口） ==================== */
static int gpio_open(struct inode *inode, struct file *filp)
{
    // 绑定私有数据到文件句柄（大厂规范）
    filp->private_data = &dtbled;
    return 0;
}

static int gpio_release(struct inode *inode, struct file *filp)
{
    // 绑定私有数据到文件句柄的绑定
    filp->private_data = NULL;
    return 0;
}

static ssize_t gpio_read(struct file *filp, char __user *buf, size_t size, loff_t *off)
{
    return -EINVAL; // 内核标准错误码，标识不支持此操作
}

static ssize_t gpio_write(struct file *filp, const char __user *buf, size_t size, loff_t *off)
{
    uint8_t data = 0;
    ssize_t ret = 0;

    if (1 == size)
    {
        ret = copy_from_user(&data, buf, size);
        if(ret == 0){
            printk(KERN_INFO "led write success! data: %c\n", data);
        }else{
            printk(KERN_INFO "led write failed!\n");
        }
    
        if (data=='1')
        {
            led_on();
            printk(KERN_INFO "led on!\n");
        }
        else if (data=='0')
        {
            led_off();
            printk(KERN_INFO "led off!\n");
        }
        else
        {
            printk(KERN_INFO "Parameter is illegal!\n");
        }

    }
    else
    {
        ret = -EINVAL;
        printk(KERN_INFO "size of write must be 1!\n");
    }

    return size-ret;//返回实际写入的字节数
}

/* ==================== 6. 文件操作集（固定格式） ==================== */
static const struct file_operations led_fops = {
    .owner      = THIS_MODULE,
    .open       = gpio_open,
    .release    = gpio_release,
    .read       = gpio_read,
    .write      = gpio_write,
};

/* ==================== 7. 驱动入口（严格资源申请+错误处理） ==================== */
static int __init imx6ull_led_init(void)
{
    struct dtbled_dev *led = &dtbled;
    struct property *prop = NULL;
    u32 Reg[10] = {0};
    
    int ret,i = 0;

    // 提示驱动初始化开始
    printk(KERN_INFO "imx6ull led driver init start...\n");

    /*获取设备数中的属性数据*/
    /*1、获取设备树节点：/Auqinled*/
    led->node = of_find_node_by_path("/Auqinled");
    if (led->node == NULL)
    {
        printk(KERN_ERR "of_find_node_by_path failed!\n");
        ret = -ENOENT; // 内核标准错误码，标识未找到设备树节点
    } else {
        printk(KERN_INFO "of_find_node_by_path success! node name: %s\n", led->node->name);
    }   

    /*2、获取compatible属性*/
    prop = of_find_property(led->node,"compatible",NULL);
    if (prop == NULL)
    {
        printk(KERN_ERR "of_find_property compatible failed!\n");
        ret = -ENOENT; // 内核标准错误码，标识未找到compatible属性
    } else {
        printk(KERN_INFO "of_find_property compatible success!,compatible=%s\n", (char *)prop->value);
    }
    
    /*3、获取status属性*/
    prop = of_find_property(led->node,"status",NULL);
    if (prop == NULL)
    {
        printk(KERN_ERR "of_find_property status failed!\n");
        ret = -ENOENT; // 内核标准错误码，标识未找到status属性
    } else {
        printk(KERN_INFO "of_find_property status success!,status=%s\n", (char *)prop->value);
    }

    /*4、获取reg属性*/
    ret = of_property_read_u32_array(led->node,"reg",Reg,sizeof(Reg)/sizeof(u32));
    if(ret < 0) {
        printk(KERN_ERR "of_property_read_u32_array reg failed!\n");
        ret = -ENOENT; // 内核标准错误码，标识未找到reg属性
    }else{
        printk(KERN_INFO "of_property_read_u32_array reg success! reg:");
        for(i=0;i<sizeof(Reg)/sizeof(u32);i++){
            printk(KERN_INFO "Reg[%d]=0x%X\n", i, Reg[i]);
        }
    }



    /* 步骤1：申请设备号（动态分配，大厂推荐） */
    // 动态分配设备号
    ret = alloc_chrdev_region(&led->devno, 0, 1, IMX6ULL_LED_DEV_NAME);
    if (ret < 0)
    {
        // 分配失败
        printk(KERN_ERR "alloc chrdev region failed! ret=%d\n", ret);
        goto err_alloc_chrdev_region;
    }
    led->major = MAJOR(led->devno); // 提取主设备号
    led->minor = MINOR(led->devno); // 提取次设备号

    // 提示led devno
    printk(KERN_INFO "led devno: major=%d, minor=%d\n", led->major, led->minor);

    /* 步骤2：初始化LED硬件 */
    /* 映射GPIO物理地址到虚拟地址 */
    CCM_CCGR1 = of_iomap(led->node,0);
    if (CCM_CCGR1 == NULL)
    {
        printk(KERN_ERR "ioremap CCM_CCGR1 failed!\n");
        ret = -ENOMEM;
        goto err_ioremap_ccm;
    }
    IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03 = of_iomap(led->node,1);
    if (IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03 == NULL)
    {
        printk(KERN_ERR "ioremap IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03 failed!\n");
        ret = -ENOMEM;
        goto err_ioremap_mux;
    }
    IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03 = of_iomap(led->node,2);
    if (IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03 == NULL)
    {
        printk(KERN_ERR "ioremap IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03 failed!\n");
        ret = -ENOMEM;
        goto err_ioremap_pad;
    }
    GPIO1_DR = of_iomap(led->node,3);
    if (GPIO1_DR == NULL)
    {
        printk(KERN_ERR "ioremap GPIO1_DR failed!\n");
        ret = -ENOMEM;
        goto err_ioremap_dr;
    }
    GPIO1_GDIR = of_iomap(led->node,4);
    if (GPIO1_GDIR == NULL)
    {
        printk(KERN_ERR "ioremap GPIO1_GDIR failed!\n");
        ret = -ENOMEM;
        goto err_ioremap_gdir;
    }

    led_gpio_hw_init();
    
    // 3. 初始化 cdev，将 file_operations 绑定到 cdev
    cdev_init(&led->cdev, &led_fops);
    led->cdev.owner = THIS_MODULE;

    ret = cdev_add(&led->cdev, led->devno, 1);
    if (ret < 0) {
        printk(KERN_ERR "cdev_add failed! ret=%d\n", ret);
        goto err_cdev_add; 
    }

    // // 4. 创建class
    // led->class = class_create(THIS_MODULE, IMX6ULL_LED_DEV_NAME);
    // if (IS_ERR(led->class)) {
    //     ret = PTR_ERR(led->class);
    //     led->class = NULL;
    //     printk(KERN_ERR "class_create failed! ret=%d\n", ret);
    //     goto err_class_create;
    // }

    // //5 . 创建device
    // led->device = device_create(led->class, NULL, led->devno, NULL, IMX6ULL_LED_DEV_NAME);
    // if (IS_ERR(led->device)) {
    //     ret = PTR_ERR(led->device);
    //     led->device = NULL;
    //     printk(KERN_ERR "device_create failed! ret=%d\n", ret);
    //     goto err_device_create;
    // }

    // 提示驱动初始化成功:所有资源申请完成才算成功
    printk(KERN_INFO "imx6ull led driver init success!\n");

    // 返回成功 0
    return 0;
 
    /* 错误处理：反向释放资源（大厂核心规范） */
// err_device_create:
//     class_destroy(led->class);
// err_class_create:
//     cdev_del(&led->cdev);
err_cdev_add:
err_ioremap_dr:
    iounmap(GPIO1_DR);
err_ioremap_gdir:
    iounmap(GPIO1_GDIR);
err_ioremap_pad:
    iounmap(IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03);
err_ioremap_mux:
    iounmap(IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03);
err_ioremap_ccm:
    iounmap(CCM_CCGR1);
    unregister_chrdev_region(led->devno, 1);
err_alloc_chrdev_region:
    return ret;
}

/* ==================== 8. 驱动出口（对称释放资源） ==================== */
static void __exit imx6ull_led_exit(void)
{
    struct dtbled_dev *led = &dtbled;

    printk(KERN_INFO "imx6ull gpio driver exit start...\n");

    /* 错误处理：反向释放资源（大厂核心规范） */
    // device_destroy(led->class, led->devno);
    // class_destroy(led->class);
    cdev_del(&led->cdev);
    iounmap(GPIO1_DR);
    iounmap(GPIO1_GDIR);
    iounmap(IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03);
    iounmap(IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03);
    iounmap(CCM_CCGR1);
    unregister_chrdev_region(led->devno, 1);

    printk(KERN_INFO "imx6ull led driver exit success!\n");

}

/* ==================== 9. 模块入口/出口注册 ==================== */
module_init(imx6ull_led_init);
module_exit(imx6ull_led_exit);

/* ==================== 10. 模块信息（大厂规范） ==================== */
//模块信息（必加GPL，否则加载失败）
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Auqin");
MODULE_DESCRIPTION("IMX6ULL LED Driver");
MODULE_VERSION("V1.0");
