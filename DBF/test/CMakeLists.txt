cmake_minimum_required(VERSION 3.0)
project(DBFTest)

include(DanOsCommon)
add_subproject_dir(../)

aux_source_directory(./ APP_SOURCES)
add_executable(${PROJECT_NAME} ${APP_SOURCES})
target_link_libraries(${PROJECT_NAME} DBF)

#add_compile_options(-Werror -fno-strict-aliasing -Wno-write-strings)
#add_compile_options(-rdynamic)     # for backtrace symbol
#include_directories($ENV{DanOS_DIR}/3rdParty/goahead_v3_6_3/include)
#if ( CMAKE_SYSTEM_PROCESSOR STREQUAL "ppc" )
#link_directories($ENV{DanOS_DIR}/3rdParty/Tcl/libppc)
#link_directories($ENV{DanOS_DIR}/3rdParty/goahead_v3_6_3/libppc)
#link_directories($ENV{DanOS_DIR}/3rdParty/pm5440_sdk_v3_12/libppc)
#else()
#link_directories($ENV{DanOS_DIR}/3rdParty/Tcl/libx86)
#link_directories($ENV{DanOS_DIR}/3rdParty/goahead_v3_6_3/libx86)
#endif()
#target_link_libraries(${APPLICATION_NAME}  m tcl pthread pm5440 goahead)
