#include "uart.h"
#include "bsp_int.h"


uint32_t   UART_base_add[] = UART_BASE_ADDRS;
UART_Type* UART_base_ptrs[] = UART_BASE_PTRS;
uint32_t   UART_irqs[] = UART_IRQS;

UART_HandleTypeDef huart1;

UART_Type *UART_GetInstance(uint32_t UARTNum)
{
    return UART_base_ptrs[UARTNum];
}

void UART_Init(void)
{

    //获取串口1外设的寄存器结构体指针
    UART_Type* uartptrs = UART_GetInstance(1);


    //设置串口IO复用引脚
	IOMUXC_SetPinMux(IOMUXC_UART1_TX_DATA_UART1_TX,0);			/* 复用为UART1_TX */
    IOMUXC_SetPinMux(IOMUXC_UART1_RX_DATA_UART1_RX,0);			/* 复用为UART1_RX */

    /*配置 UART1_TX_DATA 的 IO 属性为 UART 的相关属性
     *bit [16]:0 HYS 关闭
     *bit [15:14]: 00 100K 下拉
     *bit [13]: 0 kepper 功能
     *bit [12]: 0 pull/keeper 功能  enable
     *bit [11]: 0 关闭开路输出
     *bit [7:6]: 10 速度 100Mhz
     *bit [5:3]: 110 R0/6 驱动能力
     *bit [0]: 0 低转换率
     */
    IOMUXC_SetPinConfig(IOMUXC_UART1_TX_DATA_UART1_TX, IOMUXC_SW_PAD_CTL_PAD_HYS(0)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_PUS(0)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_PUE(0)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_PKE(1)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_ODE(0)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_SPEED(2)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_DSE(6)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_SRE(0));

    /*配置 UART1_RX_DATA 的 IO 属性为 UART 的相关属性
     *bit [16]:0 HYS 关闭
     *bit [15:14]: 00 100K 下拉
     *bit [13]: 0 kepper 功能
     *bit [12]: 0 pull/keeper 功能  enable
     *bit [11]: 0 关闭开路输出
     *bit [7:6]: 10 速度 100Mhz
     *bit [5:3]: 110 R0/6 驱动能力
     *bit [0]: 0 低转换率
     */
    IOMUXC_SetPinConfig(IOMUXC_UART1_RX_DATA_UART1_RX, IOMUXC_SW_PAD_CTL_PAD_HYS(0)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_PUS(0)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_PUE(0)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_PKE(1)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_ODE(0)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_SPEED(2)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_DSE(6)|\
                                                      IOMUXC_SW_PAD_CTL_PAD_SRE(0));

    //配置结构体成员:具体串口配置
    huart1.Instance = uartptrs;
    huart1.Config.clocksource = UART_clksource_from_pll3_80M;
    huart1.Config.clockdivider = UART_clk_divider_1;
    huart1.Config.modeselect = UART_mode_DTE;
    huart1.Config.baudrate = 115200;
    huart1.Config.databits = UART_WORDLENGTH_8B;
    huart1.Config.parity = UART_PARITY_NONE;
    huart1.Config.stopbits = UART_STOPBITS_1;
    huart1.Config.senderEnable = UART_Sender_ENABLE;
    huart1.Config.receiverEnable = UART_Receiver_ENABLE;
    //初始化串口1
    BSP_UART_Init(&huart1);


    huart1.IT_Config.IT_receiver_ready = INTERRUPT_DISABLE;         // the receiver ready interrupt Enable(达到RxFIFO 设置的 接收数据数量门槛 则触发串口中断)
    huart1.IT_Config.IT_IDLE = INTERRUPT_DISABLE;                   // the IDLE interrupt Enable(RxFIFO数据为空+RX_DATA引脚达到由软件编程的空闲帧的数量则触发串口中断)
    huart1.IT_Config.IT_Receiver_Data_Ready = INTERRUPT_DISABLE;    // Receiver Data Ready Interrupt Enable(至少接收到一个字节数据，则触发串口中断,此状态位满足无数据就绪可读时,自动清零,可用于轮询读取)
    huart1.IT_Config.IT_Receiver_IDLE = INTERRUPT_DISABLE;          // the Receiver IDLE interrupt Enable(反应串口接收器的“工作状态”（空闲/接收中）,接收器空闲时,触发串口中断,推荐使用IDLE条件中断,不建议使用此中断)
    huart1.IT_Config.IT_Idle_Condition = INTERRUPT_DISABLE;         // the Idle Condition Detect(配置空闲的检测条件,可设置为4/8/16/32帧空闲)
    huart1.IT_Config.IT_transmitter_FIFO_empty = INTERRUPT_DISABLE; // the transmitter FIFO empty interrupt Enable(发送FIFO为空时，触发串口中断,当有数据写进去TxFIFO时，自动清零，也可用于轮询发送)
    huart1.IT_Config.IT_transmitter_ready = INTERRUPT_DISABLE;      // the transmitter ready interrupt Enable(达到TxFIFO 设置的 低于其现有的数据数量门槛时， 则触发串口中断，即可以往TxFIFO中写入数据时，触发串口中断，当TxFIFO中数据数量超过其设置值时，自动清零，可用于轮询发送)
    huart1.IT_Config.IT_Transmitter_Complete = INTERRUPT_DISABLE;   // the Transmitter Complete interrupt Enable(发送完成时，触发串口中断，可用于轮询发送，此状态位满足有数据写进去TxFIFO时，自动清零，可用于轮询读取)
    huart1.IT_Config.IT_Receiver_Overrun = INTERRUPT_DISABLE;       // Receiver Overrun  Error Interrupt Enable(当接收器发生溢出错误时，触发串口中断)
    huart1.IT_Config.irqHandler = uart1_irq_handler;;                 // 绑定串口中断处理函数

    //初始化串口1中断
    BSP_UART_IT_Init(&huart1);

    //使能串口
    BSP_UART_Enable(&huart1);
}

