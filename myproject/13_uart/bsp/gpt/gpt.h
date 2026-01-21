#ifndef __GPT_H
#define __GPT_H

#include "imx6ul.h"


/*< Free_Run or Restart mode */
#define GPT_FRR_0 0 /*< 0：Restart mode */
#define GPT_FRR_1 1 /*< 1：Free_Run mode */


/*< Clock source selection */
#define GPT_CLKSRC_off 0          /*< No clock */
#define GPT_CLKSRC_ipg 1          /*< Peripheral Clock (ipg_clk) */
#define GPT_CLKSRC_ipg_highfreq 2 /*< High-frequency reference clock */
#define GPT_CLKSRC_ext 3          /*< External Clock */
#define GPT_CLKSRC_ipg_32k 4      /*< Low-frequency reference clock */
#define GPT_CLKSRC_ipg_24M 5      /*< Crystal oscillator as Reference Clock (ipg_clk_24M) */

/*< GPT Software reset */
#define GPT_SWR_0 0 /*< 0：GPT is not in reset state*/
#define GPT_SWR_1 1 /*< 1：GPT is in reset state*/

/*< GPT Counter clock prescaler value */
#define GPT_PRESCALAR_Div1 0   /*< 0：Divide by 1*/
#define GPT_PRESCALAR_Div2 1   /*< 1：Divide by 2*/
#define GPT_PRESCALAR_Div4 3   /*< 3：Divide by 4*/
#define GPT_PRESCALAR_Div65 66 /*< 65：Divide by 66*/

/*< This bit enables the GPT */
#define GPT_EN_0 0 /*< 0：GPT is disabled */
#define GPT_EN_1 1 /*< 1：GPT is enabled */



/* GPT 配置结构体 */
typedef struct
{
    uint32_t softwareReset;        /*< GPT Software reset */
    uint32_t freeRunOrRestartMode; /*< Free_Run or Restart mode */
    uint32_t clockSource;          /*< GPT Clock source selection */
    uint32_t prescalarValue;       /*< GPT Counter clock prescaler value */

} GPTconfig_t;

// GPT 外设操作函数声明  
GPT_Type* GPT_GetInstance(uint32_t GPTNum);   
void GPT_Init(void);   




#endif /* __GPT_H */
