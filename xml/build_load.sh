#!/bin/bash

cd ./appfs/
rm ./lib/libpm5440.so
rm ./lib/libgoahead.so
rm ./bin/digi_fw_app_revb.mem
tar czvf ../mxp_load.tgz *
#cp ../mxp_load.tgz ${DanOS_DIR}/../../nfsroot/
cp ../mxp_load.tgz /root/share/nfsroot/

#echo "***** Building appfs image file"

#mkdir -p ./app/root > /dev/null 2>&1 || true

#set -e

#make install

#if rev=`svn info ../ 2>/dev/null | grep '^Revision'`; then
#    rev=`echo $rev | awk '{print $NF}'`
#fi
#BLD_V=$rev
#BLD_DATE=`date`
#BLD_BY=`whoami`
#echo "Build version is $BLD_V, date is $BLD_DATE, by $BLD_BY"
#echo "Build By  : $BLD_BY" > ./app/root/usr/local/etc/swversion
#echo "Build Rev : $BLD_V" >> ./app/root/usr/local/etc/swversion
#echo "Build Date: $BLD_DATE" >> ./app/root/usr/local/etc/swversion
#echo "" >> ./app/root/usr/local/etc/swversion
#set +e
#pfrev=`svn info $WORKING_DIR/$REPO_CORIANTPF 2>/dev/null | grep "^URL\|^Last\ Changed\ Rev"`
#set -e
#if [ -z "$pfrev" ]; then #using the new integrated workspace
#    svn info $WORKING_DIR/.. | grep "^URL\|^Last\ Changed\ Rev" | xargs >> ./app/root/usr/local/etc/swversion
#else
#    echo $pfrev >> ./app/root/usr/local/etc/swversion
#    svn info $WORKING_DIR/$REPO_CORIANTOS | grep "^URL\|^Last\ Changed\ Rev" | xargs >> ./app/root/usr/local/etc/swversion
#    svn info $WORKING_DIR/$REPO_3RDPARTY | grep "^URL\|^Last\ Changed\ Rev" | xargs >> ./app/root/usr/local/etc/swversion
#fi

#CURR_SIZE=$(echo "0$(/usr/bin/du -s --apparent-size --block-size=1K ./app/root | cut -f1) * 1.10" | bc -l | xargs printf "%1.0f")
#$WORKING_DIR/$REPO_3RDPARTY/Coriant/SDK/WindRiver/wrlplat/tlabqoriq50_small/build/rootfs/sdk/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/genext2fs \
#../tools/genext2fs \
#	--size-in-blocks $CURR_SIZE --root ./app/root ./app/app.img

#echo ***Compressing load image file ...
#gzip -c ./app/app.img > ./app/app.img.gz

#echo ***Appending load header ...
#UNCOMP_SZ=$(ls -l ./app/app.img|cut -d ' ' -f 5)
#echo "uncompressed appfs image size $UNCOMP_SZ"
#$WORKING_DIR/$REPO_3RDPARTY/Coriant/Tools/Production/AppendHeader/bin/linux_x86/AppendHeader ./app/app.img.gz ./app/app.tlf \
#        -TYPE=APPL -LOAD=0 -ENTRY=0 -EXEC=RAM \
#        -CARD_TYPE=GROOVE \
#        -RELEASE=0 \
#        -BUILD=$BLD_V \
#        -PATCH_VER=0 \
#        -UNCOMP_SIZE=$UNCOMP_SZ
#dd if=./app/app.tlf bs=256 count=1 of=./app/app.tlf.hdr 2>/dev/null
