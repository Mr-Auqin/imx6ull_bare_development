// 核心必加
#include <linux/module.h>   // module_init/module_exit/MODULE_LICENSE
#include <linux/fs.h>       // file_operations/open/read/write/release
#include <linux/cdev.h>     // cdev_init/cdev_add/cdev_del
#include <linux/device.h>   // class_create/device_create/class_destroy
#include <linux/slab.h>     // kmalloc/kfree（内核内存分配）
#include <linux/uaccess.h>  // copy_to_user/copy_from_user（用户/内核数据拷贝）
#include <linux/errno.h>    // 内核标准错误码（-EINVAL/-ENOMEM等）
#include <linux/init.h>     // __init修饰的函数（如chrdevbase_init）编译到内核的.init.text段



//申明cdev的 open/release/read/write 函数
static int chrdevbase_open(struct inode *inode, struct file *filp);
static int chrdevbase_release(struct inode *inode, struct file *filp);
static ssize_t chrdevbase_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt);
static ssize_t chrdevbase_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt);



//定义核心变量
#define CHRDEVBASE_NAME  "chrdevbase" // 设备名
#define CHRDEVBASE_MAJOR  2048        /* 主设备号（手动指定，避免动态分配） */

static char readbuf[100];      // 驱动层读缓冲区
static char writebuf[100];     // 驱动层写缓冲区
static char kerneldata[100] = "kernel data!"; // 驱动默认数据


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
    retvalue = copy_to_user(buf, readbuf, cnt);//返回值为未成功拷贝的字节数（成功时返回0）
    if(retvalue == 0){
        printk("chrdevbase read success!\n");
    }else{
        printk("chrdevbase read failed!\n");
    }
    
    return cnt-retvalue;//返回读取成功的字节数
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

    //writebuf缓冲区clear
    memset(writebuf, 0, sizeof(writebuf));
    // 接收用户层传递过来的数据（核心API：copy_from_user）
    retvalue = copy_from_user(writebuf, buf, cnt);//返回值为未成功拷贝的字节数（成功时返回0）
    //写缓冲区的数据传递给内核数据
    memcpy(kerneldata, writebuf, sizeof(writebuf));
    if(retvalue == 0){
        printk("chrdevbase write success! data: %s\n", writebuf);
    }else{
        printk("chrdevbase write failed!\n");
    }
    
    return cnt-retvalue;//返回实际写入的字节数
}


// 驱动加载入口函数：无goto，纯devm_自动释放
static int __init chrdevbase_init(void)
{
    int ret = 0;

    /********************* 第一步：初始化私有数据 *********************/
    memset(readbuf, 0, sizeof(readbuf));
    memset(writebuf, 0, sizeof(writebuf));

    /********************* 第二步：注册字符设备 *********************/
    /* 1. 注册字符设备（手动指定主设备号，避免动态分配） */
    ret = register_chrdev(CHRDEVBASE_MAJOR, CHRDEVBASE_NAME, &chrdevbase_fops);
    if (ret < 0) {
        printk(KERN_ERR "chrdevbase: 注册字符设备失败！\n");
        return ret;
    }
    printk(KERN_INFO "chrdevbase: 驱动加载成功！主设备号：%d\n", CHRDEVBASE_MAJOR);
    return 0;

    /********************* 第三步：硬件初始化（devm_版示例，虚拟驱动可省） *********************/
    // 若有GPIO：用devm_gpio_request替代gpio_request，自动释放
    // ret = devm_gpio_request(&chrdevbase_cdev.dev, LED_GPIO, "led");
    // if (ret < 0) { printk("GPIO申请失败"); return ret; }

    return 0; // 成功返回，所有devm资源由内核托管
}

// 驱动出口函数：无需手动释放资源，devm自动处理
static void __exit chrdevbase_exit(void)
{
    // 只需打印日志，所有devm资源（设备号/cdev/类/节点）由内核自动释放
    unregister_chrdev(CHRDEVBASE_MAJOR, CHRDEVBASE_NAME);
    printk(KERN_INFO "chrdevbase: 驱动卸载成功！\n");
}


//模块入口/出口宏（内核规范）
module_init(chrdevbase_init); // 核心宏：注册驱动入口函数，insmod时内核自动执行该函数
module_exit(chrdevbase_exit); // 核心宏：注册驱动出口函数，rmmod时内核自动执行该函数

//模块信息（必加GPL，否则加载失败）
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Auqin");
MODULE_DESCRIPTION("Chrdevbase Virtual Character Driver");
MODULE_VERSION("V1.0");




































