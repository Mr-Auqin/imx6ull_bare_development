#!/bin/bash
# 仅临时启用NAT模式上网卡&桥接模式上网卡并获取IP（重启失效）

# 定义颜色和图标（复制即用）
RED='\033[1;31m ❌'          # 文本加粗 红色（错误/叉）
GREEN='\033[1;32m ✅'        # 文本加粗 绿色（成功/对勾）
YELLOW='\033[1;33m ⚠️'       # 文本加粗 黄色（警告/感叹号）
RESET='\033[0m'         # 重置颜色

CHECKMARK="✅"
CROSS="❌"
WARNING="⚠️"

#定义核心变量
NAT_NIC='ens33' #NAT模式 上网卡
Bridge_NIC='ens38'  #桥接模式 上网卡


DevelopMent_Board_IP='192.168.10.12'  #定义开发板固定的IP
Windos_Host_IP='192.168.10.10'        #定义windows主机固定的IP
Bridge_NIC_IP='192.168.10.11'         #定义Ubuntu中 桥接网卡的IP
Bridge_NIC_Netmask='255.255.255.0'    #定义Ubuntu中 桥接网卡掩码


# 提权检查（必须root权限，否则命令执行失败）
if [ $EUID -ne 0 ]; then
    echo -e "${RED} 请用sudo运行:sudo ./network.sh ${RESET}"
    exit 1
fi

# 核心命令：启用双网卡
echo "🔧 正在启用NAT_NIC/Bridge_NIC网卡..."
ip link set ${NAT_NIC} up
ip link set ${Bridge_NIC} up

# 核心命令：获取DHCP IP 获取给到$NAT_NIC 用于NAT模式上网
echo "🔧 正在获取NAT_NIC IP地址..."
dhclient -v ${NAT_NIC} > /dev/null 2>&1

echo "🔧 正在设置Bridge_NIC 桥接模式下的 ip地址..."
ifconfig ${Bridge_NIC} ${Bridge_NIC_IP} netmask ${Bridge_NIC_Netmask} > /dev/null 2>&1

# 验证结果（可选，方便你看是否成功）
echo -e "\n $CHECKMARK 操作完成!当前网卡状态："
Check_NAT_NIC=$(ifconfig ${NAT_NIC} | grep "inet " | awk '{print $2}')
Check_Bridge_NIC_IP=$(ifconfig ${Bridge_NIC} | grep "inet " | awk '{print $2}')

# 小优化：IP为空时标红提示，避免显示空白
if [ -z "$Check_NAT_NIC" ]; then
    Check_NAT_NIC="${RED}获取失败${RESET}"
fi
if [ -z "$Check_Bridge_NIC_IP" ]; then
    Check_Bridge_NIC_IP="${RED}配置失败${RESET}"
fi

echo -e "${GREEN} Windos_Host_IP:       ${Windos_Host_IP}(三方互通) ${RESET}" 
echo -e "${GREEN} Bridge_NIC_IP:        ${Check_Bridge_NIC_IP}(三方互通) ${RESET}"
echo -e "${GREEN} DevelopMent_Board_IP: ${DevelopMent_Board_IP}(三方互通) ${RESET}"
echo -e "${GREEN} NAT_NIC_IP:           ${Check_NAT_NIC}(Ubuntu上网) ${RESET}"