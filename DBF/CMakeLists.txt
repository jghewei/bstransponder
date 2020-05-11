cmake_minimum_required(VERSION 3.0)
project(DBF)

include(DanOsCommon)
add_subproject_dir(../COM)
#add_subproject_dir(test)

aux_source_directory(./src LIB_SOURCES)
add_library(${PROJECT_NAME} STATIC ${LIB_SOURCES})
target_link_libraries(${PROJECT_NAME} COM)
target_include_directories(${PROJECT_NAME} PUBLIC "inc")

