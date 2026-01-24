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

//��ѯ�ķ�ʽ����(����)
//�ú���������ֱ�����ݷ������
//������
//      uart��UARTʵ��
//      data��Ҫ���͵�����
//      len��Ҫ���͵����ݵĳ���
void BSP_UART_Transmit(UART_Type *uart, uint8_t *data, uint32_t len);
void BSP_UART_Receive(UART_Type *UART, uint8_t *data, uint32_t len);

//�жϵķ�ʽ����(������)
// �ú��������жϷ�������ʵ�����ݵķ���
//  ������
//      uart��UARTʵ��
//      data��Ҫ���͵�����
//      len��Ҫ���͵����ݵĳ���
void BSP_UART_Transmit_IT(UART_Type *uart, uint8_t *data, uint32_t len);
void BSP_UART_Receive_IT(UART_Type *UART, uint8_t *data, uint32_t len);

//ʹ��DMA�ķ�ʽ�������ݣ�UARTʹ��DMAʵ�����ݵķ���
//������
//      uart��UARTʵ��
//      data��Ҫ���͵�����
//      len��Ҫ���͵����ݵĳ���
//�ú�������DMA�жϷ�������ʵ�����ݵķ��ͣ��������������̡߳�
//�ڵ����������ʱ��UART��֪ͨDMAʵ�����ݵķ��ͣ�ֱ�����ݷ������,ͨ���ж�֪ͨӦ�ó���,���ڷ������
//�����ڼ䣬�ú������������������̣߳����Լ���ִ����������
void BSP_UART_Transmit_DMA(UART_Type *uart, uint8_t *data, uint32_t len);
void BSP_UART_Receive_DMA(UART_Type *UART, uint8_t *data, uint32_t len);







#endif /* __UART_H */




