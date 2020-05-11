#add_compile_options(-D__TARGET__ -mcpu=e300c3 -m32 -fno-omit-frame-pointer -pipe)
add_compile_options(-D__TARGET__ -mcpu=e300c3 -m32 -pipe)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR ppc)
#set(CMAKE_SYSROOT $ENV{WORKING_DIR}/$ENV{REPO_3RDPARTY}/Coriant/SDK/WindRiver/wrlplat/tlabqoriq50_small/build/rootfs/sdk/sysroots/tlabqoriqsmall-wrs-linux)

set(TOOLCHAIN /opt/freescale/usr/local/gcc-4.1.78-eglibc-2.5.78-1/powerpc-e300c3-linux-gnu/bin)

LIST(APPEND CMAKE_PROGRAM_PATH ${TOOLCHAIN})

set(CMAKE_C_COMPILER ${TOOLCHAIN}/powerpc-e300c3-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN}/powerpc-e300c3-linux-gnu-g++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY OTHER)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE OTHER)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