void BSP_UART_Init(UART_HandleTypeDef *huart)
{
    __UART_Register_field_config_t field_config;
    
    //初始化栈中的局部变量结构体
    memset(&field_config, 0, sizeof(field_config));

    //配置串口时钟源,选择时钟源时钟分频为1
    SET_BIT(&CCM->CSCDR1, CCM_CSCDR1_UART_CLK_SEL_MASK, CCM_CSCDR1_UART_CLK_SEL(huart->Config.clocksource));
    SET_BIT(&CCM->CSCDR1, CCM_CSCDR1_UART_CLK_PODF_MASK, CCM_CSCDR1_UART_CLK_PODF(0));

    //复位串口
    field_config.__UART_UCR2_SRST = 0;      // Software Reset: 1-Do not reset the UART, 0-Reset the UART
    SET_BIT(&huart->Instance->UCR2, UART_UCR2_SRST_MASK, UART_UCR2_SRST(field_config.__UART_UCR2_SRST));
    while((huart->Instance->UCR2&UART_UCR2_SRST_MASK)==0);//等待复位完成

    //配置串口模组参考时钟分频,串口模式选择
    field_config.__UART_UFCR_RFDIV = huart->Config.clockdivider; // Reference Frequency Divider
    field_config.__UART_UFCR_DCEDTE = huart->Config.modeselect;  // DCE/DTE mode select:  0 DCE mode selected  1 DTE mode selected
    SET_BIT(&huart->Instance->UFCR, UART_UFCR_RFDIV_MASK, UART_UFCR_RFDIV(field_config.__UART_UFCR_RFDIV));
    SET_BIT(&huart->Instance->UFCR, UART_UFCR_DCEDTE_MASK, UART_UFCR_DCEDTE(field_config.__UART_UFCR_DCEDTE));


    //配置串口数据位
    field_config.__UART_UCR2_WS = huart->Config.databits;        // Word Size: 0-7bits, 1-8bits
    SET_BIT(&huart->Instance->UCR2, UART_UCR2_WS_MASK, UART_UCR2_WS(field_config.__UART_UCR2_WS));

    // 配置串口停止位
    field_config.__UART_UCR2_STPB = huart->Config.stopbits; // Stop Bit Length: 0-1bit, 1-2bits
    SET_BIT(&huart->Instance->UCR2, UART_UCR2_STPB_MASK, UART_UCR2_STPB(field_config.__UART_UCR2_STPB));

    //配置串口校验位
    if (huart->Config.parity == UART_PARITY_NONE)
    {
        field_config.__UART_UCR2_PREN = 0;     // Parity Enable: 0 Disable parity generator and checker
        field_config.__UART_UCR2_PROE = 0;     // Parity Odd Even: 0 Even parity  , 1 Odd parity
    }
    else if(huart->Config.parity == UART_PARITY_EVEN)
    {
        field_config.__UART_UCR2_PREN = 1;     // Parity Enable: 1 Enable parity generator and checker
        field_config.__UART_UCR2_PROE = 0;     // Parity Odd Even: 0 Even parity  , 1 Odd parity
    }
    else if(huart->Config.parity == UART_PARITY_ODD)
    {
        field_config.__UART_UCR2_PREN = 1;     // Parity Enable: 1 Enable parity generator and checker
        field_config.__UART_UCR2_PROE = 1;     // Parity Odd Even: 0 Even parity  , 1 Odd parity
    }

    SET_BIT(&huart->Instance->UCR2, UART_UCR2_PREN_MASK, UART_UCR2_PREN(field_config.__UART_UCR2_PREN));
    SET_BIT(&huart->Instance->UCR2, UART_UCR2_PROE_MASK, UART_UCR2_PROE(field_config.__UART_UCR2_PROE));
    
    //配置忽略RTS
    field_config.__UART_UCR2_IRTS = 1;      // Ignore RTS Pin: 1-Ignore the RTS pin
    SET_BIT(&huart->Instance->UCR2, UART_UCR2_IRTS_MASK, UART_UCR2_IRTS(field_config.__UART_UCR2_IRTS));

    //配置MUXED mode
    field_config.__UART_UCR3_RXDMUXSEL = 1; // In this chip, UARTs are used in MUXED mode, so that this bit should always be set.
    SET_BIT(&huart->Instance->UCR3, UART_UCR3_RXDMUXSEL_MASK, UART_UCR3_RXDMUXSEL(field_config.__UART_UCR3_RXDMUXSEL));

    //配置UART internal frequency divided by 1000
    field_config.__UART_ONEMS_ONEMS = 80000000/1000;    // must contain the value of the UART internal frequency divided by 1000
    SET_BIT(&huart->Instance->ONEMS, UART_ONEMS_ONEMS_MASK, UART_ONEMS_ONEMS(field_config.__UART_ONEMS_ONEMS));

    //配置波特率
    if(115200==huart->Config.baudrate)
    {
        field_config.__UART_UBIR_INC = 71;   // Incremental Counter
        field_config.__UART_UBMR_MOD = 3124; // Decremental Counter
    }
    SET_BIT(&huart->Instance->UBIR, UART_UBIR_INC_MASK, UART_UBIR_INC(field_config.__UART_UBIR_INC));
    SET_BIT(&huart->Instance->UBMR, UART_UBMR_MOD_MASK, UART_UBMR_MOD(field_config.__UART_UBMR_MOD));
    


    // 配置发送器和接收器:按照使能需求进行配置
    field_config.__UART_UCR2_TXEN = huart->Config.senderEnable;   // Transmitter Enable: 0-Disable the transmitter, 1-Enable the transmitter
    field_config.__UART_UCR2_RXEN = huart->Config.receiverEnable; // Receiver Enable: 0-Disable the receiver, 1-Enable the receiver
    SET_BIT(&huart->Instance->UCR2, UART_UCR2_TXEN_MASK, UART_UCR2_TXEN(field_config.__UART_UCR2_TXEN));
    SET_BIT(&huart->Instance->UCR2, UART_UCR2_RXEN_MASK, UART_UCR2_RXEN(field_config.__UART_UCR2_RXEN));

    // 配置串口使能
    field_config.__UART_UCR1_UARTEN = 1;    // the UART UARTEN:0-Disable the UART,1-Enable the UART
    SET_BIT(&huart->Instance->UCR1, UART_UCR1_UARTEN_MASK, UART_UCR1_UARTEN(field_config.__UART_UCR1_UARTEN));

}


