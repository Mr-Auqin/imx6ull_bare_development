#include "uart.h"


uint32_t   UART_base_add[] = UART_BASE_ADDRS;
UART_Type* UART_base_ptrs[] = UART_BASE_PTRS;
uint32_t   UART_irqs[] = UART_IRQS;


UART_Type *UART_GetInstance(uint32_t UARTNum)
{
    return UART_base_ptrs[UARTNum];
}

void UART_Init(void)
{

    //UART_Config_t 功能配置结构体
    UART_Config_t UARTconfig;
    //UART_IT_Config_t  中断配置结构体
    UART_IT_Config_t itconfig;
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




    //UARTconfig 结构体清零
    memset(&UARTconfig, 0, sizeof(UART_Config_t));
    //配置结构体成员:具体串口配置
    UARTconfig.clocksource = UART_clksource_from_pll3_80M;
    UARTconfig.clockdivider = UART_clk_divider_1;
    UARTconfig.modeselect = UART_mode_DTE;
    UARTconfig.baudrate = 115200;
    UARTconfig.databits = UART_WORDLENGTH_8B;
    UARTconfig.parity = UART_PARITY_NONE;
    UARTconfig.stopbits = UART_STOPBITS_1;
    UARTconfig.senderEnable = UART_Sender_ENABLE;
    UARTconfig.receiverEnable = UART_Receiver_ENABLE;
    //初始化串口1
    BSP_UART_Init(uartptrs, &UARTconfig);

    //itconfig 结构体清零
    memset(&itconfig, 0, sizeof(UART_IT_Config_t));
    itconfig.txReadyInterruptEnable = 0;             // the transmitter ready interrupt
    itconfig.txEmptyInterruptEnable = 0;             // the transmitter FIFO empty interrupt
    itconfig.rxReadyInterruptEnable = 0;             // the receiver ready interrupt
    itconfig.ReceiveStatusInterruptEnable = 0;       // the Receive Status Interrupt
    itconfig.idleInterruptEnable = 0;                // the IDLE interrupt
    itconfig.idleConditionDetect = 0;                // the Idle Condition Detect
    itconfig.transmitterCompleteInterruptEnable = 0; // the Transmitter Complete interrupt
    itconfig.receiverOverrunInterruptEnable = 0;     // the Receiver Overrun  Error interrupt
    itconfig.receiverDataReadyInterruptEnable = 0;   // the Receiver Data Ready interrupt
    //初始化串口1中断
    BSP_UART_IT_Init(uartptrs, &itconfig);

    //使能串口
    BSP_UART_Enable(uartptrs);
}

