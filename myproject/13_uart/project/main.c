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
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_exit.h"
#include "epit.h"
#include "uart.h"
#include "bsp_KeyFliter.h"



/*
 * @description : main 函数
 * @param : 无
 * @return : 无
 */
int main(void)
{

    int_init();      /* 初始化中断 */
    imx6u_clkinit(); /*初始化CLK Tree*/
    clk_enable();    /* 使能所有的时钟 */
    delay_Init();    /* 初始化延时函数 */
    led_init();      /* 初始化 led */
    beep_init();     /* 初始化 beep */
    KeyFiler_init(); /* 初始化按键带有10ms消抖无阻塞，外部中断+定时器消抖实现 */
    UART_Init();     /* 初始化串口 */


    while (1) /* 死循环 */
    {

        // 指示系统启动完成
        led_on();   /* 打开 LED */
        puts("Debug ...\r\n");
        delay_ms(200); /* 延时大约 200ms */
        led_off();  /* 关闭 LED */
        puts("Debug complete\r\n");
        delay_ms(200); /* 延时大约 200ms */

    }

    return 0;
}