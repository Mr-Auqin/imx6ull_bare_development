#!/bin/bash
# ###########################################
# 脚本名称:build_linux.sh
# 功能:一键编译 IMX6ULL linux(清理→配置→编译→验证）
# 使用说明:执行 ./build_linux.sh 即可
# ###########################################

# 定义颜色和图标（复制即用）
RED='\033[1;31m ❌'          # 文本加粗 红色（错误/叉）
GREEN='\033[1;32m ✅'        # 文本加粗 绿色（成功/对勾）
YELLOW='\033[1;33m ⚠️'       # 文本加粗 黄色（警告/感叹号）
RESET='\033[0m'         # 重置颜色

CHECKMARK="✅"
CROSS="❌"
WARNING="⚠️"



# 1. 定义核心变量(根据自己的环境修改）
ARCH="arm"                          # 架构:IMX6ULL 是 ARM
CROSS_COMPILE="arm-linux-gnueabihf-" # 交叉编译器前缀
DEFCONFIG="imx_v7_mfg_defconfig"     # Alientek 开发板配置文件
Linux_DIR=$(pwd)                     # 当前目录(linux 顶层目录）
JOBS=16                              # 多核编译(根据CPU核心数改，比如8核写8）

# 补充交叉编译器检查
#使用command -v 检查命令是否存在 &>  重定向 用来将标准输出 标准错误 都丢到/dev/null(空设备)
if ! command -v ${CROSS_COMPILE}gcc &> /dev/null; then
	echo -e "${RED}错误:未找到交叉编译器 ${CROSS_COMPILE}gcc${RESET}"
	exit 1
fi

#补充目录检查
if [ ! -f "Makefile" ];then
	echo -e "${RED}错误:请在linux的顶层目录运行此脚本!${RESET}"
	exit 1
fi

# 2. 打印提示信息(方便看执行进度）
echo "========================================"
echo "开始编译 IMX6ULL Linux"
echo "架构:$ARCH"
echo "交叉编译器:$CROSS_COMPILE"
echo "配置文件:$DEFCONFIG"
echo "编译目录:$Linux_DIR"
echo "========================================"

# 3. 深度清理(可选，根据需要注释/取消注释）
echo -e "\n【第一步】深度清理 Linux Kernel(恢复原始状态）..."
make ARCH=$ARCH CROSS_COMPILE=$CROSS_COMPILE distclean

# 4. 执行 defconfig 配置(生成 .config）
echo -e "\n【第二步】执行 $DEFCONFIG 配置..."
make ARCH=$ARCH CROSS_COMPILE=$CROSS_COMPILE $DEFCONFIG

# 5. 多核编译 Linux
echo -e "\n【第三步】开始编译 Linux Kernel(-j$JOBS 多核加速）..."
make ARCH=$ARCH CROSS_COMPILE=$CROSS_COMPILE -j$JOBS

# 6. 验证编译结果
echo -e "\n【第四步】验证编译产物..."
if [ -f "$Linux_DIR/zImage" ]; then
    echo -e "${GREEN}编译成功!zImage 已生成${RESET}"
    ls -l $Linux_DIR/zImage
else
    echo -e "${RED}编译失败!未找到 zImage${RESET}"
    exit 1 # 脚本异常退出
fi

# 7. 结束提示
echo -e "\n========================================"
echo "linux 编译脚本执行完成"
echo "========================================"
