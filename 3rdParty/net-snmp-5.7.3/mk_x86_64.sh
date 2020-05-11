#!/bin/bash

rm -rf build_x86_64
mkdir build_x86_64

tar xzvf net-snmp-5.7.3.tar.gz
cd net-snmp-5.7.3

#./configure --with-out-mib-modules="mibII" --disable-ucd-snmp-compatibility --enable-minimalist -enable-mini-agent \
#--disable-applications --disable-manuals --disable-scripts --disable-mibs --disable-mib-loading --disable-ipv6  \
#--with-default-snmp-version="2" --with-sys-contact="" --with-sys-location="" --disable-embedded-perl --without-perl-modules \
#--target=powerpc-linux --host=powerpc-linux --with-persistent-directory="/tmp/snmp" --with-logfile="/tmp/snmp/snmpd.log" \
#--prefix=`pwd`/../build_x86_64

./configure --with-out-mib-modules="mibII" --disable-ucd-snmp-compatibility --disable-agent \
--disable-applications --disable-manuals --disable-scripts --disable-mibs --disable-mib-loading --disable-ipv6  \
--with-default-snmp-version="2" --with-sys-contact="" --with-sys-location="" --disable-embedded-perl --without-perl-modules \
--target=powerpc-linux --host=powerpc-linux --with-persistent-directory="/tmp/snmp" --with-logfile="/tmp/snmp/snmpd.log" \
--disable-static --enable-shared \
--prefix=`pwd`/../build_x86_64

make
make install

cd ..
rm -rf net-snmp-5.7.3

