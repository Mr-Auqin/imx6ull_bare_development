#ifndef __UART_H
#define __UART_H

#include "imx6ul.h"

/*UART_Config_Select*/
#define UART_clksource_from_pll3_80M 0
#define UART_clksource_from_osc_clk  1

#define UART_clk_divider_6 0
#define UART_clk_divider_5 1
#define UART_clk_divider_4 2
#define UART_clk_divider_3 3
#define UART_clk_divider_2 4
#define UART_clk_divider_1 5
#define UART_clk_divider_7 6

#define UART_mode_DCE 0    //最常用配置:DCE模式 芯片内部[TX引脚]是UART模块的TX脚  芯片内部[RX引脚]是UART模块的RX脚
#define UART_mode_DTE 1    //DTE模式:芯片内部[TX引脚]是UART模块的RX脚  芯片内部[RX引脚]是UART模块的TX脚  TX和RX需要交换管脚时使用

#define UART_WORDLENGTH_7B 0
#define UART_WORDLENGTH_8B 1

#define UART_PARITY_NONE 0
#define UART_PARITY_EVEN 1
#define UART_PARITY_ODD 2

#define UART_STOPBITS_1 0
#define UART_STOPBITS_2 1

#define UART_Sender_DISABLE 0
#define UART_Sender_ENABLE 1

#define UART_Receiver_DISABLE 0
#define UART_Receiver_ENABLE 1

/*UART_IT_Config_Select*/
#define INTERRUPT_DISABLE 0
#define INTERRUPT_ENABLE  1


#define UART_IT_CONFIG_IDLECONDITION_4_frames 0
#define UART_IT_CONFIG_IDLECONDITION_8_frames 1
#define UART_IT_CONFIG_IDLECONDITION_16_frames 2
#define UART_IT_CONFIG_IDLECONDITION_32_frames 3

/*UART_FLAG*/
#define UART_FLAG_RRDY UART_USR1_RRDY_MASK // Receiver Ready Interrupt / DMA Flag
#define UART_FLAG_IDLE UART_USR2_IDLE_MASK // Idle Condition Interrupt Flag
#define UART_FLAG_RDR UART_USR2_RDR_MASK   // Receive Data Ready Interrupt Flag
#define UART_FLAG_RXDS UART_USR1_RXDS_MASK // Receiver IDLE Interrupt Flag
#define UART_FLAG_TXFE UART_USR2_TXFE_MASK // Transmitter FIFO Empty Interrupt Flag
#define UART_FLAG_TRDY UART_USR1_TRDY_MASK // Transmitter Ready Interrupt / DMA Flag
#define UART_FLAG_TXDC UART_USR2_TXDC_MASK // Transmitter Complete Interrupt Flag
#define UART_FLAG_ORE UART_USR2_ORE_MASK   // Overrun Error Interrupt Flag

// 参数说明：
// REG_ADDR: 要修改内存地址
// Field_MASK: 要修改的位的掩码
// Field_VAL: 要设置的目标值
// 示例:SET_BIT(&UART1->UCR1,UART_UCR1_UARTEN_MASK,UART_UCR1_UARTEN(1));
// 优化版：直接操作内存地址，完成读-改-写
#define SET_BIT(REG_ADDR, Field_MASK, Field_VAL)               \
	do                                                         \
	{                                                          \
		/* 1. 读：从内存地址读取当前值 */                      \
		volatile unsigned int _reg_val = *(REG_ADDR);          \
		/* 2. 改：计算新值 */                                  \
		_reg_val = ((_reg_val & ~(Field_MASK)) | (Field_VAL)); \
		/* 3. 写：写回内存地址（真正修改内存） */              \
		*(REG_ADDR) = _reg_val;                                \
	} while (0) // do-while保证宏作为语句调用时语法合法



#define UART_Number 8   // 串口个数



