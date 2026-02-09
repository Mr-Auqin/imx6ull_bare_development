// 核心必加
#include <linux/module.h>   // module_init/module_exit/MODULE_LICENSE
#include <linux/fs.h>       // file_operations/open/read/write/release
#include <linux/cdev.h>     // cdev_init/cdev_add/cdev_del
#include <linux/device.h>   // class_create/device_create/class_destroy
#include <linux/slab.h>     // kmalloc/kfree（内核内存分配）
#include <linux/uaccess.h>  // copy_to_user/copy_from_user（用户/内核数据拷贝）
#include <linux/errno.h>    // 内核标准错误码（-EINVAL/-ENOMEM等）
#include <linux/init.h>     // __init修饰的函数（如chrdevbase_init）编译到内核的.init.text段





// 1. 定义核心变量
#define CHRDEVBASE_MAJOR 200    // 主设备号（静态分配，新手易理解）
#define CHRDEVBASE_NAME  "chrdevbase" // 设备名

static char readbuf[100];      // 驱动层读缓冲区
static char writebuf[100];     // 驱动层写缓冲区
static char kerneldata[] = "kernel data!"; // 驱动默认数据

static dev_t devid;            // 设备号（主+次）
static struct cdev chrdevbase_cdev;
static struct class *chrdevbase_class;
static struct device *chrdevbase_device;


//定义文件操作集（核心：绑定字符驱动函数）
static struct file_operations chrdevbase_fops = {
    .owner = THIS_MODULE,         // 必须赋值，指向当前模块
    .open = chrdevbase_open,      //打开字符驱动设备
    .release = chrdevbase_release,//释放字符驱动设备
    .read = chrdevbase_read,      //读取字符驱动设备
    .write = chrdevbase_write,    //写入字符驱动设备
};


/*
 * @description : 打开设备
 * @param – inode : 传递给驱动的inode
 * @param – filp : 设备文件，file结构体有个叫做private_data的成员变量
 *                一般在open的时候将private_data指向设备结构体。
 * @return : 0 成功;其他 失败
 */
static int chrdevbase_open(struct inode *inode, struct file *filp)
{
    // printk是内核打印，类似printf，KERN_INFO是日志级别
    printk("chrdevbase open!\n");
    return 0;
}

/*
 * @description : 关闭/释放设备
 * @param – filp : 要关闭的设备文件(文件描述符)
 * @return : 0 成功;其他 失败
 */
static int chrdevbase_release(struct inode *inode, struct file *filp)
{
    // printk是内核打印，类似printf，KERN_INFO是日志级别
    //release释放设备时，释放的资源按照 打开的顺序 反向释放 目的是为了防止资源依赖问题（核心规范） 
    //具体讲 打开的时候 先申请了A资源 然后在A资源内申请了B资源 B资源是依赖于A资源的 如果释放的时候 先释放了A资源 那么B资源就会丢失 导致无法释放
    printk("chrdevbase release!\n");
    return 0;
}


/*
 * @description : 从设备读取数据 
 * @param – filp : 要打开的设备文件(文件描述符)
 * @param – buf : 返回给用户空间的数据缓冲区
 * @param – cnt : 要读取的数据长度
 * @param – offt : 相对于文件首地址的偏移
 * @return : 读取的字节数，如果为负值，表示读取失败
 */
static ssize_t chrdevbase_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
    int retvalue = 0;
    
    // 把内核数据拷贝到用户层（核心API：copy_to_user）
    //将kerneldata 先拷贝到 内核缓冲区 readbuf 里 再从readbuf拷贝到用户层buf里 
    //这样做的目的是为了保护内核数据的安全性 避免用户层直接访问内核数据导致系统崩溃
    memcpy(readbuf, kerneldata, sizeof(kerneldata));
    retvalue = copy_to_user(buf, readbuf, cnt);
    if(retvalue == 0){
        printk("chrdevbase read success!\n");
    }else{
        printk("chrdevbase read failed!\n");
    }
    
    return retvalue;
}

/*
 * @description : 向设备写数据 
 * @param – filp : 设备文件，表示打开的文件描述符
 * @param – buf : 要写给设备写入的数据
 * @param – cnt : 要写入的数据长度
 * @param – offt : 相对于文件首地址的偏移
 * @return : 写入的字节数，如果为负值，表示写入失败
 */
