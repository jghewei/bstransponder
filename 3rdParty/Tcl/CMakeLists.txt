cmake_minimum_required(VERSION 3.0)
project(tcl)

include(DanOsCommon)

add_compile_options(-Wno-all -Wno-pointer-to-int-cast -DTCL_MEM_DEBUG)

aux_source_directory(src TCL_LIB_SOURCES)

# Define the library
add_library(${PROJECT_NAME} STATIC ${TCL_LIB_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC "include")

