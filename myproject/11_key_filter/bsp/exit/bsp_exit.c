/***************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 bsp_exit.c
作者	   : 左忠凯
版本	   : V1.0
描述	   : 外部中断驱动。
其他	   : 配置按键对应的GPIP为中断模式
论坛 	   : www.wtmembed.com
日志	   : 初版V1.0 2019/1/4 左忠凯创建
***************************************************************/
#include "bsp_exit.h"
#include "bsp_gpio.h"
#include "bsp_int.h"
#include "delay.h"
#include "beep.h"
#include "epit.h"

/*
 * @description			: 初始化外部中断
 * @param				: 无
 * @return 				: 无
 */
void exit_init(void)
{
	gpio_pin_config_t key_config;

	/* 1、设置IO复用 */
	IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18,0);			/* 复用为GPIO1_IO18 */

    /* 2、配置 GPIO1_IO03 的 IO 属性为 GPIO 外部中断输入的相关属性
     *bit 16:0 HYS 关闭
     *bit [15:14]: 00 100K 下拉
     *bit [13]: 0 kepper 功能
     *bit [12]: 0 pull/keeper 功能  disable
     *bit [11]: 0 关闭开路输出
     *bit [7:6]: 10 速度 100Mhz
     *bit [5:3]: 110 R0/6 驱动能力
     *bit [0]: 0 低转换率
     */
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, IOMUXC_SW_PAD_CTL_PAD_HYS(0)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_PUS(0)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_PUE(0)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_PKE(0)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_ODE(0)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_SPEED(2)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_DSE(6)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_SRE(0));


	/* 2、初始化GPIO为中断模式 */
	key_config.direction = kGPIO_DigitalInput;
	key_config.interruptMode = kGPIO_IntFallingEdge;
	key_config.outputLogic = 1;
	gpio_init(GPIO1, 18, &key_config);

	GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);				/* 使能GIC中对应的中断 */
	system_register_irqhandler(GPIO1_Combined_16_31_IRQn, (system_irq_handler_t)gpio1_io18_irqhandler, NULL);	/* 注册中断服务函数 */
	gpio_enableint(GPIO1, 18);								/* 使能GPIO1_IO18的中断功能 */
}

/*
 * @description			: GPIO1_IO18最终的中断处理函数
 * @param				: 无
 * @return 				: 无
 */
void gpio1_io18_irqhandler(void)
{ 

    // 读取中断标志位 判断是否是GPIO1 pin18脚的 外部中断
    if (gpio_getintflags(GPIO1, 18))
    {
        //启动定时器10ms定时,等待定时器10ms定时 触发中断 在 定时器的中断处理中 再去读取按键状态 完成消抖的无阻塞实现 掉用对应的按键处理函数 
        
        EPIT_ReStart(EPIT_GetInstance(1)); //重新启动定时器EPIT1 用于10ms定时

        gpio_clearintflags(GPIO1, 18); /* 清除中断标志位 */
    }
}



