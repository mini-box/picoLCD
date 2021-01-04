#!/bin/bash
internet_ip=86.124.32.193
network_ip=192.168.33.33
while [ 1 ]; do
cnt=0 
internet_msg="Online"
network_msg="Online"
i=`cat /proc/sys/dev/sensors/vt1211-isa-6000/temp3 | cut -d " " -f 3`; 
j=`echo "($i-32.0)*5/9" | bc -l`; 
k=`top -n 2 -b -p 0 | tail -9 | grep Cpu | cut -d "," -f 4`; 
picolcd clear text "CPU Temp: ${j}C ${i}F"; 
picolcd text 1 0 "Idle: ${k}";
sleep 1;
while [ $cnt -lt 4 ]; do
eth0=`/opt/rrdtool/cban -i eth0 -m -k | tr "\n" " "`
eth1=`/opt/rrdtool/cban -i eth1 -m -k | tr "\n" " "`
picolcd clear text 0 0 "I/O Eth0: ${eth0}"
picolcd text 1 0 "I/O Eth1: ${eth1}"
let cnt=$cnt+1
done
sleep 1;
fan1=`cat /proc/sys/dev/sensors/vt1211-isa-6000/fan1 | cut -d " " -f 2`; 
fan2=`cat /proc/sys/dev/sensors/vt1211-isa-6000/fan2 | cut -d " " -f 2`; 
picolcd clear text 0 0 "CPU  Fan RPM: ${fan1}"
picolcd text 1 0 "Case Fan RPM: ${fan2}"
sleep 2;
ping -c 2 $internet_ip || internet_msg="Offline"
ping -c 2 $network_ip || network_msg="Offline"
picolcd clear text 0 0 "Internet: ${internet_msg}"
picolcd text 1 0 "Network: ${network_msg}"

done
