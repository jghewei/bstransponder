cmake_minimum_required(VERSION 3.0)

#the overall "sysroot" to search for libs/packages, added to the normally searched roots
#if ( CMAKE_SYSTEM_PROCESSOR STREQUAL "ppc")
#    set(CMAKE_FIND_ROOT_PATH "$ENV{WORKING_DIR}/$ENV{REPO_3RDPARTY}/OEM/Libraries/tlabqoriq50_small")
#else()
#    set(CMAKE_FIND_ROOT_PATH "$ENV{WORKING_DIR}/$ENV{REPO_3RDPARTY}/OEM/Libraries/x86_64_EL7")
#endif()

########################################################################################################################
###### BOOST CONFIGS (use findboost)
########################################################################################################################
#set(BOOST_INCLUDEDIR "$ENV{WORKING_DIR}/$ENV{REPO_3RDPARTY}/OEM/Libraries/boost/boost_1.59.0/include")

#IF ( CMAKE_SYSTEM_PROCESSOR STREQUAL "ppc")
#    set(BOOST_LIBRARYDIR "$ENV{WORKING_DIR}/$ENV{REPO_3RDPARTY}/OEM/Libraries/boost/boost_1.59.0/lib_ppc_32bits")
#else()
#    set(BOOST_LIBRARYDIR "$ENV{WORKING_DIR}/$ENV{REPO_3RDPARTY}/OEM/Libraries/boost/boost_1.59.0/lib_x86_64bits")
#endif()

##set(Boost_USE_STATIC_LIBS   ON)
#set(Boost_USE_MULTITHREADED ON)


########################################################################################################################
###### Use CCACHE if available
########################################################################################################################
find_program(CCACHE_FOUND ccache)

if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)


########################################################################################################################
###### COMMON C and CPP FLAGS
########################################################################################################################
set(CMAKE_C_FLAGS "-Wall")
#set(CMAKE_CXX_FLAGS "-Wall -std=c++0x -std=gnu++0x")
set(CMAKE_CXX_FLAGS "-Wall -Wno-write-strings")    # gcc 4.1.2 doesn't support std 11

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

IF ( CMAKE_SYSTEM_PROCESSOR STREQUAL "ppc")
    set( CMAKE_SKIP_BUILD_RPATH true )
endif()


########################################################################################################################
###### CMAKE usefull variables
########################################################################################################################
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RESULTS_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/reports")

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/app/root")
	#NOTE: this is mainly for device target appfs build, but work for x86 "make install" too.
	#	you can override this for x86, but NEVER override this for devices, or the final image build fails


########################################################################################################################
###### FUNCTION to add subprojects even if not in "tree"
########################################################################################################################
function(add_subproject_dir src_dir)

    set(EXTRA_ARG "")
    foreach(extra_arg ${ARGN})
        if ( "${extra_arg}" STREQUAL "EXCLUDE_FROM_ALL" )
            set(EXTRA_ARG ${extra_arg})
        endif()
    endforeach()

    get_filename_component(abs_src_dir "${src_dir}" ABSOLUTE)

    #test if the depended felder is in base srouce folder first
    string(LENGTH "${CMAKE_SOURCE_DIR}" wdir_len)
    string(SUBSTRING "${abs_src_dir}" 0 ${wdir_len} src_dir_lead)
    if( "${src_dir_lead}" STREQUAL "${CMAKE_SOURCE_DIR}" )
        string(SUBSTRING "${abs_src_dir}" ${wdir_len} 1 src_dir_rela)
        if ( "${src_dir_rela}" STREQUAL "/" )
             math(EXPR wdir_len "${wdir_len}+1")
        endif()
        string(SUBSTRING "${abs_src_dir}" ${wdir_len} -1 src_dir_rela)
    else()
        string(LENGTH "$ENV{WORKING_DIR}" wdir_len)
        string(SUBSTRING "${abs_src_dir}" 0 ${wdir_len} src_dir_lead)
        if( "${src_dir_lead}" STREQUAL "$ENV{WORKING_DIR}" )
            string(SUBSTRING "${abs_src_dir}" ${wdir_len} 1 src_dir_rela)
            if ( "${src_dir_rela}" STREQUAL "/" )
                 math(EXPR wdir_len "${wdir_len}+1")
            endif()
            string(SUBSTRING "${abs_src_dir}" ${wdir_len} -1 src_dir_rela)
        endif()
    endif()

    if ( src_dir_rela )
        string(REPLACE "/" "_" src_dir_id ${src_dir_rela})
        if ( NOT ( "1" STREQUAL "$ENV{${src_dir_id}_defined}" ) )
            set(ENV{${src_dir_id}_defined} 1)
            add_subdirectory("${src_dir}" "${CMAKE_BINARY_DIR}/${src_dir_rela}" ${EXTRA_ARG})
        endif()
    endif()
endfunction(add_subproject_dir)

include_directories($ENV{DanOS_DIR}/3rdParty/Tcl/include)
include_directories($ENV{DanOS_DIR}/3rdParty/pm5440_sdk_v3_12/applib/inc)
include_directories($ENV{DanOS_DIR}/3rdParty/goahead_v3_6_3/include)
include_directories($ENV{DanOS_DIR}/3rdParty/net-snmp-5.7.3/build_${CMAKE_SYSTEM_PROCESSOR}/include)
#include_directories(/root/net-snmp-5.7.3/build_${CMAKE_SYSTEM_PROCESSOR}/include)

link_directories($ENV{DanOS_DIR}/3rdParty/Tcl/lib${CMAKE_SYSTEM_PROCESSOR})
link_directories($ENV{DanOS_DIR}/3rdParty/goahead_v3_6_3/lib${CMAKE_SYSTEM_PROCESSOR})
link_directories($ENV{DanOS_DIR}/3rdParty/pm5440_sdk_v3_12/lib${CMAKE_SYSTEM_PROCESSOR})
link_directories($ENV{DanOS_DIR}/3rdParty/net-snmp-5.7.3/build_${CMAKE_SYSTEM_PROCESSOR}/lib)
#link_directories(/root/net-snmp-5.7.3/build_${CMAKE_SYSTEM_PROCESSOR}/lib)

