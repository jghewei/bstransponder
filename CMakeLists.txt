cmake_minimum_required(VERSION 3.0)
project(MXP)

include(DanOsCommon)

add_subproject_dir(DBF)
#add_subproject_dir(EQM)
add_subproject_dir(SFM)
#add_subproject_dir(HAL)
add_subproject_dir(WEB)
add_subproject_dir(APP)
add_subproject_dir(SNMP)

#add_compile_options(-Werror -fno-strict-aliasing -Wno-write-strings)
#add_compile_options(-rdynamic)     # for backtrace symbol
#install(TARGETS ${APPLICATION_NAME} DESTINATION usr/local/bin)

add_custom_target(appfs COMMAND ../xml/build_appfs.sh COMMENT "making Appfs")
add_custom_target(load COMMAND ../xml/build_load.sh COMMENT "making Load" DEPENDS appfs)
add_custom_target(sload COMMAND ../xml/build_sload.sh COMMENT "making sLoad" DEPENDS appfs)
add_custom_target(hload COMMAND ../xml/build_hload.sh COMMENT "making hLoad" DEPENDS appfs)

