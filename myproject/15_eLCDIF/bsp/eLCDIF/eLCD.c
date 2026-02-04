#include "eLCD.h"



//MPU and VSYNC mode 是用于单帧传输的,DOTCLK and DVI mode 用于连续帧传输
//当一帧刷新完成时,会将LCDIF_NEXT_BUF_ADDR的值 更新到LCDIF_CUR_BUF_ADDR中去
//诺LCDIF_NEXT_BUF_ADDR的值在没有在帧刷新周期内被更改,则eLCDIF会继续保持传输最后一帧的内容 直到新值被编程到LCDIF_NEXT_BUF_ADDR寄存器中去





//The LCDIF_MASTER bit must be set to 1 to enable the bus master interface




