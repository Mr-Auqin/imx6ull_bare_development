#ifndef _BSP_INT_H
#define _BSP_INT_H
#include "imx6ul.h"
/***************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 bsp_int.c
作者	   : 左忠凯
版本	   : V1.0
描述	   : 中断驱动头文件。
其他	   : 无
论坛 	   : www.wtmembed.com
日志	   : 初版V1.0 2019/1/4 左忠凯创建
***************************************************************/

/* 中断服务函数形式 */ 
typedef void (*system_irq_handler_t) (unsigned int giccIar, void *param);
//定义一个函数指针类型 别名为system_irq_handler_t

 
/* 中断服务函数结构体*/
typedef struct _sys_irq_handle
{
    system_irq_handler_t irqHandler; /* 中断服务函数指针 */
    void *userParam;                 /* 中断服务函数参数指针 */
} sys_irq_handle_t;


/* 函数声明 */
void int_init(void);
//中断初始化
void system_irqtable_init(void);
//系统(外设)中断表初始化
void system_register_irqhandler(IRQn_Type irq, system_irq_handler_t handler, void *userParam);
//系统(外设注册中断)
void system_irqhandler(unsigned int giccIar); 
//系统(外设)中断处理函数--对接.s汇编启动文件中的 C 函数入口 system_irqhandler 函数
void default_irqhandler(unsigned int giccIar, void *userParam); 
//默认中断处理函数的申明 和 system_irq_handler_t 函数指针是同类型参数的函数 ,此函数用于 初始化外设默认的中断服务函数

#endif
