cmake_minimum_required(VERSION 3.0)
project(HAL)

include(DanOsCommon)

add_subproject_dir(../COM)
add_subproject_dir(../DBF)
#add_subproject_dir(test)

aux_source_directory(./src LIB_SOURCES)

if ( CMAKE_SYSTEM_PROCESSOR STREQUAL "ppc" )
	include_directories(./drv)
	include_directories($ENV{DanOS_DIR}/3rdParty/pm5440_sdk_v3_12/applib/inc)
	link_directories($ENV{DanOS_DIR}/3rdParty/pm5440_sdk_v3_12/libppc)
	aux_source_directory(./drv DRV_SOURCES)
	add_library(${PROJECT_NAME} STATIC ${LIB_SOURCES} ${DRV_SOURCES})
	target_link_libraries(${PROJECT_NAME} COM DBF m pthread pm5440)
else()
	add_library(${PROJECT_NAME} STATIC ${LIB_SOURCES})
	target_link_libraries(${PROJECT_NAME} COM DBF)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC "inc")
