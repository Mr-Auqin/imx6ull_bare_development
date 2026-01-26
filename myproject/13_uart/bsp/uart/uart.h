#ifndef __UART_H
#define __UART_H

#include "imx6ul.h"

/*UART_Config_Select*/
#define UART_clksource_ipg_clk 0
#define UART_clksource_ipg_clk_s 1
#define UART_clksource_ipg_clk_perclk 2

#define UART_clk_divider_6 0
#define UART_clk_divider_5 1
#define UART_clk_divider_4 2
#define UART_clk_divider_3 3
#define UART_clk_divider_2 4
#define UART_clk_divider_1 5
#define UART_clk_divider_7 6

#define UART_mode_DCE 0
#define UART_mode_DTE 1

#define UART_WORDLENGTH_7B 0
#define UART_WORDLENGTH_8B 1

#define UART_PARITY_NONE 0
#define UART_PARITY_EVEN 1
#define UART_PARITY_ODD 2

#define UART_STOPBITS_1 0
#define UART_STOPBITS_2 1

#define UART_Sender_ENABLE 1
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
// REGVALUE: 要修改的原始值
// Field_MASK: 要修改的位的掩码
// Field_VAL: 要设置的目标值
// 示例:SET_BIT(UART1->UCR1,UART_UCR1_UARTEN_MASK,UART_UCR1_UARTEN(1));
#define SET_BIT(REGVALUE, Field_MASK, Field_VAL) \
    (/* 核心逻辑：清0指定位 + 设置新值 */        \
     (((REGVALUE) & ~(Field_MASK)) | (Field_VAL)))





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
    uint32_t txReadyInterruptEnable;             // the transmitter ready interrupt
    uint32_t txEmptyInterruptEnable;             // the transmitter FIFO empty interrupt
    uint32_t rxReadyInterruptEnable;             // the receiver ready interrupt
    uint32_t ReceiveStatusInterruptEnable;       // the Receive Status Interrupt
    uint32_t idleInterruptEnable;                // the IDLE interrupt
    uint32_t idleConditionDetect;                // the Idle Condition Detect
    uint32_t transmitterCompleteInterruptEnable; // the Transmitter Complete interrupt
    uint32_t receiverOverrunInterruptEnable;     // the Receiver Overrun  Error interrupt
    uint32_t receiverDataReadyInterruptEnable;   // the Receiver Data Ready interrupt

} UART_IT_Config_t;

typedef struct
{

    uint32_t UART_UCR1_Status;    // the UART Status:0-Disable the UART,1-Enable the UART
    uint32_t UART_UCR2_IRTS;      // Ignore RTS Pin: 1-Ignore the RTS pin
    uint32_t UART_UCR2_PREN;      // Parity Enable: 0 Disable parity generator and checker
    uint32_t UART_UCR2_PROE;      // Parity Odd Even: 0 Even parity  , 1 Odd parity
    uint32_t UART_UCR2_STPB;      // Stop Bit Length: 0-1bit, 1-2bits
    uint32_t UART_UCR2_WS;        // Word Size: 0-7bits, 1-8bits
    uint32_t UART_UCR2_TXEN;      // Transmitter Enable: 0-Disable the transmitter, 1-Enable the transmitter
    uint32_t UART_UCR2_RXEN;      // Receiver Enable: 0-Disable the receiver, 1-Enable the receiver
    uint32_t UART_UCR2_SRST;      // Software Reset: 1-Do not reset the UART, 0-Reset the UART
    uint32_t UART_UCR3_RXDMUXSEL; // In this chip, UARTs are used in MUXED mode, so that this bit should always be set.
    uint32_t UART_UFCR_RFDIV;     // Reference Frequency Divider
    uint32_t UART_UFCR_DCEDTE;    // DCE/DTE mode select:  0 DCE mode selected  1 DTE mode selected
    uint32_t UART_UBIR_INC;       // Incremental Counter
    uint32_t UART_UBMR_MOD;       // Decremental Counter
    uint32_t UART_ONEMS_ONEMS;    // must contain the value of the UART internal frequency divided by 1000

} __UART_Register_field_config_t;

