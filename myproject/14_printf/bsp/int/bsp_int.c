#include "bsp_int.h"
/***************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 bsp_int.c
作者	   : 左忠凯
版本	   : V1.0
描述	   : 中断驱动文件。
其他	   : 无
论坛 	   : www.wtmembed.com
日志	   : 初版V1.0 2019/1/4 左忠凯创建
***************************************************************/

/* 中断嵌套计数器 */
static unsigned int irqNesting;
//用于记录进入中断的次数,知道目前是否处于中断嵌套的状态

/* 中断服务函数表 */
//表里面存放了对应中断号的中断处理函数指针 和 这个中断处理函数的用户参数
static sys_irq_handle_t irqTable[NUMBER_OF_INT_VECTORS];

/*
 * @description	: 中断初始化函数
 * @param		: 无
 * @return 		: 无
 */
void int_init(void)
{
	GIC_Init(); 						/* 初始化GIC（全局中断控制器）:关闭所有中断,设置到默认的最低优先级*/
                                        /* 设置优先使用抢占式优先级,使能Group0的distribution  使能Group0的distribution与signaling*/
	system_irqtable_init();				/* 初始化中断表:填写默认的中断处理函数指针与用户参数指针*/
	__set_VBAR((uint32_t)0x87800000); 	/* 中断向量表偏移，偏移到链接的起始地址   		 */
}

/*
 * @description	: 初始化中断服务函数表 
 * @param		: 无
 * @return 		: 无
 */
void system_irqtable_init(void)
{
	unsigned int i = 0;
	irqNesting = 0;
	
	/* 先将所有的中断服务函数设置为默认值 */
	for(i = 0; i < NUMBER_OF_INT_VECTORS; i++)
	{
        //系统注册中断处理函数:为所有的中断号 注册默认中断处理函数指针 与 用户参数指针
		system_register_irqhandler((IRQn_Type)i,default_irqhandler, NULL);
	}
}

/*
 * @description			: 给指定的中断号注册中断服务函数 
 * @param - irq			: 要注册的中断号
 * @param - handler		: 要注册的中断处理函数
 * @param - usrParam	: 中断服务处理函数参数
 * @return 				: 无
 */
void system_register_irqhandler(IRQn_Type irq, system_irq_handler_t handler, void *userParam) 
{
    //对应的中断号中 赋值中断处理函数指针 与 这个中断对应的用户参数指针
	irqTable[irq].irqHandler = handler;
  	irqTable[irq].userParam = userParam;
}

/*
 * @description			: C语言中断服务函数，irq汇编中断服务函数会
 						  调用此函数，此函数通过在中断服务列表中查
 						  找指定中断号所对应的中断处理函数并执行。
 * @param - giccIar		: 中断号
 * @return 				: 无
 */
//C语言的系统中断处理函数 对应.s 汇编启动文件中调用的C函数接口
//这里的参数giccIar 是由汇编中提到的r0寄存器 传递过来的, arm规定C语言中的参数 一般由r0~r3 这4个寄存器进行传递,不建议参数超过4个
void system_irqhandler(unsigned int giccIar) 
{

   //获取中断号
   uint32_t intNum = giccIar & 0x3FFUL;
   
   /* 检查中断号是否符合要求 */
   if ((intNum == 1023) || (intNum >= NUMBER_OF_INT_VECTORS))
   {
	 	return;
   }
 
   irqNesting++;	/* 中断嵌套计数器加一 */

   /* 根据传递进来的中断号，在irqTable中调用确定的中断服务函数*/
   irqTable[intNum].irqHandler(intNum, irqTable[intNum].userParam);
 
   irqNesting--;	/* 中断执行完成，中断嵌套寄存器减一 */

}

/*
 * @description			: 默认中断服务函数
 * @param - giccIar		: 中断号
 * @param - usrParam	: 中断服务处理函数参数
 * @return 				: 无
 */
void default_irqhandler(unsigned int giccIar, void *userParam) 
{
	while(1) 
  	{
        //死循环 进入到默认的中断处理中就卡死
   	}
}



