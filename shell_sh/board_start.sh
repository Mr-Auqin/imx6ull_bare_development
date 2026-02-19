#!/bin/bash
# ###########################################
# 脚本名称:Board_Start.sh
# 功能:一键配置开发板网络地址 使用nfs 挂载 ubuntu 开发目录
# 使用说明:执行 ./board_start.sh 即可
# ###########################################

# 定义颜色和图标（复制即用）
RED='\033[1;31m ❌'          # 文本加粗 红色（错误/叉）
GREEN='\033[1;32m ✅'        # 文本加粗 绿色（成功/对勾）
YELLOW='\033[1;33m ⚠️'       # 文本加粗 黄色（警告/感叹号）
RESET='\033[0m'              # 重置颜色

CHECKMARK="✅"
CROSS="❌"
WARNING="⚠️"


echo -e "\n【第一步】配置 eth0 网络 ip & netmask ..."
ifconfig eth0 192.168.10.12 netmask 255.255.255.0

echo -e "\n【第二步】通过网络文件系统nfs 挂载 Ubuntu 下的开发驱动目录"
mount -t nfs -o vers=3 192.168.10.11:/home/auqin/imax6ull_bare_development/linux_drivers ~/nfs_share


# 7. 结束提示
echo -e "\n========================================"
echo "Board Start 配置ETH0IP & NFS 挂载完成"
echo "========================================"

echo -e "${GREEN} eth0 192.168.10.12 ${RESET}" 
mount | grep nfs