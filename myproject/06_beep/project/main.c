/**************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名 : main.c
作者 : 左忠凯
版本 : V1.0
描述 : I.MX6U 开发板裸机实验 2 C 语言点灯
使用 C 语言来点亮开发板上的 LED 灯，学习和掌握如何用 C 语言来
 完成对 I.MX6U 处理器的 GPIO 初始化和控制。
其他 : 无
日志 : 初版 V1.0 2019/1/3 左忠凯创建
**************************************************************/

#include "imx6ul.h"
#include "clk.h"
#include "led.h"
#include "delay.h"
#include "beep.h"

/*
 * @description : main 函数
 * @param : 无
 * @return : 无
 */
int main(void)
{
    clk_enable(); /* 使能所有的时钟 */
    led_init();   /* 初始化 led */
    beep_init();  /* 初始化 beep */

    while (1) /* 死循环 */
    {
        led_off();  /* 关闭 LED */
        beep_off(); /* 关闭蜂鸣器 */
        delay(500); /* 延时大约 500ms */

        led_on();   /* 打开 LED */
        beep_on();  /* 打开蜂鸣器 */
        delay(500); /* 延时大约 500ms */
    }

    return 0;
}