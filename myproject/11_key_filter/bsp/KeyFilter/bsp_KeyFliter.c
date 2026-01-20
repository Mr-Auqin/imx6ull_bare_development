#include "bsp_key.h"
#include "bsp_gpio.h"
#include "bsp_exit.h"
#include "epit.h"
#include "bsp_KeyFliter.h"

void KeyFiler_init(void)
{
    // 初始化按键对应PAD复用为GPIO PAD盘属性设置 GPIO输入配置  中断条件  启用外部中断 注册中断服务函数
    exit_init();
    //初始化定时器，用于按键消抖,定时10ms,使能中断，注册中断服务函数
    EPIT_Init();
}


