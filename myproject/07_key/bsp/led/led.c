#include "led.h"
/*
 * @description : 初始化 LED 对应的 GPIO
 * @param : 无
 * @return : 无
 */
void led_init(void)
{
    /* 1、初始化 GPIO1_IO03 Pad 复用, 复用为 GPIO1_IO03  用作GPIO输出功能*/
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0);

    /* 2、配置 GPIO1_IO03 的 IO 属性为 GPIO 输出需要的属性 配置为输出时：pull/keeper 功能可以直接关闭
     *bit 16:0 HYS 关闭
     *bit [15:14]: 00 100K 下拉
     *bit [13]: 0 kepper 功能
     *bit [12]: 0 pull/keeper 功能  disable
     *bit [11]: 0 关闭开路输出
     *bit [7:6]: 10 速度 100Mhz
     *bit [5:3]: 110 R0/6 驱动能力
     *bit [0]: 0 低转换率
     */
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03, IOMUXC_SW_PAD_CTL_PAD_HYS(0)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_PUS(0)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_PUE(0)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_PKE(0)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_ODE(0)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_SPEED(2)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_DSE(6)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_SRE(0));


    /* 3、初始化 GPIO, GPIO1_IO03 设置为输出 */
    GPIO1->GDIR |= (1 << 3); /* 设置 GPIO1_IO03 为输出 */

    /* 4、设置 GPIO1_IO03 输出高电平，关闭 LED0 */
    GPIO1->DR |= (1 << 3);/* 输出高电平，关闭 LED */
}

/*
 * @description : 打开 LED 灯
 * @param : 无
 * @return : 无
 */
void led_on(void)
{
    /*
     * 将 GPIO1_DR 的 bit3 清零
     */
    GPIO1->DR &= ~(1 << 3);/* 输出低电平，打开 LED */
}

/*
 * @description : 关闭 LED 灯
 * @param : 无
 * @return : 无
 */
void led_off(void)
{
    /*
     * 将 GPIO1_DR 的 bit3 置 1
     */
    GPIO1->DR |= (1 << 3);/* 输出高电平，关闭 LED */
}