#include "beep.h"
/*
 * @description : 初始化 LED 对应的 GPIO
 * @param : 无
 * @return : 无
 */
void beep_init(void)
{
    /* 1、初始化 IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01 复用, 复用为 GPIO5_IO01  用作GPIO输出功能*/
    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0);

    /* 2、配置 GPIO5_IO01 的 IO 属性为 GPIO 输出需要的属性 配置为输出时：pull/keeper 功能可以直接关闭
     *bit 16:0 HYS 关闭
     *bit [15:14]: 00 100K 下拉
     *bit [13]: 0 kepper 功能
     *bit [12]: 0 pull/keeper 功能  disable
     *bit [11]: 0 关闭开路输出
     *bit [7:6]: 10 速度 100Mhz
     *bit [5:3]: 110 R0/6 驱动能力
     *bit [0]: 0 低转换率
     */
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, IOMUXC_SW_PAD_CTL_PAD_HYS(0)|\
                                                            IOMUXC_SW_PAD_CTL_PAD_PUS(0)|\
                                                            IOMUXC_SW_PAD_CTL_PAD_PUE(0)|\
                                                            IOMUXC_SW_PAD_CTL_PAD_PKE(0)|\
                                                            IOMUXC_SW_PAD_CTL_PAD_ODE(0)|\
                                                            IOMUXC_SW_PAD_CTL_PAD_SPEED(2)|\
                                                            IOMUXC_SW_PAD_CTL_PAD_DSE(6)|\
                                                            IOMUXC_SW_PAD_CTL_PAD_SRE(0));


    /* 3、初始化 GPIO5, GPIO5_IO01 设置为输出 */
    GPIO5->GDIR |= (1 << 1); /* 设置 GPIO5_IO01 为输出 */

    /* 4、设置 GPIO5_IO01 输出高电平，关闭 Beep */
    GPIO5->DR |= (1 << 1);/* 输出高电平，关闭 Beep */
}

/*
 * @description : 打开 Beep 蜂鸣器
 * @param : 无
 * @return : 无
 */
void beep_on(void)
{
    /*
     * 将 GPIO5_DR 的 bit1 清零
     */
    GPIO5->DR &= ~(1 << 1);/* 输出低电平，Beep 蜂鸣器 */
}

/*
 * @description : 关闭 Beep 蜂鸣器
 * @param : 无
 * @return : 无
 */
void beep_off(void)
{
    /* 设置 GPIO5_IO01 输出高电平，关闭 Beep */
    GPIO5->DR |= (1 << 1);/* 输出高电平，关闭 Beep */
}

void beep_toggle(void)
{
    /* 翻转 GPIO5_IO01 引脚电平状态 */
    GPIO5->DR ^= (1 << 1);
}
      