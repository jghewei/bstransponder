#!/bin/sh

cp -r /mnt/appfs/* /mnt/active/

cd /mnt/active/lib/
ln -s libnetsnmp.so libnetsnmp.so.30
ln -s libnetsnmpagent.so libnetsnmpagent.so.30 
ln -s libnetsnmphelpers.so libnetsnmphelpers.so.30
ln -s libnetsnmpmibs.so libnetsnmpmibs.so.30
cd -

