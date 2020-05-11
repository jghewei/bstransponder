cmake_minimum_required(VERSION 3.0)
project(HALTest)

include(DanOsCommon)
add_subproject_dir(../)

aux_source_directory(./ APP_SOURCES)
add_executable(${PROJECT_NAME} ${APP_SOURCES})
target_link_libraries(${PROJECT_NAME} HAL)