static ssize_t chrdevbase_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
{
    int retvalue = 0;
    // 接收用户层传递过来的数据（核心API：copy_from_user）
    retvalue = copy_from_user(writebuf, buf, cnt);
    if(retvalue == 0){
        printk("chrdevbase write success! data: %s\n", writebuf);
    }else{
        printk("chrdevbase write failed!\n");
    }
    
    return retvalue;
}


// 驱动加载入口函数：insmod时执行，返回0=成功，负数=失败
static int __init chrdevbase_init(void)
{
    int ret = 0; // 错误码接收变量，内核函数失败返回负的标准错误码
    /********************* 第一步：初始化私有数据 *********************/
    memset(readbuf, 0, sizeof(readbuf));  // 缓冲区清零
    memset(writebuf, 0, sizeof(writebuf));
    /********************* 第二步：分配设备号 *********************/
    // 动态分配（推荐）：alloc_chrdev_region(&设备号, 次设备号起始, 设备数, 设备名)
    ret = alloc_chrdev_region(&devid, 0, 1, CHRDEVBASE_NAME);
    if (ret < 0) { // 分配失败，直接goto错误处理
        printk("设备号分配失败: %d\n", ret);
        goto ERR_DEVID; // 跳转到对应错误标签，释放已申请资源（这里还没资源，直接返回）
    }
    /********************* 第三步：初始化+注册cdev *********************/
    cdev_init(&chrdevbase_cdev, &chrdevbase_fops); // 绑定cdev和文件操作集
    chrdevbase_cdev.owner = THIS_MODULE;          // 必设：指向当前模块，内核用于资源管理
    // 注册cdev：向内核注册字符设备核心对象
    ret = cdev_add(&chrdevbase_cdev, devid, 1);
    if (ret < 0) {
        printk("cdev注册失败: %d\n", ret);
        goto ERR_CDEV; // 失败：先释放已分配的设备号，再返回
    }
    /********************* 第四步：创建设备类+设备节点 *********************/
    // 创建设备类：/sys/class/下生成类目录，用于udev自动创建设备节点
    chrdevbase_class = class_create(THIS_MODULE, CHRDEVBASE_NAME);
    if (IS_ERR(chrdevbase_class)) { // 注意：class_create失败返回错误指针，用IS_ERR判断
        ret = PTR_ERR(chrdevbase_class); // 提取错误码
        printk("设备类创建失败: %d\n", ret);
        goto ERR_CLASS;
    }
    // 创建设备节点：/dev/下生成设备文件（无需手动mknod）
    chrdevbase_device = device_create(chrdevbase_class, NULL, devid, NULL, CHRDEVBASE_NAME);
    if (IS_ERR(chrdevbase_device)) {
        ret = PTR_ERR(chrdevbase_device);
        printk("设备节点创建失败: %d\n", ret);
        goto ERR_DEVICE;
    }
    /********************* 第五步：硬件初始化（有硬件时） *********************/
    // 虚拟驱动无硬件，若为LED/按键驱动，这里写GPIO申请+配置：
    // gpio_request(LED_GPIO, "led");
    // gpio_direction_output(LED_GPIO, 1);
    
    printk("chrdevbase驱动加载成功!\n");
    return 0; // 成功，返回0

/********************* 错误处理：反向释放资源（按申请顺序倒着来） *********************/
ERR_DEVICE:
    class_destroy(chrdevbase_class); // 释放已创建的设备类
ERR_CLASS:
    cdev_del(&chrdevbase_cdev);      // 释放已注册的cdev
ERR_CDEV:
    unregister_chrdev_region(devid, 1); // 释放已分配的设备号
ERR_DEVID:
    return ret; // 返回错误码，内核会提示加载失败
}



// 5. 驱动出口函数（卸载驱动时执行，注销资源）
static void __exit chrdevbase_exit(void)
{
    // 反向释放：先创建设备的后释放
    device_destroy(chrdevbase_class, devid);
    class_destroy(chrdevbase_class);
    cdev_del(&chrdevbase_cdev);
    unregister_chrdev_region(devid, 1);
    
    printk("chrdevbase exit success!\n");
}




// 6. 模块入口/出口宏（内核规范）
module_init(chrdevbase_init); // 核心宏：告诉内核这是驱动入口函数，内核会将其加入初始化表
module_exit(chrdevbase_exit);

// 7. 模块信息（必加GPL，否则加载失败）
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Auqin");
MODULE_DESCRIPTION("Chrdevbase Virtual Character Driver");
MODULE_VERSION("V1.0");




































