#!/bin/bash
# ###########################################
# 脚本名称：build_uboot.sh
# 功能：一键编译 IMX6ULL U-Boot（清理→配置→编译→验证）
# 使用说明：执行 ./build_uboot.sh 即可
# ###########################################

# 1. 定义核心变量（根据自己的环境修改）
ARCH="arm"                          # 架构：IMX6ULL 是 ARM
CROSS_COMPILE="arm-linux-gnueabihf-" # 交叉编译器前缀
DEFCONFIG="mx6ull_14x14_ddr512_emmc_defconfig" # Alientek 开发板配置文件
UBOOT_DIR=$(pwd)                    # 当前目录（U-Boot 顶层目录）
JOBS=4                              # 多核编译（根据CPU核心数改，比如8核写8）

# 补充交叉编译器检查
#使用command -v 检查命令是否存在 &>  重定向 用来将标准输出 标准错误 都丢到/dev/null(空设备)
if ! command -v ${CROSS_COMPILE}gcc &> /dev/null; then
	echo -e "\033[31m错误：未找到交叉编译器 ${CROSS_COMPILE}gcc\033[0m"
	exit 1
fi

#补充目录检查
if [ ! -f "Makefile" ];then
	echo -e "\033[31m错误：未找到交叉编译器 ${CROSS_COMPILE}gcc\033[0m"
	exit 1
fi

# 2. 打印提示信息（方便看执行进度）
echo "========================================"
echo "开始编译 IMX6ULL U-Boot"
echo "架构：$ARCH"
echo "交叉编译器：$CROSS_COMPILE"
echo "配置文件：$DEFCONFIG"
echo "编译目录：$UBOOT_DIR"
echo "========================================"

# 3. 深度清理（可选，根据需要注释/取消注释）
echo -e "\n【第一步】深度清理 U-Boot（恢复原始状态）..."
make ARCH=$ARCH CROSS_COMPILE=$CROSS_COMPILE distclean

# 4. 执行 defconfig 配置（生成 .config）
echo -e "\n【第二步】执行 $DEFCONFIG 配置..."
make ARCH=$ARCH CROSS_COMPILE=$CROSS_COMPILE $DEFCONFIG

# 5. 多核编译 U-Boot
echo -e "\n【第三步】开始编译（-j$JOBS 多核加速）..."
make ARCH=$ARCH CROSS_COMPILE=$CROSS_COMPILE -j$JOBS

# 6. 验证编译结果
echo -e "\n【第四步】验证编译产物..."
if [ -f "$UBOOT_DIR/u-boot.bin" ]; then
    echo -e "\033[32m编译成功！u-boot.bin 已生成\033[0m"
    ls -l $UBOOT_DIR/u-boot.bin
else
    echo -e "\033[31m编译失败！未找到 u-boot.bin\033[0m"
    exit 1 # 脚本异常退出
fi

# 7. 结束提示
echo -e "\n========================================"
echo "U-Boot 编译脚本执行完成"
echo "========================================"
