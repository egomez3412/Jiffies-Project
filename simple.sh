#!/bin/bash
make
sleep 5
sudo -p "osc"
sudo dmesg -c
clear

printf "Begin Script...\n\n"
sudo insmod simple.ko
sudo rmmod simple
sudo dmesg
printf "\n...end of script\n\n"