void BSP_UART_IT_Init(UART_HandleTypeDef *huart)
{
    __UART_Register_IT_config_t __itconfig;
    uint32_t Num,loop = 0;

    
    //寻找UART指针对应的串口标号
    for(loop = 0; loop < 9; loop++)
    {
        if(huart->Instance == UART_base_ptrs[loop])
        {
            Num = loop;
            break;
        }
    } 

    //使能GIC中对应的 UART中断
    GIC_EnableIRQ(UART_irqs[Num]);
    //注册UART 中断处理函数
    system_register_irqhandler(UART_irqs[Num], (system_irq_handler_t)huart->IT_Config.irqHandler, NULL);
    
    //使能UART外设中断配置
    memset(&__itconfig,0,sizeof(__UART_Register_IT_config_t));

    __itconfig.UART_UCR1_RRDYEN   = huart->IT_Config.IT_receiver_ready;             // the receiver ready interrupt Enable(达到RxFIFO 设置的 接收数据数量门槛 则触发串口中断)
    __itconfig.UART_UCR1_IDEN     = huart->IT_Config.IT_IDLE;                       // the IDLE interrupt Enable(RxFIFO数据为空+RX_DATA引脚达到由软件编程的空闲帧的数量则触发串口中断)
    __itconfig.UART_UCR4_DREN     = huart->IT_Config.IT_Receiver_Data_Ready;        // Receiver Data Ready Interrupt Enable(至少接收到一个字节数据，则触发串口中断,此状态位满足无数据就绪可读时,自动清零,可用于轮询读取)
    __itconfig.UART_UCR3_RXDSEN   = huart->IT_Config.IT_Receiver_IDLE;              // the Receiver IDLE interrupt Enable(反应串口接收器的“工作状态”（空闲/接收中）,接收器空闲时,触发串口中断,推荐使用IDLE条件中断,不建议使用此中断)
    __itconfig.UART_UCR1_ICD      = huart->IT_Config.IT_Idle_Condition;             // the Idle Condition Detect(配置空闲的检测条件,可设置为4/8/16/32帧空闲)
    __itconfig.UART_UCR1_TXMPTYEN = huart->IT_Config.IT_transmitter_FIFO_empty;     // the transmitter FIFO empty interrupt Enable(发送FIFO为空时，触发串口中断,当有数据写进去TxFIFO时，自动清零，也可用于轮询发送)
    __itconfig.UART_UCR1_TRDYEN   = huart->IT_Config.IT_transmitter_ready;          // the transmitter ready interrupt Enable(达到TxFIFO 设置的 低于其现有的数据数量门槛时， 则触发串口中断，即可以往TxFIFO中写入数据时，触发串口中断，当TxFIFO中数据数量超过其设置值时，自动清零，可用于轮询发送)
    __itconfig.UART_UCR4_TCEN     = huart->IT_Config.IT_Transmitter_Complete;       // the Transmitter Complete interrupt Enable(发送完成时，触发串口中断，可用于轮询发送，此状态位满足有数据写进去TxFIFO时，自动清零，可用于轮询读取)
    __itconfig.UART_UCR4_OREN     = huart->IT_Config.IT_Receiver_Overrun;           // Receiver Overrun  Error Interrupt Enable(当接收器发生溢出错误时，触发串口中断)


    SET_BIT(&huart->Instance->UCR1, UART_UCR1_RRDYEN_MASK, UART_UCR1_RRDYEN(__itconfig.UART_UCR1_RRDYEN));
    SET_BIT(&huart->Instance->UCR1, UART_UCR1_IDEN_MASK, UART_UCR1_IDEN(__itconfig.UART_UCR1_IDEN));
    SET_BIT(&huart->Instance->UCR4, UART_UCR4_DREN_MASK, UART_UCR4_DREN(__itconfig.UART_UCR4_DREN));
    SET_BIT(&huart->Instance->UCR3, UART_UCR3_RXDSEN_MASK, UART_UCR3_RXDSEN(__itconfig.UART_UCR3_RXDSEN));
    SET_BIT(&huart->Instance->UCR1, UART_UCR1_ICD_MASK, UART_UCR1_ICD(__itconfig.UART_UCR1_ICD));
    SET_BIT(&huart->Instance->UCR1, UART_UCR1_TXMPTYEN_MASK, UART_UCR1_TXMPTYEN(__itconfig.UART_UCR1_TXMPTYEN));
    SET_BIT(&huart->Instance->UCR1, UART_UCR1_TRDYEN_MASK, UART_UCR1_TRDYEN(__itconfig.UART_UCR1_TRDYEN));
    SET_BIT(&huart->Instance->UCR4, UART_UCR4_TCEN_MASK, UART_UCR4_TCEN(__itconfig.UART_UCR4_TCEN));
    SET_BIT(&huart->Instance->UCR4, UART_UCR4_OREN_MASK, UART_UCR4_OREN(__itconfig.UART_UCR4_OREN));

}
    