typedef struct
{
    uint32_t UART_UCR1_RRDYEN;   // the receiver ready interrupt Enable
    uint32_t UART_UCR1_IDEN;     // the IDLE interrupt Enable
    uint32_t UART_UCR4_DREN;     // Receiver Data Ready Interrupt Enable
    uint32_t UART_UCR3_RXDSEN;   // the Receiver IDLE interrupt Enable
    uint32_t UART_UCR1_ICD;      // the Idle Condition Detect
    uint32_t UART_UCR1_TXMPTYEN; // the transmitter FIFO empty interrupt Enable
    uint32_t UART_UCR1_TRDYEN;   // the transmitter ready interrupt Enable
    uint32_t UART_UCR4_TCEN;     // the Transmitter Complete interrupt Enable
    uint32_t UART_UCR4_OREN;     // Receiver Overrun  Error Interrupt Enable

} __UART_Register_IT_config_t;

/**
 * @brief Get a UART instance
 *
 * This function returns a UART instance based on the given UART number
 *
 * @param GPTNum The UART number
 * @return The UART instance
 */
UART_Type *UART_GetInstance(uint32_t GPTNum);

/**
 * @brief Initialize a UART instance
 *
 * This function initializes a UART instance based on the given configuration
 *
 * @param uart The UART instance to be initialized
 * @param uartconfig The UART configuration
 */
void BSP_UART_Init(UART_Type *uart, UART_Config_t *uartconfig);
void BSP_UART_Start(UART_Type *UART);
void BSP_UART_Stop(UART_Type *UART);

/**
 * @brief Transmits data through a UART instance
 *
 * This function transmits the given data through the given UART instance
 *
 * @param uart The UART instance to be used for transmission
 * @param data The data to be transmitted
 * @param len The length of the data to be transmitted
 */
void BSP_UART_Transmit(UART_Type *uart, uint8_t *data, uint32_t len);

/**
 * @brief Receives data through a UART instance
 *
 * This function receives the given data through the given UART instance
 *
 * @param uart The UART instance to be used for reception
 * @param data The data to be received
 * @param len The length of the data to be received
 */
void BSP_UART_Receive(UART_Type *uart, uint8_t *data, uint32_t len);

/**
 * @brief Transmits data through a UART instance using interrupts
 *
 * This function transmits the given data through the given UART instance using interrupts
 *
 * @param uart The UART instance to be used for transmission
 * @param data The data to be transmitted
 * @param len The length of the data to be transmitted
 */
void BSP_UART_Transmit_IT(UART_Type *uart, uint8_t *data, uint32_t len);

/**
 * @brief Receives data through a UART instance using interrupts
 *
 * This function receives the given data through the given UART instance using interrupts
 *
 * @param uart The UART instance to be used for reception
 * @param data The data to be received
 * @param len The length of the data to be received
 */
void BSP_UART_Receive_IT(UART_Type *uart, uint8_t *data, uint32_t len);

/**
 * @brief Transmits data through a UART instance using DMA
 *
 * This function transmits the given data through the given UART instance using DMA
 *
 * @param uart The UART instance to be used for transmission
 * @param data The data to be transmitted
 * @param len The length of the data to be transmitted
 */
void BSP_UART_Transmit_DMA(UART_Type *uart, uint8_t *data, uint32_t len);

/**
 * @brief Receives data through a UART instance using DMA
 *
 * This function receives the given data through the given UART instance using DMA
 *
 * @param uart The UART instance to be used for reception
 * @param data The data to be received
 * @param len The length of the data to be received
 */
void BSP_UART_Receive_DMA(UART_Type *uart, uint8_t *data, uint32_t len);

void BSP_UART_Get_Flag(UART_Type *uart, uint32_t flag);
void BSP_UART_Clear_Flag(UART_Type *uart, uint32_t flag);
void BSP_UART_Clear_Flag(UART_Type *uart, uint32_t flag);

#endif /* __UART_H */