typedef struct
{

    uint32_t clocksource;
    uint32_t clockdivider;
    uint32_t modeselect;
    uint32_t baudrate;
    uint32_t databits;
    uint32_t parity;
    uint32_t stopbits;
    uint32_t senderEnable;
    uint32_t receiverEnable;

} UART_Config_t;

typedef struct
{
    uint32_t IT_receiver_ready;         // the receiver ready interrupt Enable(达到RxFIFO 设置的 接收数据数量门槛 则触发串口中断)
    uint32_t IT_IDLE;                   // the IDLE interrupt Enable(RxFIFO数据为空+RX_DATA引脚达到由软件编程的空闲帧的数量则触发串口中断)
    uint32_t IT_Receiver_Data_Ready;    // Receiver Data Ready Interrupt Enable(至少接收到一个字节数据，则触发串口中断,此状态位满足无数据就绪可读时,自动清零,可用于轮询读取)
    uint32_t IT_Receiver_IDLE;          // the Receiver IDLE interrupt Enable(反应串口接收器的“工作状态”（空闲/接收中）,接收器空闲时,触发串口中断,推荐使用IDLE条件中断,不建议使用此中断)
    uint32_t IT_Idle_Condition;         // the Idle Condition Detect(配置空闲的检测条件,可设置为4/8/16/32帧空闲)
    uint32_t IT_transmitter_FIFO_empty; // the transmitter FIFO empty interrupt Enable(发送FIFO为空时，触发串口中断,当有数据写进去TxFIFO时，自动清零，也可用于轮询发送)
    uint32_t IT_transmitter_ready;      // the transmitter ready interrupt Enable(达到TxFIFO 设置的 低于其现有的数据数量门槛时， 则触发串口中断，即可以往TxFIFO中写入数据时，触发串口中断，当TxFIFO中数据数量超过其设置值时，自动清零，可用于轮询发送)
    uint32_t IT_Transmitter_Complete;   // the Transmitter Complete interrupt Enable(发送完成时，触发串口中断，可用于轮询发送，此状态位满足有数据写进去TxFIFO时，自动清零，可用于轮询读取)
    uint32_t IT_Receiver_Overrun;       // Receiver Overrun  Error Interrupt Enable(当接收器发生溢出错误时，触发串口中断)
    void (*irqHandler)(void);           // 中断服务函数

} UART_IT_Config_t;

typedef struct
{

    uint32_t __UART_UCR1_UARTEN;    // the UART UARTEN:0-Disable the UART,1-Enable the UART
    uint32_t __UART_UCR2_IRTS;      // Ignore RTS Pin: 1-Ignore the RTS pin
    uint32_t __UART_UCR2_PREN;      // Parity Enable: 0 Disable parity generator and checker
    uint32_t __UART_UCR2_PROE;      // Parity Odd Even: 0 Even parity  , 1 Odd parity
    uint32_t __UART_UCR2_STPB;      // Stop Bit Length: 0-1bit, 1-2bits
    uint32_t __UART_UCR2_WS;        // Word Size: 0-7bits, 1-8bits
    uint32_t __UART_UCR2_TXEN;      // Transmitter Enable: 0-Disable the transmitter, 1-Enable the transmitter
    uint32_t __UART_UCR2_RXEN;      // Receiver Enable: 0-Disable the receiver, 1-Enable the receiver
    uint32_t __UART_UCR2_SRST;      // Software Reset: 1-Do not reset the UART, 0-Reset the UART
    uint32_t __UART_UCR3_RXDMUXSEL; // In this chip, UARTs are used in MUXED mode, so that this bit should always be set.
    uint32_t __UART_UFCR_RFDIV;     // Reference Frequency Divider
    uint32_t __UART_UFCR_DCEDTE;    // DCE/DTE mode select:  0 DCE mode selected  1 DTE mode selected
    uint32_t __UART_UBIR_INC;       // Incremental Counter
    uint32_t __UART_UBMR_MOD;       // Decremental Counter
    uint32_t __UART_ONEMS_ONEMS;    // must contain the value of the UART internal frequency divided by 1000

} __UART_Register_field_config_t;

