cmake_minimum_required(VERSION 3.0)
project(dtach)

include(DanOsCommon)

include_directories(/root/mpc8308/linux-2.6.29.6/include)
include_directories(src)

add_compile_options(-Wno-all -Wno-pointer-to-int-cast)

aux_source_directory(src SOURCES)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} util)

