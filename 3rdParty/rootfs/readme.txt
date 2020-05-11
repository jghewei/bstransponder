rm -rf ~/temp/; mkdir -p ~/temp/loop
cp ./rootfs.ext2.gz ~/temp; cd ~/temp
gunzip rootfs.ext2.gz
mount -o loop rootfs.ext2 loop/
cd loop && ls
cd ..; umount loop/
gzip rootfs.ext2
mkimage -n 'uboot ext2 ramdisk rootfs' -A ppc -O linux -T ramdisk -C gzip -d rootfs.ext2.gz rootfs.ext2.gz.uboot
cp rootfs.ext2.gz ${DanOS_DIR}/3rdParty/rootfs/; cp rootfs.ext2.gz.uboot ${DanOS_DIR}/3rdParty/rootfs/
cd ${DanOS_DIR}/3rdParty/rootfs/
cp rootfs.ext2.gz.uboot ../../../../nfsroot/


LTIB生成文件系统(ramdisk, jffs, jffs2, cramfs)镜像的子功能函数mk_fs_image位于安装目录的bin/Ltibutils.pm, 此文件是perl的模块库，定义了很多函数，由安装目录下的perl执行脚本ltib进行调用。
　　 
　　对于LTIB自动生成的文件系统位于安装目录下的rootfs目录(包含文件多，比较大)
　　在rootfs目录的基础上又进行裁减(如去除含有内核镜像的/boot目录, /usr/lib/*.a, /usr/include/*等)，会生成比较适合下载到开发板上的根文件系统rootfs.tmp(此目录默认情况下编译结束会z自动删除)。
　　 修改Ltibutils.pm脚本(注释818 rm -rf $stage)或者在使用./ltib --preconfig config/platform/mpc8349itx/defconfig-min-fs 编译弹出第一个配置菜单时进入
　　 --- Target Image Generation 
　　 Options ---> 进入选择
　　 [*] Keep temporary staging directory 
　　 这样编译结束会保留临时根文件系统rootfs.tmp
　　在rootfs.tmp的基础上添加设备文件，生成压缩(genext2fs, gzip)的文件为rootfs.ext2.gz(RAMDISK文件系统, ext2格式可读写)
　　用于uboot启动的文件系统镜像是rootfs.ext2.gz.uboot
　　 
　　 
　　 
　　定制我们需要的文件系统有三种方式:
　　
　　
　　 
　　A: 只需添加少量文件到文件系统中
　　rootfs.ext2.gz 在Ltibutils.pm脚本中是先得到根文件系统的大小(du -slk),然后再添加一定大小(语句: $fs_size += $fs_size >= 20000 ? 16384 : 2400;), 然后用此大小来做根分区的大小。所以可以在rootfs.ext2.gz中添加少量文件.(只要不超过根分区的大小即可)
　　利用rootfs.ext2.gz修改添加文件到文件系统镜像方法:
　　$ mkdir -p ~/temp/loop
　　$ cp /usr/local/mpc8349/small/rootfs.ext2.gz ~/temp
　　$ cd ~/temp
　　$ gunzip rootfs.ext2.gz
　　$ su root
　　# mount -o loop rootfs.ext2 loop/
　　# cd loop && ls
　　# df ./ ## 查看剩余空间大小，空间使用率
　　# 添加自己的应用程序库脚本等到目录中，或修改启动脚本etc/rc.d/rcS(注意权限)
　　# cd /home/liuby/temp/
　　# umount loop/
　　# exit
　　$ gzip rootfs.ext2 或者gzip -v9 rootfs.ext2
　　$ mkimage -n 'uboot ext2 ramdisk rootfs' -A ppc -O linux -T ramdisk -C gzip -d rootfs.ext2.gz rootfs.ext2.gz.uboot
　　 ## 在镜像前添加64字节的头，为uboot提供必要的信息。
　　 ## 注: mkimage 位于 /opt/freescale/ltib/usr/bin/mkimage, 可以将其复制到/usr/bin/目录下
　　 ## /opt/freescale/ltib/usr/bin/下有许多常用工具
　　 ## mkimage用法参考http://blog.chinaunix.net/u2/80953/showart.php?id=1890789 
　　$ ls rootfs.ext2.gz.uboot ## 即可下载使用
　　 
Image Name:   uboot ext2 ramdisk rootfs
Created:      Tue Oct 25 17:49:02 2016
Image Type:   PowerPC Linux RAMDisk Image (gzip compressed)
Data Size:    4336282 Bytes = 4234.65 kB = 4.14 MB
Load Address: 0x00000000
Entry Point:  0x00000000

mkimage -n 'uboot ext2 ramdisk rootfs' -A ppc -O linux -T ramdisk -C gzip -d ./rootfs.ext2.gz ./rootfs.ext2.gz.uboot

其中有几点要注意：

1、关于偏移量的问题（请千万注意这一点）。‘-e’命令之后的地址，必须比’-a’命令后的地址多0x40的偏移量。一旦没有偏移，则会导致系统出现引导失败的问题。而很多博客文章里，居然把这2个值写成一样的，都写成0x30008000。也不知道各位博主们自己到底有没有尝试过，害我一开始折腾了半天才发现这个问题，简直就是浪费我的时间。

2、执行该命令时，要注意一下权限的问题。我一开始时在虚拟机的共享目录下执行，一直提示失败：

leon@Ubuntu:/mnt/share/tmp/zImages$ mkimage -n 'tekkaman' -A arm -O linux -T kernel -C none -a 0x30008000 -e 0x30008040 -d zImage zImage.imgmkimage: Can't map zImage.img: Invalid argument
后来跳转到~目录下执行，就一下子成功了。成功后的提示信息如下：

leon@Ubuntu:~$ mkimage -n 'tekkaman' -A arm -O linux -T kernel -C none -a 0x30008000 -e 0x30008040 -d zImage zImage.imgImage Name:   leon_20170201Created:      Wed Feb  1 14:50:47 2017Image Type:   ARM Linux Kernel Image (uncompressed)Data Size:    2266576 Bytes = 2213.45 kB = 2.16 MBLoad Address: 30008000Entry Point:  30008040
　　
　　
　　B: 自由定制根分区大小
　　同样利用rootfs.ext2.gz
　　$ mkdir -p ~/temp/loop
　　$ cp /usr/local/mpc8349/small/rootfs.ext2.gz ~/temp
　　$ cd ~/temp
　　$ gunzip rootfs.ext2.gz
　　$ su root
　　# mount -o loop rootfs.ext2 loop/
　　# cd loop && ls
　　# df ./ 
　　# 添加自己的应用程序库脚本等到目录中，或修改启动脚本etc/rc.d/rcS(注意权限)
　　# fs_size=`LANG=C du -slk loop/ | cut -f1` ## 统计loop目录大小
　　# fs_count=`find loop/ | wc -l` ## 统计loop/目录中的文件节点数目, 功能同perl 中find( sub { $fs_count++}, loop/ );
　　# echo $fs_count && echo $fs_size
　　# inodes=`expr $fs_count + 400` ## inodes为索引节点数。下面创建的ext2文件系统中最多有$inodes个节点
　　# blocks=`expr $fs_size + 2400` ## 为根分区增加一些空闲空间(2400kB，也可以更大些)
　　# echo $inodes && echo $blocks
　　# genext2fs -U -b $blocks -i $inodes -d loop/ loop.ext2
　　 ## 根据loop目录创建ext2文件系统镜像，分区大小为$blocks kByte. inode数目为$inodes
　　 ## 注: genext2fs 位于 /opt/freescale/ltib/usr/bin/genext2fs, 可以将其复制到/usr/bin/目录下
　　 ## genext2fs用法见附录
　　# gzip -v9 loop.ext2
　　# mkimage -n 'uboot ext2 ramdisk rootfs' -A ppc -O linux -T ramdisk -C gzip -d loop.ext2.gz loop.ext2.gz.uboot
　　# exit && ls loop.ext2.gz.uboot ## 根文件系统loop.ext2.gz.uboot
　　 
　　 
　　 
　　
　　
　　
　　
　　
　　C: 自由定制根文件系统
　　利用编译时的临时文件系统目录rootfs.tmp
　　rootfs.tmp其实就是相当于创建常用目录，添加一些命令和Busybox，可以试着自己做一个
　　$ cd /usr/local/mpc8349/small/ ## ltib的安装目录, 配置编译后产生rootfs.tmp目录
　　$ rm -f rootfs.ext2.gz rootfs.ext2.gz.uboot
　　$ cd rootfs.tmp/ && ls
　　$ 添加自己的应用程序或库文件或脚本到对应的目录中，或修改启动脚本etc/rc.d/rcS(注意权限)
　　$ cd /usr/local/mpc8349/small/rootfs.tmp/
　　$ fs_size=`LANG=C du -slk rootfs.tmp/ | cut -f1`
　　$ fs_count=`find rootfs.tmp/ | wc -l`
　　$ echo $fs_count && echo $fs_size
　　$ inodes=`expr $fs_count + 400` 
　　$ blocks=`expr $fs_size + 2400`
　　$ echo $inodes && echo $blocks
　　$ genext2fs -v -U -b $blocks -i $inodes -D bin/device_table.txt -d rootfs.tmp/ rootfs.ext2
　　 ## -D bin/device_table.txt是根据bin/device_table.txt来产生/dev下设备节点表
　　 ## -v 显示文件系统结构(可不用)
　　$ gzip -v9 rootfs.ext2
　　$ mkimage -n 'uboot ext2 ramdisk rootfs' -A ppc -O linux -T ramdisk -C gzip -d rootfs.ext2.gz rootfs.ext2.gz.uboot
　　
　　
　　
　　注: 如果ext2文件系统大小(blocks kB)超过默认的4096kB, 则需要在uboot的环境变量中添加启动命令行参数来指定ramdisk_size大小至少为${blocks}kB. 例如:
　　=> setenv bootargs root=/dev/ram rw ramdisk_size=$blocks
　　
　　
　　
　　附录: genext2fs 用法: genext2fs [options] image
　　根据目录或文件为嵌入式系统创建产生一个ext2文件系统镜像(Create an ext2 filesystem image from directories/files);
　　它是嵌入式中生成ramdisk文件系统的重要工具(ext2格式, 可写可读)
　　 -x image Use this image as a starting point
　　 -d directory Add this directory as source
　　 -b blocks Size in blocks(指定大小: 1 block = 1kB)
　　 -i inodes Number of inodes
　　 -r reserved Number of reserved blocks
　　 -g path Generate a block map file for this path
　　 -e value Fill unallocated blocks with value
　　 -z Make files with holes
　　 -D,-f Use the named FILE as a device table file
　　 -q Squash permissions and owners making all files be owned by root
　　 -U Squash owners making all files be owned by root
　　 -P Squash permissions on all files
　　 -v Print resulting filesystem structure
　　 -h Show this help
　　
　　
　　
　　
　　genext2fs调用的文件device_table.txt参考自http://buildroot.uclibc.org/网站的/trunk/buildroot/target/generic/device_table.txt
　　附上面用的device_table.txt
　　#
　　# When building a target filesystem, it is desirable to not have to
　　# become root and then run 'mknod' a thousand times. Using a device 
　　# table you can create device nodes and directories "on the fly".
　　#
　　# This is a sample device table file for use with genext2fs. You can
　　# do all sorts of interesting things with a device table file. For
　　# example, if you want to adjust the permissions on a particular file
　　# you can just add an entry like:
　　# /sbin/foobar f 2755 0 0 - - - - -
　　# and (assuming the file /sbin/foobar exists) it will be made setuid
　　# root (regardless of what its permissions are on the host filesystem.
　　# Furthermore, you can use a single table entry to create a many device
　　# minors. For example, if I wanted to create /dev/hda and /dev/hda[0-15]
　　# I could just use the following two table entries:
　　# /dev/hda b 640 0 0 3 0 0 0 -
　　# /dev/hda b 640 0 0 3 1 1 1 15
　　# 
　　# Device table entries take the form of:
　　# 
　　# where name is the file name, type can be one of: 
　　# f A regular file
　　# d Directory
　　# c Character special device file
　　# b Block special device file
　　# p Fifo (named pipe)
　　# uid is the user id for the target file, gid is the group id for the
　　# target file. The rest of the entries (major, minor, etc) apply only 
　　# to device special files.
　　# Have fun
　　# -Erik Andersen 
　　#
　　# 
　　/dev d 755 0 0 - - - - -
　　/dev/pts d 755 0 0 - - - - -
　　/dev/shm d 755 0 0 - - - - -
　　/dev/input d 755 0 0 - - - - -
　　#/tmp d 1777 0 0 - - - - -
　　#/etc d 755 0 0 - - - - -
　　#/home/default d 2755 1000 1000 - - - - -
　　# 
　　#/bin/busybox f 4755 0 0 - - - - -
　　#/etc/shadow f 600 0 0 - - - - -
　　#/etc/passwd f 644 0 0 - - - - -
　　#/etc/network/if-up.d d 755 0 0 - - - - -
　　#/etc/network/if-pre-up.d d 755 0 0 - - - - -
　　#/etc/network/if-down.d d 755 0 0 - - - - -
　　#/etc/network/if-post-down.d d 755 0 0 - - - - -
　　#/usr/share/udhcpc/default.script f 755 0 0 - - - - -
　　# uncomment this to allow starting x as non-root
　　#/usr/X11R6/bin/Xfbdev f 4755 0 0 - - - - -
　　# Normal system devices
　　/dev/mem c 640 0 0 1 1 0 0 -
　　/dev/kmem c 640 0 0 1 2 0 0 -
　　/dev/null c 666 0 0 1 3 0 0 -
　　/dev/zero c 666 0 0 1 5 0 0 -
　　/dev/random c 666 0 0 1 8 0 0 -
　　/dev/urandom c 666 0 0 1 9 0 0 -
　　/dev/ram b 640 0 0 1 1 0 0 -
　　/dev/ram b 640 0 0 1 0 0 1 4
　　/dev/loop b 640 0 0 7 0 0 1 2
　　/dev/rtc c 640 0 0 10 135 - - -
　　/dev/console c 666 0 0 5 1 - - -
　　/dev/tty c 666 0 0 5 0 - - -
　　/dev/tty c 666 0 0 4 0 0 1 8
　　/dev/ttyp c 666 0 0 3 0 0 1 10
　　/dev/ptyp c 666 0 0 2 0 0 1 10
　　/dev/ptmx c 666 0 0 5 2 - - -
　　/dev/ttyP c 666 0 0 57 0 0 1 4
　　/dev/ttyS c 666 0 0 4 64 0 1 4
　　/dev/fb c 640 0 5 29 0 0 32 4
　　#/dev/ttySA c 666 0 0 204 5 0 1 3
　　/dev/psaux c 666 0 0 10 1 0 0 -
　　/dev/ppp c 666 0 0 108 0 - - -
　　/dev/ttyAM c 666 0 0 204 16 0 1 4
　　/dev/ttyCPM c 666 0 0 204 46 0 1 2
　　# MTD stuff
　　/dev/mtd c 640 0 0 90 0 0 2 7
　　/dev/mtdblock b 640 0 0 31 0 0 1 7
　　#Tun/tap driver
　　/dev/net d 755 0 0 - - - - -
　　/dev/net/tun c 660 0 0 10 200 - - -
　　# Audio stuff
　　/dev/audio c 666 0 29 14 4 - - -
　　/dev/audio1 c 666 0 29 14 20 - - -
　　/dev/dsp c 666 0 29 14 3 - - -
　　/dev/dsp1 c 666 0 29 14 19 - - -
　　/dev/sndstat c 666 0 29 14 6 - - -
　　# User-mode Linux stuff
　　/dev/ubda b 640 0 0 98 0 0 0 -
　　/dev/ubda b 640 0 0 98 1 1 1 8
　　# IDE Devices
　　/dev/hda b 640 0 0 3 0 0 0 -
　　/dev/hda b 640 0 0 3 1 1 1 8
　　/dev/hdb b 640 0 0 3 64 0 0 -
　　/dev/hdb b 640 0 0 3 65 1 1 8
　　#/dev/hdc b 640 0 0 22 0 0 0 -
　　#/dev/hdc b 640 0 0 22 1 1 1 8
　　#/dev/hdd b 640 0 0 22 64 0 0 -
　　#/dev/hdd b 640 0 0 22 65 1 1 8
　　# SCSI Devices
　　/dev/sda b 640 0 0 8 0 0 0 -
　　/dev/sda b 640 0 0 8 1 1 1 8
　　/dev/sdb b 640 0 0 8 16 0 0 -
　　/dev/sdb b 640 0 0 8 17 1 1 8
　　/dev/sdc b 640 0 0 8 32 0 0 -
　　/dev/sdc b 640 0 0 8 33 1 1 8
　　/dev/sdd b 640 0 0 8 48 0 0 -
　　/dev/sdd b 640 0 0 8 49 1 1 8
　　#/dev/sde b 640 0 0 8 64 0 0 -
　　#/dev/sde b 640 0 0 8 65 1 1 8
　　#/dev/sdf b 640 0 0 8 80 0 0 -
　　# MD devices
　　/dev/md b 640 0 0 9 0 0 1 4
　　# SPI devices
　　/dev/qspi c 666 0 0 126 0 - - -
　　# Watchdog timer port
　　/dev/watchdog c 644 0 0 10 130 - - -
　　# Input
　　/dev/input/mice c 644 0 0 13 63 - - -
　　/dev/input/event c 644 0 0 13 64 0 1 3