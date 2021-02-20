#!/bin/bash
make
sleep 5
sudo -p "osc"
sudo dmesg -c
clear

printf "Begin Script...\n\n"
sudo insmod seconds.ko
printf "Seconds should print three times of kernal running...\n\n"
sudo dmesg
sleep 1
cat /proc/seconds
sleep 1
cat /proc/seconds
sleep 1
cat /proc/seconds
sleep 1
sudo rmmod seconds
printf "\n...end of script (dmesg cleaned!)\n\n"
sudo dmesg -c