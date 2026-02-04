#include "bsp_key.h"
#include "bsp_gpio.h"
#include "delay.h"


void key_init(void)
{
    gpio_pin_config_t key_config;   
    /* 按键初始化代码放在这里 */    
    /* 1、初始化 IO 复用, 复用为 GPIO1_IO18 */
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);

    /* 2、、配置 UART1_CTS_B 的 IO PAD 属性 
     *bit 16:0 HYS 开启
     *bit [15:14]: 01 47K 上拉
     *bit [13]: 1 pull 功能
     *bit [12]: 0 pull/keeper 功能  disable
     *bit [11]: 0 关闭开路输出
     *bit [7:6]: 10 速度 100Mhz
     *bit [5:3]: 110 R0/6 驱动能力
     *bit [0]: 0 低转换率
     */
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, IOMUXC_SW_PAD_CTL_PAD_HYS(1)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_PUS(1)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_PUE(1)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_PKE(1)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_ODE(0)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_SPEED(2)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_DSE(6)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_SRE(0));

    /* 3、初始化 GPIO, GPIO1_IO18 设置为输入 */
    key_config.direction = kGPIO_DigitalInput; /* 输入 */
    key_config.outputLogic = 0;                /* 默认输出电平为低 */  
    gpio_init(GPIO1, 18, &key_config);

}

int key_getvalue(void)
{
    int ret = 0;
    static unsigned char release = 1;//按键松开
    /* 读取按键值的代码放在这里 */
    if((release==1) && gpio_pinread(GPIO1, 18) == 0){/* 按键释放状态下 按键KEY0按下 */
        delay(10);/* 消抖 */
        if((release==1) && gpio_pinread(GPIO1, 18) == 0){/* 按键释放状态下 确保按键KEY0按下 */
            release = 0;/* 按键按下 */
            ret =  KEY0_VALUE;
        }
    }
    else if(gpio_pinread(GPIO1, 18) == 1){/* 按键KEY0松开 */
        ret =  0;
        release = 1;/* 按键松开 */
    }   
    return ret;
}
