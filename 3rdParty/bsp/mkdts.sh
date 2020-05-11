#!/bin/bash

./dtc -O dtb -o mpc8308.dtb -b 0 -p 1024 mpc8308.dts
cp mpc8308.dtb /root/share/nfsroot/

