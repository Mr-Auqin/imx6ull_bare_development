#include "epit.h"
#include "led.h"
#include "bsp_int.h"
#include "bsp_gpio.h"
#include "beep.h"

static void BSP_EPIT_Init(EPIT_Type *EPIT, EPITconfig_t *EPITconfig);
static void BSP_EPIT_start(EPIT_Type *EPIT);
static void BSP_EPIT_stop(EPIT_Type *EPIT);

static void BSP_EPIT_enableinterrupts(EPIT_Type *EPIT);
static void BSP_EPIT_disableinterrupts(EPIT_Type *EPIT);
static uint8_t BSP_EPIT_getstatus(EPIT_Type *EPIT, uint32_t flag);
static void BSP_EPIT_clearstatus(EPIT_Type *EPIT, uint32_t flag);

uint32_t EPIT_base_add[] = { 0u, EPIT1_BASE, EPIT2_BASE };
EPIT_Type* EPIT_base_ptrs[] = { (EPIT_Type *)0u, EPIT1, EPIT2 };
uint32_t EPIT_irqs[] = { NotAvail_IRQn, EPIT1_IRQn, EPIT2_IRQn };

EPITconfig_t EPITconfig;




EPIT_Type* EPIT_GetInstance(uint32_t EPITNum)   
{
    return EPIT_base_ptrs[EPITNum];
};


static void BSP_EPIT_Init(EPIT_Type* EPIT, EPITconfig_t* EPITconfig) {

    //获取EPIT外设的寄存器结构体指针
    EPIT_Type* epitptrs = EPIT;
    //配置EPIT控制寄存器CR
    epitptrs->CR = EPIT_CR_CLKSRC(EPITconfig->clockSource) |
                   EPIT_CR_OM(EPITconfig->outputMode) |
                   EPIT_CR_STOPEN(EPITconfig->stopModeEnable) |
                   EPIT_CR_WAITEN(EPITconfig->waitModeEnable) |
                   EPIT_CR_DBGEN(EPITconfig->debugModeEnable) |
                   EPIT_CR_IOVW(EPITconfig->counterOverwriteEnable) |
                   EPIT_CR_SWR(EPITconfig->softwareReset) |
                   EPIT_CR_PRESCALAR(EPITconfig->prescalarValue) |
                   EPIT_CR_ENMOD(EPITconfig->enableMode) |
                   EPIT_CR_RLD(EPITconfig->reloadControl);
    //配置EPIT加载寄存器LR
    epitptrs->LR = EPIT_LR_LOAD(EPITconfig->LoadRegister);
    //配置EPIT比较寄存器CMPR        
    epitptrs->CMPR = EPIT_CMPR_COMPARE(EPITconfig->CompareRegister);

}


static void BSP_EPIT_start(EPIT_Type* EPIT) {

    //获取EPIT外设的寄存器结构体指针
    EPIT_Type* epitptrs = EPIT;
    //配置EPIT控制寄存器CR 使能EPIT外设
    epitptrs->CR |= EPIT_CR_EN(1);

}



static  void BSP_EPIT_stop(EPIT_Type* EPIT) {

    //获取EPIT外设的寄存器结构体指针
    EPIT_Type* epitptrs = EPIT;
    //配置EPIT控制寄存器CR 失能EPIT外设
    epitptrs->CR &= ~EPIT_CR_EN_MASK;    //对应位清零
    epitptrs->CR |= EPIT_CR_EN(0);      //设置失能

}



static void BSP_EPIT_enableinterrupts(EPIT_Type* EPIT) {

    //获取EPIT外设的寄存器结构体指针
    EPIT_Type* epitptrs = EPIT;
    //配置EPIT控制寄存器CR 输出比较中断使能位 使能
    epitptrs->CR |= EPIT_CR_OCIEN(1);

}


static __attribute__((unused)) void BSP_EPIT_disableinterrupts(EPIT_Type* EPIT) {

    //获取EPIT外设的寄存器结构体指针
    EPIT_Type* epitptrs = EPIT;
    //配置EPIT控制寄存器CR 输出比较中断使能位  失能
    epitptrs->CR &= ~EPIT_CR_OCIEN_MASK;    //对应位清零
    epitptrs->CR |= ~EPIT_CR_OCIEN(0);      //设置失能

}