typedef struct
{
    uint32_t UART_UCR1_RRDYEN;   // the receiver ready interrupt Enable(达到RxFIFO 设置的 接收数据数量门槛 则触发串口中断)
    uint32_t UART_UCR1_IDEN;     // the IDLE interrupt Enable(RxFIFO数据为空+RX_DATA引脚达到由软件编程的空闲帧的数量则触发串口中断)
    uint32_t UART_UCR4_DREN;     // Receiver Data Ready Interrupt Enable(至少接收到一个字节数据，则触发串口中断,此状态位满足无数据就绪可读时,自动清零,可用于轮询读取)
    uint32_t UART_UCR3_RXDSEN;   // the Receiver IDLE interrupt Enable(反应串口接收器的“工作状态”（空闲/接收中）,接收器空闲时,触发串口中断,推荐使用IDLE条件中断,不建议使用此中断)
    uint32_t UART_UCR1_ICD;      // the Idle Condition Detect(配置空闲的检测条件,可设置为4/8/16/32帧空闲)
    uint32_t UART_UCR1_TXMPTYEN; // the transmitter FIFO empty interrupt Enable(发送FIFO为空时，触发串口中断,当有数据写进去TxFIFO时，自动清零，也可用于轮询发送)
    uint32_t UART_UCR1_TRDYEN;   // the transmitter ready interrupt Enable(达到TxFIFO 设置的 低于其现有的数据数量门槛时， 则触发串口中断，即可以往TxFIFO中写入数据时，触发串口中断，当TxFIFO中数据数量超过其设置值时，自动清零，可用于轮询发送)
    uint32_t UART_UCR4_TCEN;     // the Transmitter Complete interrupt Enable(发送完成时，触发串口中断，可用于轮询发送，此状态位满足有数据写进去TxFIFO时，自动清零，可用于轮询读取)
    uint32_t UART_UCR4_OREN;     // Receiver Overrun  Error Interrupt Enable(当接收器发生溢出错误时，触发串口中断)
} __UART_Register_IT_config_t;



/**
 * @brief  UART handle Structure definition
 */
typedef struct __UART_HandleTypeDef
{
    UART_Type *Instance;                                                       /*!< UART_Instance  */
    UART_Config_t Config;                                                      /*!< UART communication parameters      */
    UART_IT_Config_t IT_Config;                                                /*!< UART interrupt configuration parameters      */
    const uint8_t *pTxBuffPtr;                                                 /*!< Pointer to UART Tx transfer Buffer */
    uint16_t TxXferSize;                                                       /*!< UART Tx Transfer size              */
    __IO uint16_t TxXferCount;                                                 /*!< UART Tx Transfer Counter           */
    uint8_t *pRxBuffPtr;                                                       /*!< Pointer to UART Rx transfer Buffer */
    uint16_t RxXferSize;                                                       /*!< UART Rx Transfer size              */
    __IO uint16_t RxXferCount;                                                 /*!< UART Rx Transfer Counter           */
    void (*TxHalfCpltCallback)(struct __UART_HandleTypeDef *huart);            /*!< UART Tx Half Complete Callback        */
    void (*TxCpltCallback)(struct __UART_HandleTypeDef *huart);                /*!< UART Tx Complete Callback             */
    void (*RxHalfCpltCallback)(struct __UART_HandleTypeDef *huart);            /*!< UART Rx Half Complete Callback        */
    void (*RxCpltCallback)(struct __UART_HandleTypeDef *huart);                /*!< UART Rx Complete Callback             */
    void (*ErrorCallback)(struct __UART_HandleTypeDef *huart);                 /*!< UART Error Callback                   */
    void (*AbortCpltCallback)(struct __UART_HandleTypeDef *huart);             /*!< UART Abort Complete Callback          */
    void (*AbortTransmitCpltCallback)(struct __UART_HandleTypeDef *huart);     /*!< UART Abort Transmit Complete Callback */
    void (*AbortReceiveCpltCallback)(struct __UART_HandleTypeDef *huart);      /*!< UART Abort Receive Complete Callback  */
    void (*WakeupCallback)(struct __UART_HandleTypeDef *huart);                /*!< UART Wakeup Callback                  */
    void (*RxFifoFullCallback)(struct __UART_HandleTypeDef *huart);            /*!< UART Rx Fifo Full Callback            */
    void (*TxFifoEmptyCallback)(struct __UART_HandleTypeDef *huart);           /*!< UART Tx Fifo Empty Callback           */
    void (*RxEventCallback)(struct __UART_HandleTypeDef *huart, uint16_t Pos); /*!< UART Reception Event Callback     */
    void (*MspInitCallback)(struct __UART_HandleTypeDef *huart);               /*!< UART Msp Init callback                */
    void (*MspDeInitCallback)(struct __UART_HandleTypeDef *huart);             /*!< UART Msp DeInit callback              */
} UART_HandleTypeDef;

