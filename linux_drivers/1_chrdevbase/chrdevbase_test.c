#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

// 设备节点路径(和驱动中CHRDEVBASE_NAME一致)
#define CHRDEVBASE_DEV "/dev/chrdevbase"

/*
 * @description : 主函数,测试字符驱动的读写功能
 * @param - argc : 参数个数
 * @param - argv : 参数列表,支持两种用法：
 *                1. ./chrdevbase_test read ：读取驱动数据
 *                2. ./chrdevbase_test write "测试数据" ：向驱动写入数据
 * @return : 0 成功;其他 失败
 */
int main(int argc, char *argv[])
{
    int fd, retvalue;
    char readbuf[100], writebuf[100];
    
    // ========== 第一步：参数校验 ==========
    if(argc < 2){
        printf("Usage:\n");
        printf("  %s read        - 读取驱动数据\n", argv[0]);
        printf("  %s write <data> - 向驱动写入数据\n", argv[0]);
        return -1;
    }

    // ========== 第二步：打开设备节点 ==========
    fd = open(CHRDEVBASE_DEV, O_RDWR); // 以读写模式打开
    if(fd < 0){
        // 带错误码打印,方便调试
        printf("打开设备失败!errno=%d, errmsg=%s\n", errno, strerror(errno));
        return -1;
    }
    printf("成功打开设备：%s(fd=%d)\n", CHRDEVBASE_DEV, fd);

    // ========== 第三步：根据参数执行读写操作 ==========
    // 读取驱动数据
    if(strcmp(argv[1], "read") == 0){
        // 初始化缓冲区,避免脏数据
        memset(readbuf, 0, sizeof(readbuf));
        // 从驱动读取数据(最多读取100字节)
        retvalue = read(fd, readbuf, sizeof(readbuf));
        if(retvalue < 0){
            printf("读取设备失败!errno=%d, errmsg=%s\n", errno, strerror(errno));
            close(fd);
            return -1;
        }
        printf("读取成功!读取字节数：%d,数据：%s\n", retvalue, readbuf);
    }

    // 向驱动写入数据
    else if(strcmp(argv[1], "write") == 0){
        if(argc < 3){
            printf("写入失败!请指定要写入的数据,例如：%s write \"hello driver\"\n", argv[0]);
            close(fd);
            return -1;
        }
        // 拷贝要写入的数据到缓冲区
        strncpy(writebuf, argv[2], sizeof(writebuf)-1); // 留1字节存'\0'
        // 向驱动写入数据
        retvalue = write(fd, writebuf, strlen(writebuf));
        if(retvalue < 0){
            printf("写入设备失败!errno=%d, errmsg=%s\n", errno, strerror(errno));
            close(fd);
            return -1;
        }
        printf("写入成功!写入字节数：%d,数据：%s\n", retvalue, writebuf);
    }

    // 无效参数
    else{
        printf("无效参数!仅支持 read/write 操作\n");
        close(fd);
        return -1;
    }

    // ========== 第四步：关闭设备 ==========
    retvalue = close(fd);
    if(retvalue < 0){
        printf("关闭设备失败!errno=%d, errmsg=%s\n", errno, strerror(errno));
        return -1;
    }
    printf("成功关闭设备\n");

    return 0;
}