static uint8_t BSP_EPIT_getstatus(EPIT_Type* EPIT, uint32_t flag) {

    //获取EPIT外设的寄存器结构体指针
    EPIT_Type* epitptrs = EPIT;
    //获取EPIT状态寄存器SR 指定标志位状态
    return (uint8_t)(epitptrs->SR & flag);

}


static void BSP_EPIT_clearstatus(EPIT_Type* EPIT, uint32_t flag) {

    //获取EPIT外设的寄存器结构体指针
    EPIT_Type* epitptrs = EPIT;
    //清除EPIT状态寄存器SR 指定标志位
    epitptrs->SR |= flag;

}


void EPIT_Init(void) {

    //获取EPIT1外设的寄存器结构体指针
    EPIT_Type* epitptrs = EPIT_GetInstance(1);
    //EPITconfig_t 功能配置结构体
    EPITconfig_t EPITconfig;
    //EPITconfig_t 结构体清零
    memset(&EPITconfig, 0, sizeof(EPITconfig_t));
    //配置结构体成员：目标10ms中断一次
    EPITconfig.clockSource = EPIT_CLKSRC_ipg;        /*< High-frequency reference clock：66MHZ */
    EPITconfig.outputMode = EPIT_OM_0;               /*< Output mode 0：EPIT output is disconnected from pad*/
    EPITconfig.stopModeEnable = EPIT_STOPEN_1;       /*< Stop mode 0：EPIT is disabled in stop mode*/
    EPITconfig.waitModeEnable = EPIT_WAITEN_1;       /*< Wait mode 0：EPIT is disabled in wait mode*/
    EPITconfig.debugModeEnable = EPIT_DBGEN_1;       /*< Debug mode 0：EPIT is disabled in debug mode*/
    EPITconfig.counterOverwriteEnable = EPIT_IOVW_1; /*< 1：Write to load register results in immediate overwriting of counter value*/
    EPITconfig.softwareReset = EPIT_SWR_0;           /*< 0：EPIT is out of reset*/
    EPITconfig.prescalarValue = EPIT_PRESCALAR_Div1; /*< 0：Divide by 1：66MHZ*/
    EPITconfig.enableMode = EPIT_ENMOD_1;            /*< 1：Counter starts count from load value (RLD=1) or 0xFFFF_FFFF (If RLD=0) */
    EPITconfig.reloadControl = EPIT_RLD_1;           /*< 1：When the counter reaches zero it reloads from the modulus register (set-and-forget mode)*/
    EPITconfig.LoadRegister = 66000000/100;          /*Load register */   
    EPITconfig.CompareRegister = 0;                  /*Compare register */
    // 初始化EPIT1外设
    BSP_EPIT_Init(epitptrs, &EPITconfig);


    //使能GIC中对应的EPIT1中断
    GIC_EnableIRQ(EPIT_irqs[1]);
    //注册EPIT1中断服务函数
    system_register_irqhandler(EPIT_irqs[1], (system_irq_handler_t)EPIT_irqhandler, NULL);
    
    //清除EPIT1的中断标志位
    BSP_EPIT_clearstatus(epitptrs, EPIT_OCIF_Mask);
    //使能EPIT1中断
    BSP_EPIT_enableinterrupts(epitptrs);    

}




void EPIT_ReStart(EPIT_Type* EPIT) {

    //获取EPIT外设的寄存器结构体指针
    EPIT_Type* epitptrs = EPIT;
    //先停止EPIT定时器
    BSP_EPIT_stop(epitptrs);
    //清除EPIT的中断标志位
    BSP_EPIT_clearstatus(epitptrs, EPIT_OCIF_Mask);
    //重新启动EPIT定时器
    BSP_EPIT_start(epitptrs);   

}




//定时 10ms 进入定时器EPIT1中断服务函数
void EPIT_irqhandler(void) {

    if(BSP_EPIT_getstatus(EPIT_GetInstance(1), EPIT_OCIF_Mask)) {
        //在这里添加用户代码
        BSP_EPIT_stop(EPIT_GetInstance(1)); //停止定时器
        //再次读取key的状态，完成按键消抖
        if(gpio_pinread(GPIO1, 18) == 0){/* 消抖后读取按键的状态 确保按键KEY0按下 */
            //完成按键按下的任务处理
            beep_toggle();  //翻转蜂鸣器状态
        }       
        //清除EPIT1的中断标志位
        BSP_EPIT_clearstatus(EPIT_GetInstance(1), EPIT_OCIF_Mask);
    }

}