/**
 * @brief Get a UART instance
 *
 * This function returns a UART instance based on the given UART number
 *
 * @param UARTNum The UART number
 * @return The UART instance
 */
UART_Type *UART_GetInstance(uint32_t UARTNum);

void UART_Init(void);
void uart1_irq_handler(void);

void putc(unsigned char c);
void puts(char *str);
unsigned char getc(void);









/**
 * @brief Initialize a UART instance
 *
 * This function initializes a UART instance based on the given configuration
 *
 * @param uart The UART instance to be initialized
 * @param uartconfig The UART configuration
 */
void BSP_UART_Init(UART_HandleTypeDef *huart);
void BSP_UART_IT_Init(UART_HandleTypeDef *huart);
void BSP_UART_Enable(UART_HandleTypeDef *huart);
void BSP_UART_Disable(UART_HandleTypeDef *huart);

/**
 * @brief Transmits data through a UART instance
 *
 * This function transmits the given data through the given UART instance
 *
 * @param uart The UART instance to be used for transmission
 * @param data The data to be transmitted
 * @param len The length of the data to be transmitted
 */
void BSP_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *data, uint32_t len);

/**
 * @brief Receives data through a UART instance
 *
 * This function receives the given data through the given UART instance
 *
 * @param uart The UART instance to be used for reception
 * @param data The data to be received
 * @param len The length of the data to be received
 */
void BSP_UART_Receive(UART_HandleTypeDef *huart, uint8_t *data, uint32_t len);

/**
 * @brief Transmits data through a UART instance using interrupts
 *
 * This function transmits the given data through the given UART instance using interrupts
 *
 * @param uart The UART instance to be used for transmission
 * @param data The data to be transmitted
 * @param len The length of the data to be transmitted
 */
void BSP_UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *data, uint32_t len);

/**
 * @brief Receives data through a UART instance using interrupts
 *
 * This function receives the given data through the given UART instance using interrupts
 *
 * @param uart The UART instance to be used for reception
 * @param data The data to be received
 * @param len The length of the data to be received
 */
void BSP_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *data, uint32_t len);

/**
 * @brief Transmits data through a UART instance using DMA
 *
 * This function transmits the given data through the given UART instance using DMA
 *
 * @param uart The UART instance to be used for transmission
 * @param data The data to be transmitted
 * @param len The length of the data to be transmitted
 */
void BSP_UART_Transmit_DMA(UART_HandleTypeDef *huart, uint8_t *data, uint32_t len);

/**
 * @brief Receives data through a UART instance using DMA
 *
 * This function receives the given data through the given UART instance using DMA
 *
 * @param uart The UART instance to be used for reception
 * @param data The data to be received
 * @param len The length of the data to be received
 */
void BSP_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *data, uint32_t len);

void BSP_UART_IRQ_Handler(UART_HandleTypeDef *huart);
void BSP_UART_Get_Flag(UART_HandleTypeDef *huart, uint32_t flag);
void BSP_UART_Clear_Flag(UART_HandleTypeDef *huart, uint32_t flag);







#endif /* __UART_H */
