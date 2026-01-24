#ifndef __UART_H
#define __UART_H

#include "imx6ul.h"

typedef struct
{
    uint32_t UART_UCR1_Status; // the UART Status:0-Disable the UART,1-Enable the UART
    uint32_t UART_UCR2_IRTS;   // Ignore RTS Pin: 1-Ignore the RTS pin
    uint32_t UART_UCR2_PREN;   // Parity Enable: 0 Disable parity generator and checker
    uint32_t UART_UCR2_PROE;   // Parity Odd Even: 0 Even parity  , 1 Odd parity
    uint32_t UART_UCR2_STPB;   // Stop Bit Length: 0-1bit, 1-2bits
    uint32_t UART_UCR2_WS;     // Word Size: 0-7bits, 1-8bits
    uint32_t UART_UCR2_TXEN;   // Transmitter Enable: 0-Disable the transmitter, 1-Enable the transmitter
    uint32_t UART_UCR2_RXEN;   // Receiver Enable: 0-Disable the receiver, 1-Enable the receiver
    uint32_t UART_UCR2_SRST;   // Software Reset: 1-Do not reset the UART, 0-Reset the UART

} UARTconfig_t;

typedef struct
{

    uint32_t UART_UCR1_TRDYEN;  //the transmitter ready interrupt
    uint32_t UART_UCR1_TXMPTYEN; //the transmitter FIFO empty interrupt
    uint32_t UART_UCR1_RRDYEN;  //the receiver ready interrupt
    uint32_t UART_UCR1_IDEN;    //the IDLE interrupt
    uint32_t UART_UCR1_ICD;    //the Idle Condition Detect




} UART_IT_config_t;


/**
 * @brief Get a UART instance
 *
 * This function returns a UART instance based on the given UART number
 *
 * @param GPTNum The UART number
 * @return The UART instance
 */
UART_Type* UART_GetInstance(uint32_t GPTNum);


/**
 * @brief Initialize a UART instance
 *
 * This function initializes a UART instance based on the given configuration
 *
 * @param uart The UART instance to be initialized
 * @param uartconfig The UART configuration
 */
void BSP_UART_Init(UART_Type *uart, UARTconfig_t *uartconfig);
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










#endif /* __UART_H */




