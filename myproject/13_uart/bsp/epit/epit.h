#ifndef __EPIT_H
#define __EPIT_H

#include "imx6ul.h"


#define EPIT_INSTANCE_COUNT 2


/*< Clock source selection */
#define EPIT_CLKSRC_off 0          /*< Clock off */
#define EPIT_CLKSRC_ipg 1          /*< Peripheral clock */
#define EPIT_CLKSRC_ipg_highfreq 2 /*< High-frequency reference clock */
#define EPIT_CLKSRC_ipg_32k 3      /*< Low-frequency reference clock */

/*< EPIT output mode */
#define EPIT_OM_0 0 /*< Output mode 0：EPIT output is disconnected from pad*/
#define EPIT_OM_1 1 /*< Output mode 1：Toggle output pin*/
#define EPIT_OM_2 2 /*< Output mode 2：Clear output pin*/
#define EPIT_OM_3 3 /*< Output mode 3：Set output pin*/

/*< EPIT stop mode enable */
#define EPIT_STOPEN_0 0 /*< Stop mode 0：EPIT is disabled in stop mode*/
#define EPIT_STOPEN_1 1 /*< Stop mode 1：EPIT is enabled in stop mode*/

/*< EPIT wait mode enable */
#define EPIT_WAITEN_0 0 /*< Wait mode 0：EPIT is disabled in wait mode*/
#define EPIT_WAITEN_1 1 /*< Wait mode 1：EPIT is enabled in wait mode*/

/*< EPIT Debug mode enable */
#define EPIT_DBGEN_0 0 /*< Debug mode 0：EPIT is disabled in debug mode*/
#define EPIT_DBGEN_1 1 /*< Debug mode 1：EPIT is enabled in debug mode*/

/*< EPIT counter overwrite enable */
#define EPIT_IOVW_0 0 /*< 0：Write to load register does not result in counter value being overwritten*/
#define EPIT_IOVW_1 1 /*< 1：Write to load register results in immediate overwriting of counter value*/

/*< EPIT Software reset */
#define EPIT_SWR_0 0 /*< 0：EPIT is out of reset*/
#define EPIT_SWR_1 1 /*< 1：EPIT is undergoing reset*/

/*< EPIT Counter clock prescaler value */
#define EPIT_PRESCALAR_Div1 0 /*< 0：Divide by 1*/
#define EPIT_PRESCALAR_Div2 1 /*< 1：Divide by 2*/
#define EPIT_PRESCALAR_Div4 3 /*< 3：Divide by 4*/
#define EPIT_PRESCALAR_Div8 7 /*< 7：Divide by 8*/

/*< EPIT Counter reload control */
#define EPIT_RLD_0 0 /*< 0：When the counter reaches zero it rolls over to 0xFFFF_FFFF (free-running mode)*/
#define EPIT_RLD_1 1 /*< 1：When the counter reaches zero it reloads from the modulus register (set-and-forget mode)*/

/*< EPIT Output compare interrupt enable */
#define EPIT_OCIEN_0 0 /*< 0：Compare interrupt disabled*/
#define EPIT_OCIEN_1 1 /*< 1：Compare interrupt enabled*/

/*< EPIT enable mode. */
#define EPIT_ENMOD_0 0 /*< 0：Counter starts counting from the value it had when it was disabled. */
#define EPIT_ENMOD_1 1 /*< 1：Counter starts count from load value (RLD=1) or 0xFFFF_FFFF (If RLD=0) */

/*< This bit enables the EPIT */
#define EPIT_EN_0 0 /*< 0：EPIT is disabled */
#define EPIT_EN_1 1 /*< 1：EPIT is enabled */


/*< Output compare interrupt flag */
#define EPIT_OCIF_Mask  EPIT_SR_OCIF_MASK  /*< Output compare interrupt flag mask */


/* EPIT 配置结构体 */
typedef struct {
    uint32_t clockSource;                  /*< Clock source selection */
    uint32_t outputMode;                   /*< EPIT output mode */
    uint32_t stopModeEnable;               /*< EPIT stop mode enable */
    uint32_t waitModeEnable;               /*< EPIT wait mode enable */
    uint32_t debugModeEnable;              /*< EPIT debug mode enable */
    uint32_t counterOverwriteEnable;       /*< EPIT counter overwrite enable */
    uint32_t softwareReset;                /*< EPIT Software reset */
    uint32_t prescalarValue;               /*< EPIT Counter clock prescaler value */
    uint32_t reloadControl;                /*< EPIT Counter reload control */
    uint32_t outputCompareInterruptEnable; /*< EPIT Output compare interrupt enable */
    uint32_t enableMode;                   /*< EPIT enable mode. */
    uint32_t LoadRegister;                 /*Load register */
    uint32_t CompareRegister;              /*Compare register */
    uint32_t CounterRegister;              /*Counter register */
} EPITconfig_t; 

// EPIT 外设操作函数声明  
EPIT_Type* EPIT_GetInstance(uint32_t EPITNum);    
void EPIT_Init(void);   
void EPIT_ReStart(EPIT_Type* EPIT);

//EPIT中断服务函数声明
void EPIT_irqhandler(void);


#endif /* __EPIT_H */
