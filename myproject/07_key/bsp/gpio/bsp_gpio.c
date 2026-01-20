#include "bsp_gpio.h"


/// @brief 初始化GPIO
/// @param base 
/// @param pin 
/// @param config 
void gpio_init(GPIO_Type *base, int pin, gpio_pin_config_t *config)
{

    if(config->direction == kGPIO_DigitalOutput) {

        /* 1、初始化 GPIO, GPIO_IO_pin 设置为输出 */
        base->GDIR |= (1 << pin); /* 设置 GPIO_IO_pin 为输出 */

        /*2、输出默认电平*/
        gpio_pinwrite(base, pin, config->outputLogic);

    }else{
        /* 设置 GPIO_IO_pin 为输入 */
        base->GDIR &= ~(1 << pin);
    }   

}


/// @brief GPIO 引脚读操作
/// @param base 
/// @param pin 
/// @return  int   引脚的状态        
int gpio_pinread(GPIO_Type *base, int pin)
{
    if((base->DR >> pin) & 0x01){
        return 1;/* 高电平*/
    }else{
        return 0;/* 低电平*/
    }
}


/// @brief GPIO 引脚写操作
/// @param base 
/// @param pin 
/// @param value 
void gpio_pinwrite(GPIO_Type *base, int pin, int value)
{
    if(value){
        base->DR |=  (1 << pin);/* 输出高电平*/
    }else{      
        base->DR &= ~(1 << pin);/* 输出低电平*/     
    }
}





