#include "delay.h"
#include "gpt.h"


void delay_Init(void) {
    GPT_Init();    // 初始化GPT定时器为1MHZ 1us计数的Free_run模式
}

void delay_us(volatile unsigned int delay_us) {

    uint32_t start_count, current_count, elapsed_count;
    GPT_Type* gptptrs = GPT_GetInstance(1); // 获取GPT1外设指针

    start_count = gptptrs->CNT; // 读取当前计数值
    elapsed_count = 0;

    while (elapsed_count < delay_us) {
        current_count = gptptrs->CNT; // 读取当前计数值
        if (current_count >= start_count) {
            elapsed_count = current_count - start_count; // 计算经过的计数值
        } else {
            // 处理计数器溢出情况
            elapsed_count = (0xFFFFFFFF - start_count) + current_count + 1;
        }
    }

}

void delay_ms(volatile unsigned int delay_ms) {
    while (delay_ms--) {
        delay_us(1000); // 每次延时1000微秒，即1毫秒
    }
}


/*
 * @description : 短时间延时函数
 * @param - n : 要延时循环次数(空操作循环次数，模式延时)
 * @return : 无
 */
void delay_short(volatile unsigned int n)
{
    while (n--)
    {
    }
}

/*
 * @description : 延时函数,在 396Mhz 的主频下延时时间大约为 1ms
 * @param - n : 要延时的 ms 数
 * @return : 无
 */
void delay(volatile unsigned int n)
{
    while (n--)
    {
        delay_short(0x7ff);
    }
}