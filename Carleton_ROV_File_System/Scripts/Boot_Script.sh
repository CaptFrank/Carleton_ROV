#!/bin/sh

#  Boot_Script.sh
#  Cansat 2013 function set
#
#  Created by FRANCIS PAPINEAU on 12-06-25.
#  Copyright (c) 2012 carleton university. All rights reserved.

# This script starts all the services at boot up

# BOOT LINUX KERNEL

###############################################################
# UART SETUP
###############################################################
./UART5_tty(XBEE).sh
./UART1_tty(GPS).sh
###############################################################
#  CHECK MOUNTED DEVICES (FS)
###############################################################

echo "*** SETTING UP A SERVICE FOR XBEE UART1"

echo "#!/bin/bash" > /usr/bin/UART1_XBEE.sh
echo "/home/root/main" >> /usr/bin/UART1_XBEE.sh
chmod u+x /usr/bin/UART1_XBEE.sh

mv UART1_SERVICE.service /lib/systemd/UART1_SERVICE.service

cd /etc/systemd/system/
ln /lib/systemd/UART1_SERVICE.service UART1_SERVICE.service

systemctl daemon-reload
systemctl start UART1_SERVICE.service
systemctl enable UART1_SERVICE.service

df -h

###############################################################
#  SYSLOGS
###############################################################

cd /var

# CREATE THE HEADERS
header_creator

###############################################################
#  FIFO DIR. INST.
###############################################################

mkdir /fifo/

chmod 666 /fifo


###############################################################
#  RTC CONFIG
###############################################################

# CHECK TO SEE RTC
sudo i2cdetect -y 1
sudo modprobe rtc-ds1307
echo ds1307 0x68 > /sys/class/i2c-adapter/i2c-1/new_device

echo "*** BEFORE CLOCK SET"
hwclock -r
date

## read network time
## install localtimezone
## install ntpupdate
## set ln -s /usr/share/zoneinfo/America/New_York /etc/localtime

ntpdate pool.ntp.org

## or

## read the rtc
## write the current time

sudo hwclock -w

echo "*** AFTER CLOCK SET"
hwclock -r

# sudo nano /etc/modules
# add rtc-1307
# sudo nano /etc/rc.local
# echo ds1307 0x68 > /sys/class/i2c-adapter/i2c-0/new_device

###############################################################
#  GPIO BOOTUP
###############################################################

# SOME GUIDANCE
echo "*****************************************"
echo "* BOOTING UP THE CANSAT 2013 MODULE BY  *"
echo "* MODULE THIS MAY TAKE UP TO 10 SECONDS *"
echo "*****************************************"

cd /home/root

echo "*** STARTING RECORDING DAEMON"

script RECORD_$(date +"%m_%d_%Y").log

# CHECK THE GPIO CONFIGS FROM EEPROM
cd /sys/kernel/debug
cat gpio

# CHECK PWR GOOD SIGNAL
PWR_GOOD = cat /sys/class/gpio/gpio27/value
echo "*** POWER GOOD SIGNAL IS" $PWR_GOOD

if [PWR_GOOD -eq 0]; then
    echo "*** BAD PWR_GOOD SIGNAL REBOOTING"
    reboot
fi

###############################################################
#  ADC CHECKS
###############################################################

echo "*** SCANNING Voltages"
- SCAN VOLTAGES
- CONVST_N
- ALERT
- PWR_GOOD





