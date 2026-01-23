#ifndef __UART_H
#define __UART_H

#include "imx6ul.h"



typedef struct
{

    



} UARTconfig_t;

/**
 * @brief UART driver API
 *
 * This module provides the UART driver API.
 */

/**
 * @brief Get a UART instance
 *
 * @param GPTNum The UART number
 * @return The UART instance
 */
UART_Type* UART_GetInstance(uint32_t GPTNum);





void BSP_UART_Init(UART_Type *UART, UARTconfig_t *UARTconfig);
void BSP_UART_Start(UART_Type *UART);
void BSP_UART_Stop(UART_Type *UART);

//查询的方式发送(阻塞)
//该函数将阻塞直到数据发送完成
//参数：
//      uart：UART实例
//      data：要发送的数据
//      len：要发送的数据的长度
void BSP_UART_Transmit(UART_Type *uart, uint8_t *data, uint32_t len);
void BSP_UART_Receive(UART_Type *UART, uint8_t *data, uint32_t len);

//中断的方式发送(非阻塞)
// 该函数将在中断服务函数中实现数据的发送
//  参数：
//      uart：UART实例
//      data：要发送的数据
//      len：要发送的数据的长度
void BSP_UART_Transmit_IT(UART_Type *uart, uint8_t *data, uint32_t len);
void BSP_UART_Receive_IT(UART_Type *UART, uint8_t *data, uint32_t len);

//使用DMA的方式发送数据，UART使用DMA实现数据的发送
//参数：
//      uart：UART实例
//      data：要发送的数据
//      len：要发送的数据的长度
//该函数将在DMA中断服务函数中实现数据的发送，不会阻塞调用线程。
//在调用这个函数时，UART将通知DMA实现数据的发送，直到数据发送完成,通过中断通知应用程序,串口发送完成
//在这期间，该函数将不会阻塞调用线程，可以继续执行其他任务
void BSP_UART_Transmit_DMA(UART_Type *uart, uint8_t *data, uint32_t len);
void BSP_UART_Receive_DMA(UART_Type *UART, uint8_t *data, uint32_t len);







#endif /* __UART_H */




