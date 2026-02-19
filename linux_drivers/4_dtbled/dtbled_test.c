#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

// 设备节点路径(和驱动中CHRDEVBASE_NAME一致)
#define LED_DEV "/dev/led"

/*
 * @description : 主函数,测试字符驱动的读写功能
 * @param - argc : 参数个数
 * @param - argv : 参数列表：
 *                1. ./led_test write "测试数据" ：向驱动写入数据
 * @return : 0 成功;其他 失败
 */
int main(int argc, char *argv[])
{
    int fd, retvalue;
    char writebuf[100];
    
    // ========== 第一步：参数校验 ==========
    if(argc < 3){
        printf("Usage:\n");
        printf("  %s write <0/1> - 关闭/打开LED灯\n", argv[0]);
        return -1;
    }

    // ========== 第二步：打开设备节点 ==========
    fd = open("/dev/led", O_RDWR); // 以仅写模式打开
    if(fd < 0){
        // 带错误码打印,方便调试
        printf("打开设备失败!errno=%d, errmsg=%s\n", errno, strerror(errno));
        return -1;
    }
    printf("成功打开设备：%s(fd=%d)\n", LED_DEV, fd);

    // ========== 第三步：根据参数执行写操作 ==========
    // 向驱动写入数据
    if(strcmp(argv[1], "write") == 0){
        if(argc < 3){
            printf("写入失败!请指定要写入的数据,例如：%s write \"0/1\"\n", argv[0]);
            close(fd);
            return -1;
        }

        // 向驱动写入数据
        retvalue = write(fd, argv[2], strlen(argv[2]));
        if(retvalue < 0){
            printf("写入设备失败!errno=%d, errmsg=%s\n", errno, strerror(errno));
            close(fd);
            return -1;
        }
        printf("写入成功!写入字节数：%d,数据：%s\n", retvalue, argv[2]);
    }

    // 无效参数
    else{
        printf("无效参数!仅支持 write 操作\n");
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