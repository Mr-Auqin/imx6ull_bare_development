CROSS_COMPILE ?= arm-linux-gnueabihf-
TARGET ?= uart
OUT_OBJDIR ?= obj

CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)ld
OBJCOPY := $(CROSS_COMPILE)objcopy
OBJDUMP := $(CROSS_COMPILE)objdump


INCDIRS := imx6ul\
           bsp/clk\
		   bsp/led\
		   bsp/delay\
		   bsp/beep\
		   bsp/key\
		   bsp/gpio\
		   bsp/exit\
		   bsp/int\
		   bsp/epit\
		   bsp/KeyFilter\
		   bsp/gpt\
		   bsp/uart

SRCDIRS := project\
		   bsp/clk\
		   bsp/led\
		   bsp/delay\
		   bsp/beep\
		   bsp/key\
		   bsp/gpio\
		   bsp/exit\
		   bsp/int\
		   bsp/epit\
		   bsp/KeyFilter\
		   bsp/gpt\
		   bsp/uart

INCLUDES := $(patsubst %,-I %,$(INCDIRS))

SFILES   := $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.s)) 
CFILES   := $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c)) 

SFILENDIR := $(notdir $(SFILES))
CFILENDIR := $(notdir $(CFILES))

SOBJS := $(patsubst %.s,$(OUT_OBJDIR)/%.o,$(SFILENDIR))
COBJS := $(patsubst %.c,$(OUT_OBJDIR)/%.o,$(CFILENDIR))
OBJS  := $(SOBJS) $(COBJS)

VPATH := $(SRCDIRS)

.PHONY: clean load test


$(TARGET).bin:$(OBJS)
	@echo "Link $^ to $(TARGET).elf "
	@$(LD) -Timx6ull.lds  -o $(TARGET).elf $^
	@echo "OBJCOPY $(TARGET).elf to $@ "
	@$(OBJCOPY) -O binary -S $(TARGET).elf $(TARGET).bin
	@echo "OBJDUMP $(TARGET).elf to $(TARGET).dis "
	@$(OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis
	@echo "**************************************************"
	@echo "Target has been generated"
	@echo "Use \"make load\" to Download to target board"
	@echo "**************************************************"

$(SOBJS):$(OUT_OBJDIR)/%.o:%.s
	@echo "compile $< to $@ "
	@$(CC)  -Wall -nostdlib  $(INCLUDES) -c -O2 $< -o $@

$(COBJS):$(OUT_OBJDIR)/%.o:%.c
	@echo "compile $< to $@ "
	@$(CC)  -Wall -nostdlib  $(INCLUDES) -c -O2 $< -o $@

clean:
	rm -rf $(OUT_OBJDIR)/*.o *.bin *.elf *.dis

load:
	@chmod 777 imxdownload
	@./imxdownload ./$(TARGET).bin /dev/sdb		
