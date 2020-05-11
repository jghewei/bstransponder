obj-m			:= digi.o

PWD := $(shell pwd)
ARCH := powerpc
EXTRA_CFLAGS    := "-I$(PWD)"
#
# pick up build directory from the environment
#
BUILD_DIR=/root/linux-2.6.29.6 
CROSS_COMPILE=/opt/freescale/usr/local/gcc-4.1.78-eglibc-2.5.78-1/powerpc-e300c3-linux-gnu/bin/powerpc-e300c3-linux-gnu-
CROSS32_COMPILE=/opt/freescale/usr/local/gcc-4.1.78-eglibc-2.5.78-1/powerpc-e300c3-linux-gnu/bin/powerpc-e300c3-linux-gnu-

default:
	make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} -C $(BUILD_DIR) EXTRA_CFLAGS=${EXTRA_CFLAGS} M=${PWD} modules

all: 
	make default

clean: 
	@$(RM) -rf *.ko *.o *.mod.* Module.* modules.order .digi*.o.cmd .digi*.* .tmp_versions

release: all
	@$(RM) -rf *.o *.mod.* Module.* modules.order .digi*.o.cmd .digi*.* .tmp_versions

# change the linux kernel source code include/linux/utsrelease.h
# digi: version magic '2.6.29.6-rt23 mod_unload ' should be '2.6.29.6-rt23-ga479b6a-dirty mod_unload '
