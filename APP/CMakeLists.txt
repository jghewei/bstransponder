cmake_minimum_required(VERSION 3.0)
project(APP)

include(DanOsCommon)

add_subproject_dir(../DBF)
add_subproject_dir(../HAL)
add_subproject_dir(../SFM)
add_subproject_dir(../WEB)
add_subproject_dir(../SNMP)

aux_source_directory(./ APP_SOURCES)
add_executable(${PROJECT_NAME} ${APP_SOURCES})
target_link_libraries(${PROJECT_NAME} tcl DBF HAL SFM WEB SNMP COM)

