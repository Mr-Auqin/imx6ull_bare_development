#include "gpt.h"


static void BSP_GPT_Init(GPT_Type *GPT, GPTconfig_t *GPTconfig);
static void BSP_GPT_start(GPT_Type *GPT);
static void BSP_GPT_stop(GPT_Type *GPT);


uint32_t GPT_base_add[] = GPT_BASE_ADDRS;
GPT_Type* GPT_base_ptrs[] = GPT_BASE_PTRS;
uint32_t GPT_irqs[] = GPT_IRQS;

GPTconfig_t GPTconfig;


GPT_Type* GPT_GetInstance(uint32_t GPTNum)   
{
    return GPT_base_ptrs[GPTNum];
}

static void BSP_GPT_Init(GPT_Type *GPT, GPTconfig_t *GPTconfig)
{
    // 获取GPT外设的寄存器结构体指针
    GPT_Type *gptptrs = GPT;
    //复位GPT外设
    gptptrs->CR |= GPT_CR_SWR(GPTconfig->softwareReset);
    //等待GPT复位完成
    while ((1==(gptptrs->CR & GPT_CR_SWR_MASK)));
    
    // 配置GPT控制寄存器CR
    gptptrs->CR = GPT_CR_FRR(GPTconfig->freeRunOrRestartMode) |  /*< Free_Run or Restart mode */
                  GPT_CR_CLKSRC(GPTconfig->clockSource);         /*< GPT Clock source selection */

    // 配置GPT预分频寄存器PR
    gptptrs->PR = GPT_PR_PRESCALER(GPTconfig->prescalarValue);

}

static void BSP_GPT_start(GPT_Type* GPT) {

    //获取GPT外设的寄存器结构体指针
    GPT_Type* gptptrs = GPT;
    //配置GPT控制寄存器CR 使能GPT外设
    gptptrs->CR |= GPT_CR_EN(1);

}



static __attribute__((unused)) void BSP_GPT_stop(GPT_Type* GPT) {

    //获取GPT外设的寄存器结构体指针
    GPT_Type* gptptrs = GPT;
    //配置GPT控制寄存器CR 失能GPT外设
    gptptrs->CR &= ~GPT_CR_EN_MASK;    //对应位清零
    gptptrs->CR |= GPT_CR_EN(0);      //设置失能

}




void GPT_Init(void) {

    //获取GPT1外设的寄存器结构体指针
    GPT_Type* gptptrs = GPT_GetInstance(1);
    //GPTconfig_t 功能配置结构体
    GPTconfig_t GPTconfig;
    //GPTconfig_t 结构体清零
    memset(&GPTconfig, 0, sizeof(GPTconfig_t));
    //配置结构体成员：目标10ms中断一次
    GPTconfig.clockSource = GPT_CLKSRC_ipg;        /*< ipg_clk reference clock：66MHZ */
    GPTconfig.prescalarValue = GPT_PRESCALAR_Div65; /*< Divide by 65：66MHZ/65=1MHZ */
    GPTconfig.softwareReset = GPT_SWR_1;           /*< GPT Software reset */
    GPTconfig.freeRunOrRestartMode = GPT_FRR_1;    /*< Free_Run mode: from 0 up to 0xFFFFFFFF */    

    // 初始化GPT1外设
    BSP_GPT_Init(gptptrs, &GPTconfig);

    //启动GPT1定时器
    BSP_GPT_start(gptptrs);

}


