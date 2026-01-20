#ifndef __DELAY_H
#define __DELAY_H

#include "imx6ul.h"







// High precision delay function declarations
void delay_Init(void);
void delay_us(volatile unsigned int n);
void delay_ms(volatile unsigned int n);

// Short delay function declaration 
void delay_short(volatile unsigned int n);
void delay(volatile unsigned int n);

#endif /* __DELAY_H */