void BSP_UART_Enable(UART_HandleTypeDef *huart)
{
    // 配置串口:按照使能需求进行配置
    // the UART Status:0-Disable the UART,1-Enable the UART
    SET_BIT(&huart->Instance->UCR1, UART_UCR1_UARTEN_MASK, UART_UCR1_UARTEN(1));

}

void BSP_UART_Disable(UART_HandleTypeDef *huart)
{
    // 配置串口:按照使能需求进行配置
    // the UART Status:0-Disable the UART,1-Enable the UART
    SET_BIT(&huart->Instance->UCR1, UART_UCR1_UARTEN_MASK, UART_UCR1_UARTEN(0));
}

void BSP_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *data, uint32_t len)
{
    
}

void BSP_UART_Receive(UART_HandleTypeDef *huart, uint8_t *data, uint32_t len)
{
}

void BSP_UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *data, uint32_t len)
{
}

void BSP_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *data, uint32_t len)
{
}

void BSP_UART_Transmit_DMA(UART_HandleTypeDef *huart, uint8_t *data, uint32_t len)
{
}

void BSP_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *data, uint32_t len)
{
}

void BSP_UART_Get_Flag(UART_HandleTypeDef *huart, uint32_t flag)
{



}

void BSP_UART_Clear_Flag(UART_HandleTypeDef *huart, uint32_t flag)
{



}



void uart1_irq_handler(void)
{
    BSP_UART_IRQ_Handler(&huart1);
}








