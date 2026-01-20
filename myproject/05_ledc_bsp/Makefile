CROSS_COMPILE ?= arm-linux-gnueabihf-
TARGET ?= bsp
OUT_OBJDIR ?= obj

CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)ld
OBJCOPY := $(CROSS_COMPILE)objcopy
OBJDUMP := $(CROSS_COMPILE)objdump


INCDIRS := imx6ul\
           bsp/clk\
		   bsp/led\
		   bsp/delay

SRCDIRS := project\
		   bsp/clk\
		   bsp/led\
		   bsp/delay

INCLUDES := $(patsubst %,-I %,$(INCDIRS))

SFILES   := $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.s)) 
CFILES   := $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c)) 

SFILENDIR := $(notdir $(SFILES))
CFILENDIR := $(notdir $(CFILES))

SOBJS := $(patsubst %.s,$(OUT_OBJDIR)/%.o,$(SFILENDIR))
COBJS := $(patsubst %.c,$(OUT_OBJDIR)/%.o,$(CFILENDIR))
OBJS  := $(SOBJS) $(COBJS)

VPATH := $(SRCDIRS)

.PHONY: clean load


$(TARGET).bin:$(OBJS)
	$(LD) -Timx6ull.lds  -o $(TARGET).elf $^
	$(OBJCOPY) -O binary -S $(TARGET).elf $(TARGET).bin
	$(OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis

$(SOBJS):$(OUT_OBJDIR)/%.o:%.s
	$(CC)  -Wall -nostdlib  $(INCLUDES) -c -O2 $< -o $@

$(COBJS):$(OUT_OBJDIR)/%.o:%.c
	$(CC)  -Wall -nostdlib  $(INCLUDES) -c -O2 $< -o $@

clean:
	rm -rf $(OUT_OBJDIR)/*.o $(TARGET).bin $(TARGET).elf $(TARGET).dis

load:
	chmod 777 imxdownload
	./imxdownload ./$(TARGET).bin /dev/sdb		