void BSP_UART_Init(UART_Type *uart, UART_Config_t *uartconfig)
{
    __UART_Register_field_config_t field_config;
    
    //初始化栈中的局部变量结构体
    memset(&field_config, 0, sizeof(field_config));

    //配置串口时钟源,选择时钟源时钟分频为1
    SET_BIT(&CCM->CSCDR1, CCM_CSCDR1_UART_CLK_SEL_MASK, CCM_CSCDR1_UART_CLK_SEL(uartconfig->clocksource));
    SET_BIT(&CCM->CSCDR1, CCM_CSCDR1_UART_CLK_PODF_MASK, CCM_CSCDR1_UART_CLK_PODF(0));

    //复位串口
    field_config.__UART_UCR2_SRST = 0;      // Software Reset: 1-Do not reset the UART, 0-Reset the UART
    SET_BIT(&uart->UCR2, UART_UCR2_SRST_MASK, UART_UCR2_SRST(field_config.__UART_UCR2_SRST));
    while((uart->UCR2&UART_UCR2_SRST_MASK)==0);//等待复位完成

    //配置串口模组参考时钟分频,串口模式选择
    field_config.__UART_UFCR_RFDIV = uartconfig->clockdivider; // Reference Frequency Divider
    field_config.__UART_UFCR_DCEDTE = uartconfig->modeselect;  // DCE/DTE mode select:  0 DCE mode selected  1 DTE mode selected
    SET_BIT(&uart->UFCR, UART_UFCR_RFDIV_MASK, UART_UFCR_RFDIV(field_config.__UART_UFCR_RFDIV));
    SET_BIT(&uart->UFCR, UART_UFCR_DCEDTE_MASK, UART_UFCR_DCEDTE(field_config.__UART_UFCR_DCEDTE));


    //配置串口数据位
    field_config.__UART_UCR2_WS = uartconfig->databits;        // Word Size: 0-7bits, 1-8bits
    SET_BIT(&uart->UCR2, UART_UCR2_WS_MASK, UART_UCR2_WS(field_config.__UART_UCR2_WS));

    // 配置串口停止位
    field_config.__UART_UCR2_STPB = uartconfig->stopbits; // Stop Bit Length: 0-1bit, 1-2bits
    SET_BIT(&uart->UCR2, UART_UCR2_STPB_MASK, UART_UCR2_STPB(field_config.__UART_UCR2_STPB));

    //配置串口校验位
    if (uartconfig->parity == UART_PARITY_NONE)
    {
        field_config.__UART_UCR2_PREN = 0;     // Parity Enable: 0 Disable parity generator and checker
        field_config.__UART_UCR2_PROE = 0;     // Parity Odd Even: 0 Even parity  , 1 Odd parity
    }
    else if(uartconfig->parity == UART_PARITY_EVEN)
    {
        field_config.__UART_UCR2_PREN = 1;     // Parity Enable: 1 Enable parity generator and checker
        field_config.__UART_UCR2_PROE = 0;     // Parity Odd Even: 0 Even parity  , 1 Odd parity
    }
    else if(uartconfig->parity == UART_PARITY_ODD)
    {
        field_config.__UART_UCR2_PREN = 1;     // Parity Enable: 1 Enable parity generator and checker
        field_config.__UART_UCR2_PROE = 1;     // Parity Odd Even: 0 Even parity  , 1 Odd parity
    }

    SET_BIT(&uart->UCR2, UART_UCR2_PREN_MASK, UART_UCR2_PREN(field_config.__UART_UCR2_PREN));
    SET_BIT(&uart->UCR2, UART_UCR2_PROE_MASK, UART_UCR2_PROE(field_config.__UART_UCR2_PROE));
    
    //配置忽略RTS
    field_config.__UART_UCR2_IRTS = 1;      // Ignore RTS Pin: 1-Ignore the RTS pin
    SET_BIT(&uart->UCR2, UART_UCR2_IRTS_MASK, UART_UCR2_IRTS(field_config.__UART_UCR2_IRTS));

    //配置MUXED mode
    field_config.__UART_UCR3_RXDMUXSEL = 1; // In this chip, UARTs are used in MUXED mode, so that this bit should always be set.
    SET_BIT(&uart->UCR3, UART_UCR3_RXDMUXSEL_MASK, UART_UCR3_RXDMUXSEL(field_config.__UART_UCR3_RXDMUXSEL));

    //配置UART internal frequency divided by 1000
    field_config.__UART_ONEMS_ONEMS = 80000000/1000;    // must contain the value of the UART internal frequency divided by 1000
    SET_BIT(&uart->ONEMS, UART_ONEMS_ONEMS_MASK, UART_ONEMS_ONEMS(field_config.__UART_ONEMS_ONEMS));

    //配置波特率
    if(115200==uartconfig->baudrate)
    {
        field_config.__UART_UBIR_INC = 71;   // Incremental Counter
        field_config.__UART_UBMR_MOD = 3124; // Decremental Counter
    }
    SET_BIT(&uart->UBIR, UART_UBIR_INC_MASK, UART_UBIR_INC(field_config.__UART_UBIR_INC));
    SET_BIT(&uart->UBMR, UART_UBMR_MOD_MASK, UART_UBMR_MOD(field_config.__UART_UBMR_MOD));
    


    // 配置发送器和接收器:按照使能需求进行配置
    field_config.__UART_UCR2_TXEN = uartconfig->senderEnable;   // Transmitter Enable: 0-Disable the transmitter, 1-Enable the transmitter
    field_config.__UART_UCR2_RXEN = uartconfig->receiverEnable; // Receiver Enable: 0-Disable the receiver, 1-Enable the receiver
    SET_BIT(&uart->UCR2, UART_UCR2_TXEN_MASK, UART_UCR2_TXEN(field_config.__UART_UCR2_TXEN));
    SET_BIT(&uart->UCR2, UART_UCR2_RXEN_MASK, UART_UCR2_RXEN(field_config.__UART_UCR2_RXEN));

}

void BSP_UART_IT_Init(UART_Type *uart, UART_IT_Config_t *itconfig)
{
    __UART_Register_IT_config_t __itconfig;



    //使能GIC UART中断

    //注册中断处理函数
    
    //使能UART外设中断配置
    memset(&__itconfig,0,sizeof(__UART_Register_IT_config_t));


    __itconfig.UART_UCR1_TRDYEN = itconfig.txReadyInterruptEnable;             // the transmitter ready interrupt
    __itconfig.UART_UCR1_TXMPTYEN = itconfig.txEmptyInterruptEnable;             // the transmitter FIFO empty interrupt
    __itconfig.UART_UCR1_RRDYEN = itconfig.rxReadyInterruptEnable;             // the receiver ready interrupt
    itconfig.ReceiveStatusInterruptEnable;       // the Receive Status Interrupt
    __itconfig.UART_UCR1_IDEN = itconfig.idleInterruptEnable;                // the IDLE interrupt
    __itconfig.UART_UCR1_ICD = itconfig.idleConditionDetect;                // the Idle Condition Detect
    __itconfig.UART_UCR4_TCEN = itconfig.transmitterCompleteInterruptEnable; // the Transmitter Complete interrupt
    __itconfig.UART_UCR4_OREN = itconfig.receiverOverrunInterruptEnable;     // the Receiver Overrun  Error interrupt
    __itconfig.UART_UCR4_DREN = itconfig.receiverDataReadyInterruptEnable;   // the Receiver Data Ready interrupt

typedef struct
{



    __itconfig.UART_UCR3_RXDSEN;   // the Receiver IDLE interrupt Enable



    __itconfig.UART_UCR4_OREN;     // Receiver Overrun  Error Interrupt Enable

} __UART_Register_IT_config_t;


}

void BSP_UART_Enable(UART_Type *uart)
{
    // 配置串口:按照使能需求进行配置
    // the UART Status:0-Disable the UART,1-Enable the UART
    SET_BIT(&uart->UCR1, UART_UCR1_UARTEN_MASK, UART_UCR1_UARTEN(1));

}

void BSP_UART_Disable(UART_Type *uart)
{
    // 配置串口:按照使能需求进行配置
    // the UART Status:0-Disable the UART,1-Enable the UART
    SET_BIT(&uart->UCR1, UART_UCR1_UARTEN_MASK, UART_UCR1_UARTEN(0));
}
