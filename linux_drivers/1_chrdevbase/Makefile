# 1. 配置交叉编译器（根据你的工具链修改，IMX6ULL常用这个）
CROSS_COMPILE ?= arm-linux-gnueabihf-
# 2. 配置内核源码根目录（关键！改成你自己的内核源码路径）
KERNELDIR ?= /home/auqin/linux/linux-imx-rel_imx_4.1.15_2.1.0_ga
# 3. 当前目录（不用改，自动识别）
PWD := $(shell pwd)

# 4. 要编译的驱动模块名（和你的.c文件名一致）
MODULE_NAME := chrdevbase

# 5. 编译目标：默认编译模块，clean清理
obj-m := $(MODULE_NAME).o

# 6. 编译规则
all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules CROSS_COMPILE=$(CROSS_COMPILE) ARCH=arm -j16

# 7. 清理规则
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -rf .*.cmd *.o *.mod.c *.symvers *.order .tmp_versions