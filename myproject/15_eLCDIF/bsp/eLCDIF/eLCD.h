#ifndef __ELCD_H
#define __ELCD_H

#include "imx6ul.h"
#include "stdio.h"


typedef struct __ELCD_Init
{



}ELCD_Init_t;


typedef struct __ELCD_IT_Config
{



}ELCD_IT_Config_t;


/**
 * @brief  UART handle Structure definition
 */
typedef struct __ELCD_HandleTypeDef
{
    LCDIF_Type *Instance;                                                      /*!< LCDIF_Instance  */
    ELCD_Init_t Init;                                                          /*!< LCDIF Init parameters      */
    ELCD_IT_Config_t IT_Config;                                                /*!< LCDIF interrupt configuration parameters      */
    const uint8_t *pTxBuffPtr;                                                 /*!< Pointer to LCDIF Tx transfer Buffer */
    uint16_t TxXferSize;                                                       /*!< LCDIF Tx Transfer size              */
    __IO uint16_t TxXferCount;                                                 /*!< LCDIF Tx Transfer Counter           */
    uint8_t *pRxBuffPtr;                                                       /*!< Pointer to LCDIF Rx transfer Buffer */
    uint16_t RxXferSize;                                                       /*!< LCDIF Rx Transfer size              */
    __IO uint16_t RxXferCount;                                                 /*!< LCDIF Rx Transfer Counter           */
    void (*TxHalfCpltCallback)(struct __ELCD_HandleTypeDef *hELCD);            /*!< LCDIF Tx Half Complete Callback        */
    void (*TxCpltCallback)(struct __ELCD_HandleTypeDef *hELCD);                /*!< LCDIF Tx Complete Callback             */
    void (*RxHalfCpltCallback)(struct __ELCD_HandleTypeDef *hELCD);            /*!< LCDIF Rx Half Complete Callback        */
    void (*RxCpltCallback)(struct __ELCD_HandleTypeDef *hELCD);                /*!< LCDIF Rx Complete Callback             */
    void (*ErrorCallback)(struct __ELCD_HandleTypeDef *hELCD);                 /*!< LCDIF Error Callback                   */
    void (*AbortCpltCallback)(struct __ELCD_HandleTypeDef *hELCD);             /*!< LCDIF Abort Complete Callback          */
    void (*AbortTransmitCpltCallback)(struct __ELCD_HandleTypeDef *hELCD);     /*!< LCDIF Abort Transmit Complete Callback */
    void (*AbortReceiveCpltCallback)(struct __ELCD_HandleTypeDef *hELCD);      /*!< LCDIF Abort Receive Complete Callback  */
    void (*WakeupCallback)(struct __ELCD_HandleTypeDef *hELCD);                /*!< LCDIF Wakeup Callback                  */
    void (*RxFifoFullCallback)(struct __ELCD_HandleTypeDef *hELCD);            /*!< LCDIF Rx Fifo Full Callback            */
    void (*TxFifoEmptyCallback)(struct __ELCD_HandleTypeDef *hELCD);           /*!< LCDIF Tx Fifo Empty Callback           */
    void (*RxEventCallback)(struct __ELCD_HandleTypeDef *hELCD, uint16_t Pos); /*!< LCDIF Reception Event Callback     */
    void (*MspInitCallback)(struct __ELCD_HandleTypeDef *hELCD);               /*!< LCDIF Msp Init callback                */
    void (*MspDeInitCallback)(struct __ELCD_HandleTypeDef *hELCD);             /*!< LCDIF Msp DeInit callback              */
} ELCD_HandleTypeDef;





void eLCDIF_Init(void);

void BSP_eLCDIF_Init(ELCD_HandleTypeDef *ElcdHandle);









#endif /* __ELCD_H */
