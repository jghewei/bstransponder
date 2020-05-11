#!/bin/sh

export LD_LIBRARY_PATH=/mnt/active/lib

#ping -c 1 192.168.1.1

cd /mnt/active/ko
./digi_load

cd /mnt/active/bin
#./dtach -A /tmp/dtach-APP ./APP.bin
./dtach -n /tmp/dtach-APP ./app_log.sh

