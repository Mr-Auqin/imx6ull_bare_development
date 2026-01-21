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
void UART_Init(void);   




#endif /* __UART_H */




