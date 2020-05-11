cmake_minimum_required(VERSION 3.0)
project(pm5440)

include(DanOsCommon)

#add_compile_options(-rdynamic)     # for backtrace symbol
add_compile_options(-DCODE_ARCH=PMC_DEVICE -DTARGET_PLATFORM=PMC_CUSTOMER_OS -DPMC_CUSTOMER_OS -DPMC_BIG_ENDIAN -DDONT_USE_FW_Q -DUSE_PMC_CTXT_CALLOC -fno-strict-aliasing)

set(SDKROOT ../..)
include_directories(${SDKROOT}/inc)

################################################################################
# Make application
################################################################################
file(GLOB_RECURSE LIB_SOURCES ${SDKROOT}/*.c)
add_library(${PROJECT_NAME} SHARED ${LIB_SOURCES})

#install(TARGETS ${APPLICATION_NAME} DESTINATION usr/local/bin)

