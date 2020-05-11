cmake_minimum_required(VERSION 3.0)
project(goahead)

include(DanOsCommon)

include_directories(/root/mpc8308/linux-2.6.29.6/include)

add_compile_options(-Wno-all -Wno-pointer-to-int-cast)

aux_source_directory(src GO_LIB_SOURCES)

# Define the library
add_library(${PROJECT_NAME} SHARED ${GO_LIB_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC "include")

