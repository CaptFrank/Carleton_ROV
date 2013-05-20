#!/bin/sh

#  Get_Logs_Utils.sh
#  Cansat_2013
#
#  Created by Francis Papineau on 2012-10-15.
#  Copyright (c) 2012 Francis Papineau. All rights reserved.

PASS="root"
DIRECTORY="/Users/francispapineau/Documents/CANSAT_LOGS"

echo "*** THIS UTILITY IS USED TO GET THE LOGS FROM THE BEAGLEBONE ***"
echo "*** GETTING ACCESS TO THE BEAGLEBONE's SFTP PORT (22) ***"

## Make a log dir and go to it
if [ ! -d "$DIRECTORY" ]; then
    mkdir /Users/francispapineau/Documents/CANSAT_LOGS_$(date +"%m_%d_%Y")
fi

cd /Users/francispapineau/Documents/CANSAT_LOGS_$(date +"%m_%d_%Y")

## Log in to the beaglebone
sftp root@beaglebone <<END_OF_SESSION
cd /

## Go to the right dir.
cd /var
ls -la

## Get the files
get xbee_logs.txt
get rtc_logs.txt
get mcp2300_logs.txt
get gyro_logs.txt
get gps_logs.txt
get current_temp_logs.txt
get compass_logs.txt
get adc_logs.txt
get accelerometer_logs.txt

cd /home/root/GPS_LOG
ls -la

get LOGs.txt

cd /var

## Remove remote files
rm xbee_logs.txt
rm rtc_logs.txt
rm mcp2300_logs.txt
rm gyro_logs.txt
rm gps_logs.txt
rm current_temp_logs.txt
rm compass_logs.txt
rm adc_logs.txt
rm accelerometer_logs.txt

cd /home/root/GPS_LOG

rm LOGs.txt

END_OF_SESSION

## Make an archive
cd ../
tar -cvf CANSAT_LOGS_$(date +"%m_%d_%Y").tar ./CANSAT_LOGS

## Go to secure shell again
sftp root@beaglebone <<END_OF_SESSION

## Go to the right dir. and copy the archive.
cd /var
put CANSAT_LOGS_$(date +"%m_%d_%Y").tar

END_OF_SESSION


## PC SETUP
#1. Clear the contents of your ~/.ssh directory and set the directory permissions to 600
#$ rm ~/.ssh/*
#$ chmod 600 ~/.ssh

#2. Generate your rsa key pair (there are variations for this, but this is the version I used):
#$ ssh-keygen -t rsa

#3. Set the permissions for all files in ~/.ssh to 600
#$ chmod 600 ~/.ssh

#4. Copy your id_rsa.pub key to your server’s .ssh/authorized_keys file

#5. Add your passphrase to your keychain using this command:
#$ ssh-add -K
#(you will see Enter passphrase for [your system]/.ssh/id_rsa: )

#6. You are all set to login using ssh without a password