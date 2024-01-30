# 172.16.165.183:9292

# windows10 git bash环境下，编写易于阅读的shell脚本，基于ipconfig的输出得到本机ip和子网掩码，
# 基于本机ip和子网掩码通过ping扫描本机局域网下的所有主机，最后通过arp获得局域网下的所有主机信息

#!/bin/bash
# Get the local IP address and subnet mask from ipconfig
ipconfig_output=$(ipconfig)
ip_addr=$(echo "${ipconfig_output}" | awk -F":" '/IPv4 Address/{print $2}')
echo "ip_addr: ${ip_addr}"
netmask=$(echo "${ipconfig_output}" | awk -F":" '/Subnet Mask/ {print $2}')
echo "netmask: ${netmask}"

ip_addr_part1Str=$(echo "$ip_addr" | awk -F'.' '{print $1}')
ip_addr_part2Str=$(echo "$ip_addr" | awk -F'.' '{print $2}')
ip_addr_part3Str=$(echo "$ip_addr" | awk -F'.' '{print $3}')
ip_addr_part4Str=$(echo "$ip_addr" | awk -F'.' '{print $4}')
ip_addr_mask_part1Str=$(echo "$netmask" | awk -F'.' '{print $1}')
ip_addr_mask_part2Str=$(echo "$netmask" | awk -F'.' '{print $2}')
ip_addr_mask_part3Str=$(echo "$netmask" | awk -F'.' '{print $3}')
ip_addr_mask_part4Str=$(echo "$netmask" | awk -F'.' '{print $4}')
echo "ip_addr_partStr: ${ip_addr_part1Str} ${ip_addr_part2Str} ${ip_addr_part3Str} ${ip_addr_part4Str}"
echo "ip_addr_mask_partStr: ${ip_addr_mask_part1Str} ${ip_addr_mask_part2Str} ${ip_addr_mask_part3Str} ${ip_addr_mask_part4Str}"

ip_addr_part1=$((ip_addr_part1Str))
ip_addr_part2=$((ip_addr_part2Str))
ip_addr_part3=$((ip_addr_part3Str))
ip_addr_part4=$((ip_addr_part4Str))
ip_addr_mask_part1=$((ip_addr_mask_part1Str))
ip_addr_mask_part2=$((ip_addr_mask_part2Str))
ip_addr_mask_part3=$((ip_addr_mask_part3Str))
ip_addr_mask_part4=$((ip_addr_mask_part4Str))
echo "ip_addr_part: ${ip_addr_part1} ${ip_addr_part2} ${ip_addr_part3} ${ip_addr_part4}"
echo "ip_addr_mask_part: ${ip_addr_mask_part1} ${ip_addr_mask_part2} ${ip_addr_mask_part3} ${ip_addr_mask_part4}"


local_networkmaskStr="${ip_addr_part3} & ${ip_addr_mask_part3}"
echo "local_networkmaskStr: ${local_networkmaskStr}"
local_networkmask=$((local_networkmaskStr))
echo "local_networkmask: ${local_networkmask}"

for i in $(seq ${local_networkmask} 254); do
  echo "i: ${i}"
  for j in $(seq 1 30); do
    ip="${ip_addr_part1}.${ip_addr_part2}.${i}.${j}"
    # echo "ping ip: $ip"
    ping -c 1 -w 1 "$ip" >/dev/null 2>&1 && echo "Host $ip is up"
  done
done

# Get the MAC addresses of all hosts in the local subnet
arp_output=$(arp -a)
echo "$arp_output"



