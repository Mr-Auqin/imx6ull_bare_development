#ifndef __IMX6UL_H
#define __IMX6UL_H
/***************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 imx6ul.h
作者	   : 左忠凯
版本	   : V1.0
描述	   : 包含一些常用的头文件,包含一些常用的头文件与共用通用底层寄存器配置操作接口。
其他	   : 无
论坛 	   : www.wtmembed.com
日志	   : 初版V1.0 2019/1/3 左忠凯创建
***************************************************************/
#include "cc.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"
#include "core_ca7.h"


// 参数说明：
// REG_VAULE:  要修改寄存器的值
// Field_MASK: 要修改的位的掩码
// Field_VAL:  要设置的目标值
// 示例:SET_BIT(UART1->UCR1,UART_UCR1_UARTEN_MASK,UART_UCR1_UARTEN(1));
// 优化版：直接操作内存地址，完成读-改-写
#define SET_BIT(REG_VAULE, Field_MASK, Field_VAL)              \
    do                                                         \
    {                                                          \
        /* 1. 读：读取当前值 */                                \
        volatile unsigned int _reg_val = REG_VAULE;            \
        /* 2. 改：计算新值 */                                  \
        _reg_val = ((_reg_val & ~(Field_MASK)) | (Field_VAL)); \
        /* 3. 写：写回内存地址（真正修改内存） */              \
        REG_VAULE = _reg_val;                                  \
    } while (0) // do-while保证宏作为语句调用时语法合法

#endif

