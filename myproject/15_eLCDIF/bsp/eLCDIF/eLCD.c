#include "eLCD.h"



//MPU and VSYNC mode 是用于单帧传输的,DOTCLK and DVI mode 用于连续帧传输
//当一帧刷新完成时,会将LCDIF_NEXT_BUF_ADDR的值 更新到LCDIF_CUR_BUF_ADDR中去
//诺LCDIF_NEXT_BUF_ADDR的值在没有在帧刷新周期内被更改,则eLCDIF会继续保持传输最后一帧的内容 直到新值被编程到LCDIF_NEXT_BUF_ADDR寄存器中去





//The LCDIF_MASTER bit must be set to 1 to enable the bus master interface




void eLCDIF_Init(void) {

    uint32_t ELCD_PAD_VALUE = 0;

    // 1、Configure the external I/Os to correctly interface the external display
    // 设置eLCDIF 外部IO 口 复用到eLCDIF功能:LCD_PCLK、LCD_VSYNC、LCD_HSYNC、LCD_DE、LCD_DATA0~23
    // 设置eLCDIF 外部IO 口 复用到eLCDIF功能:LCD_PCLK、LCD_VSYNC、LCD_HSYNC、LCD_DE
    IOMUXC_SetPinMux(IOMUXC_LCD_CLK_LCDIF_CLK, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_VSYNC_LCDIF_VSYNC, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_HSYNC_LCDIF_HSYNC, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_ENABLE_LCDIF_ENABLE, 0);
    // 设置eLCDIF 外部IO 口 复用到eLCDIF功能:LCD_DATA0~23
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA00_LCDIF_DATA00, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA01_LCDIF_DATA01, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA02_LCDIF_DATA02, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA03_LCDIF_DATA03, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA04_LCDIF_DATA04, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA05_LCDIF_DATA05, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA06_LCDIF_DATA06, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA07_LCDIF_DATA07, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA08_LCDIF_DATA08, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA09_LCDIF_DATA09, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA10_LCDIF_DATA10, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA11_LCDIF_DATA11, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA12_LCDIF_DATA12, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA13_LCDIF_DATA13, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA14_LCDIF_DATA14, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA15_LCDIF_DATA15, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA16_LCDIF_DATA16, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA17_LCDIF_DATA17, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA18_LCDIF_DATA18, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA19_LCDIF_DATA19, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA20_LCDIF_DATA20, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA21_LCDIF_DATA21, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA22_LCDIF_DATA22, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA23_LCDIF_DATA23, 0);


     /*配置 ELCD 的 IO 属性为 复用成为ELCDIF 的相关属性
     *bit [16]:0 HYS 关闭
     *bit [15:14]: 00 100K 下拉
     *bit [13]: 0 kepper 功能
     *bit [12]: 0 pull/keeper 功能  disable
     *bit [11]: 0 关闭开路输出
     *bit [7:6]: 10 速度 100Mhz
     *bit [5:3]: 110 R0/6 驱动能力
     *bit [0]: 0 低转换率
     */

    ELCD_PAD_VALUE = IOMUXC_SW_PAD_CTL_PAD_HYS(0)|\
                     IOMUXC_SW_PAD_CTL_PAD_PUS(0)|\
                     IOMUXC_SW_PAD_CTL_PAD_PUE(0)|\
                     IOMUXC_SW_PAD_CTL_PAD_PKE(0)|\
                     IOMUXC_SW_PAD_CTL_PAD_ODE(0)|\
                     IOMUXC_SW_PAD_CTL_PAD_SPEED(2)|\
                     IOMUXC_SW_PAD_CTL_PAD_DSE(6)|\
                     IOMUXC_SW_PAD_CTL_PAD_SRE(0);
        
    //配置eLCDIF 外部IO pad 的属性:LCD_PCLK、LCD_VSYNC、LCD_HSYNC、LCD_DE
    IOMUXC_SetPinConfig(IOMUXC_LCD_CLK_LCDIF_CLK, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_VSYNC_LCDIF_VSYNC, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_HSYNC_LCDIF_HSYNC, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_ENABLE_LCDIF_ENABLE, ELCD_PAD_VALUE);
    //配置eLCDIF 外部IO pad 的属性:LCD_DATA0~23
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA00_LCDIF_DATA00, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA01_LCDIF_DATA01, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA02_LCDIF_DATA02, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA03_LCDIF_DATA03, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA04_LCDIF_DATA04, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA05_LCDIF_DATA05, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA06_LCDIF_DATA06, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA07_LCDIF_DATA07, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA08_LCDIF_DATA08, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA09_LCDIF_DATA09, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA10_LCDIF_DATA10, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA11_LCDIF_DATA11, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA12_LCDIF_DATA12, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA13_LCDIF_DATA13, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA14_LCDIF_DATA14, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA15_LCDIF_DATA15, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA16_LCDIF_DATA16, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA17_LCDIF_DATA17, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA18_LCDIF_DATA18, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA19_LCDIF_DATA19, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA20_LCDIF_DATA20, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA21_LCDIF_DATA21, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA22_LCDIF_DATA22, ELCD_PAD_VALUE);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA23_LCDIF_DATA23, ELCD_PAD_VALUE);


    //2. Start the DISPLAY CLOCK (pix_clk) clock and set the appropriate frequency by programming the registers in CCM.
    
    /*------------------------
    -----------ATK7016--------------
    ---------水平显示区域 1024 tCLK-
    ---------HSPW(thp) 20 tCLK------
    ---------HBP(thb) 140 tCLK------
    ---------HFP(thf) 160 tCLK------
    ---------垂直显示区域 600 th----
    ---------VSPW(tvp) 3 th---------
    ---------VBP(tvb) 20 th---------
    ---------VFP(tvf) 12 th---------
    ---------像素时钟 51.2 MHz------*/
    //显示一帧图像需要的时钟为: (VSPW+VBP+LINE+VFP) * (HSPW + HBP + HOZVAL + HFP) =  (3 + 20 + 600 + 12) * (20 + 140 + 1024 + 160) = 853440
    //显示60帧图像需要的时钟为: 853440 * 60 = 51.2MHz,所以目标 pix_clk = 51.2MHz
    //首先配置LCDIF1_CLK_ROOT的时钟为PLL5  pix_clk from LCDIF1_CLK_ROOT 因此将LCDIF1_CLK_ROOT的时钟配置为51.2MHz
    //LCDIF1_CLK_ROOT = PLL5/PRED/PODF = 768M/3/5 = 51.2MHZ
    SET_BIT(CCM->CSCDR2, CCM_CSCDR2_LCDIF1_PRE_CLK_SEL_MASK, CCM_CSCDR2_LCDIF1_PRE_CLK_SEL(2));//010 derive clock from PLL5
    SET_BIT(CCM->CSCDR2, CCM_CSCDR2_LCDIF1_PRED_MASK, CCM_CSCDR2_LCDIF1_PRED(3-1));//Pre-divider for lcdif1 clock. 000 divide by 1
    SET_BIT(CCM->CBCMR,  CCM_CBCMR_LCDIF1_PODF_MASK, CCM_CBCMR_LCDIF1_PODF(5-1));//Post-divider for LCDIF1 clock. 000 divide by 1
    SET_BIT(CCM->CSCDR2, CCM_CSCDR2_LCDIF1_CLK_SEL_MASK, CCM_CSCDR2_LCDIF1_CLK_SEL(0));//000 derive clock from divided pre-muxed LCDIF1 clock

    // 配置PLL5(Video PLL)的时钟(reference manual(IMX6ULL):18.5.1.3.4 Audio / Video PLL)
    // The output frequency can be set by programming the fields
    // in the CCM_ANALOG_PLL_AUDIO, CCM_ANALOG_PLL_VIDEO, and
    // CCM_ANALOG_MISC2 register sets according to the following equation
    // PLL output frequency = Fref * (DIV_SELECT + NUM/DENOM) 
    //                      = 24*(32+0)= 768MHz
    
    SET_BIT(CCM_ANALOG->PLL_VIDEO, CCM_ANALOG_PLL_VIDEO_POST_DIV_SELECT_MASK, CCM_ANALOG_PLL_VIDEO_POST_DIV_SELECT(2));//10 — Divide by 1
    /**DEBUG:上电默认是1:旁路Video PLL,我这里取消旁路**/
    SET_BIT(CCM_ANALOG->PLL_VIDEO, CCM_ANALOG_PLL_VIDEO_BYPASS_CLK_SRC_MASK, CCM_ANALOG_PLL_VIDEO_BYPASS_CLK_SRC(0));//0 — Don't Bypass the PLL
    SET_BIT(CCM_ANALOG->PLL_VIDEO, CCM_ANALOG_PLL_VIDEO_ENABLE_MASK, CCM_ANALOG_PLL_VIDEO_ENABLE(1));//1 — Enalbe PLL output
    /**DEBUG:上电默认是1:Powers down the PLL.**/
    SET_BIT(CCM_ANALOG->PLL_VIDEO, CCM_ANALOG_PLL_VIDEO_POWERDOWN_MASK, CCM_ANALOG_PLL_VIDEO_POWERDOWN(0));//0 — Don't Powers down the PLL.
    SET_BIT(CCM_ANALOG->PLL_VIDEO, CCM_ANALOG_PLL_VIDEO_DIV_SELECT_MASK, CCM_ANALOG_PLL_VIDEO_DIV_SELECT(32));// Valid range for DIV_SELECT divider value: 27~54.

    SET_BIT(CCM_ANALOG->MISC2, CCM_ANALOG_MISC2_VIDEO_DIV_MASK, CCM_ANALOG_MISC2_VIDEO_DIV(0));// 00 divide by 1 (Default)

    //配置NUM & DENOM  = NUM/DENOM = 0/1 = 0;
    SET_BIT(CCM_ANALOG->PLL_VIDEO_NUM, CCM_ANALOG_PLL_VIDEO_NUM_A_MASK, CCM_ANALOG_PLL_VIDEO_NUM_A(0));// Absolute value should be less than denominator
    SET_BIT(CCM_ANALOG->PLL_VIDEO_DENOM, CCM_ANALOG_PLL_VIDEO_DENOM_B_MASK, CCM_ANALOG_PLL_VIDEO_DENOM_B(1));// denominator

    //3. Start the BUS CLOCK (apb_clk) and set the appropriate frequency by programmingthe registers in CCM.
    //apb_clk from AXI_CLK_ROOT = 540MHz/3 = 180MHz  Tips: AXI_CLK_ROOT  Maximum Frequency (MHz) = 264MHz
    SET_BIT(CCM->CBCDR, CCM_CBCDR_AXI_ALT_SEL_MASK, CCM_CBCDR_AXI_ALT_SEL(1));//1 PLL3_PFD1(540MHz) will be selected as alternative clock for AXI root clock
    SET_BIT(CCM->CBCDR, CCM_CBCDR_AXI_SEL_MASK, CCM_CBCDR_AXI_SEL(1));//1 AXI alternative clock will be used as AXI clock root
    SET_BIT(CCM->CBCDR, CCM_CBCDR_AXI_PODF_MASK, CCM_CBCDR_AXI_PODF(3-1));//010 divide by 3

    //4. Bring the eLCDIF out of soft reset and disable the clock gate bit.
    SET_BIT(LCDIF->CTRL, LCDIF_CTRL_CLKGATE_MASK, LCDIF_CTRL_CLKGATE(0));//must be set to zero for normal operation disable the clock gate bit.
    SET_BIT(LCDIF->CTRL, LCDIF_CTRL_SFTRST_MASK, LCDIF_CTRL_SFTRST(0));// must be set to zero to enable normal operation When set to one, it forces a block level reset
 
    //5. Reset the LCD controller by setting LCDIF_CTRL1[RESET] bit appropriately
    //外设LCD控制器 没有复位信号 不设置

    //6. Make sure LCDIF_CTRL[READ_WRITEB] bit is 0.
    SET_BIT(LCDIF->CTRL, LCDIF_CTRL_READ_WRITEB_MASK, LCDIF_CTRL_READ_WRITEB(0));

    //7. Select the transfer mode of operation Set LCDIF_CTRL[MASTER] bit to 1
    SET_BIT(LCDIF->CTRL, LCDIF_CTRL_MASTER_MASK, LCDIF_CTRL_MASTER(1));//Set this bit to make the eLCDIF act as a bus master.

    // 8. Set the LCDIF_CTRL[INPUT_DATA_SWIZZLE] according to the endianness of
    // the LCD controller. Also, set the LCDIF_CTRL[DATA_SHIFT_DIR] and
    // LCDIF_CTRL[SHIFT_NUM_BITS] if it is required to shift the data left or right
    // before it is output.
    SET_BIT(LCDIF->CTRL, LCDIF_CTRL_INPUT_DATA_SWIZZLE_MASK, LCDIF_CTRL_INPUT_DATA_SWIZZLE(0));//0x0 NO_SWAP — No byte swapping.(Little endian)
    //这里不需要将数据进行左右移动,因此就不设置LCDIF_CTRL[DATA_SHIFT_DIR] and LCDIF_CTRL[SHIFT_NUM_BITS]了。

    // 9. Set the LCDIF_CTRL[WORD_LENGTH] field appropriately: 0 = 16-bit input, 1 =
    // 8-bit input, 2 = 18-bit input, 3 = 24/32-bit input. Also, select the correct 16/18/24 bit
    // data format with the corresponding fields in LCDIF_CTRL register
    SET_BIT(LCDIF->CTRL, LCDIF_CTRL_WORD_LENGTH_MASK, LCDIF_CTRL_WORD_LENGTH(3));//0x3 24_BIT — Input data is 24 bits per pixel.
    SET_BIT(LCDIF->CTRL, LCDIF_CTRL_DATA_FORMAT_24_BIT_MASK, LCDIF_CTRL_DATA_FORMAT_24_BIT(0));//Data input to the block is in 24 bpp format

    // 10. Set the LCDIF_CTRL1[BYTE_PACKING_FORMAT] field according to the input frame.
    SET_BIT(LCDIF->CTRL1, LCDIF_CTRL1_BYTE_PACKING_FORMAT_MASK, LCDIF_CTRL1_BYTE_PACKING_FORMAT(0x7));//set the bit field value to 0x7 if the display data is arranged in the 24-bit unpacked format

    // 11. Set the LCDIF_CTRL[LCD_DATABUS_WIDTH] appropriately: 0 = 16-bit output,1 = 8-bit output, 2 = 18-bit output, 3 = 24/32-bit output.
    SET_BIT(LCDIF->CTRL, LCDIF_CTRL_LCD_DATABUS_WIDTH_MASK, LCDIF_CTRL_LCD_DATABUS_WIDTH(3));//0x3 24_BIT — 24-bit data bus mode

    //12. Enable the necessary IRQs.
    //这里暂时没有中断需要使能,如果有对应的需要测试使用的中断,后面在此编写测试


    //初始化为DOTCLK Interface 模式
    SET_BIT(LCDIF->CTRL, LCDIF_CTRL_DOTCLK_MODE_MASK, LCDIF_CTRL_DOTCLK_MODE(1));//Set this bit to 1 to make the hardware go into the DOTCLK mode
    SET_BIT(LCDIF->CTRL, LCDIF_CTRL_BYPASS_COUNT_MASK, LCDIF_CTRL_BYPASS_COUNT(1));//This bit must be 1 in DOTCLK and DVI modes of operation.
    SET_BIT(LCDIF->VDCTRL0, LCDIF_VDCTRL0_VSYNC_OEB_MASK, LCDIF_VDCTRL0_VSYNC_OEB(0));//0 Vsync is always an output in the DOTCLK mode
    SET_BIT(LCDIF->VDCTRL0, LCDIF_VDCTRL0_VSYNC_POL_MASK, LCDIF_VDCTRL0_VSYNC_POL(0));
    SET_BIT(LCDIF->VDCTRL0, LCDIF_VDCTRL0_HSYNC_POL_MASK, LCDIF_VDCTRL0_HSYNC_POL(0));
    SET_BIT(LCDIF->VDCTRL0, LCDIF_VDCTRL0_DOTCLK_POL_MASK, LCDIF_VDCTRL0_DOTCLK_POL(0));
    SET_BIT(LCDIF->VDCTRL0, LCDIF_VDCTRL0_ENABLE_POL_MASK, LCDIF_VDCTRL0_ENABLE_POL(0));
    SET_BIT(LCDIF->VDCTRL0, LCDIF_VDCTRL0_ENABLE_PRESENT_MASK, LCDIF_VDCTRL0_ENABLE_PRESENT(1));
    SET_BIT(LCDIF->VDCTRL0, LCDIF_VDCTRL0_VSYNC_PERIOD_UNIT_MASK, LCDIF_VDCTRL0_VSYNC_PERIOD_UNIT(1));//Set it to 1 to count in terms of complete horizontal lines
    SET_BIT(LCDIF->VDCTRL0, LCDIF_VDCTRL0_VSYNC_PULSE_WIDTH_UNIT_MASK, LCDIF_VDCTRL0_VSYNC_PULSE_WIDTH_UNIT(1));//Set it to 1 to count in terms of complete horizontal lines
    SET_BIT(LCDIF->VDCTRL0, LCDIF_VDCTRL0_VSYNC_PULSE_WIDTH_MASK, LCDIF_VDCTRL0_VSYNC_PULSE_WIDTH());



BF_CS1 (LCDIF_VDCTRL0, VSYNC_PULSE_WIDTH, 2);
BF_CS1 (LCDIF_VDCTRL1, VSYNC_PERIOD, 280);
BF_CS2 (LCDIF_VDCTRL2, HSYNC_PULSE_WIDTH, 10, HSYNC_PERIOD, 360); //Assuming 
 // LCD_DATABUS_WIDTH is 24bit
BF_CS2 (LCDIF_VDCTRL3, VSYNC_ONLY, 0);
BF_CS2 (LCDIF_VDCTRL3, HORIZONTAL_WAIT_CNT, 20, VERTICAL_WAIT_CNT, 20);
BF_CS1 (LCDIF_VDCTRL4, DOTCLK_H_VALID_DATA_CNT, 320);//Note that DOTCLK_V_VALID_DATA_CNT is
 //implicitly assumed to be HW_LCDIF_TRANSFER_COUNT_V_COUNT
BF_CS1 (LCDIF_VDCTRL4, SYNC_SIGNALS_ON, 1);
BF_CS1 (LCDIF_CTRL, RUN, 1);
}



void BSP_eLCDIF_Init(ELCD_HandleTypeDef *ElcdHandle) {






}