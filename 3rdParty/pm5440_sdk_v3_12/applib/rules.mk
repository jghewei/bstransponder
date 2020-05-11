# This file contains all the rules for all levels of makefile
export

default: all
.DEFAULT_GOAL := all

# ----- Tools ------------------------------------------------------------------
SED = sed
RM = rm -f

# Notes: -----------------------------------------------------------------------
# Due to gcc linking problems(limitation), please make sure that pmc_shared is 
# the last subproject in SUBPROJ list

# ----- Standard definitions ---------------------------------------------------
# Could use 'export ' in front of all these if problems are seen with shell

INCDIR = ../inc
SRCDIR = ../src
LIBDIR_TOP = $(ROOTDIR)/lib
INCDIR_TOP = $(ROOTDIR)/inc
RELDIR = $(ROOTDIR)/../release
TESTS_DIR = $(ROOTDIR)/tests

ifeq ($(IOINCDIR),)
IOINCDIR = .
endif

ifeq ($(OUTPUT_ROOT_DIR_PREFIX),)
    #$(warning "Build from the bottom level makefile***********************************" )
    OBJDIR = ../obj
    LIBDIR = ../lib
    EXE_DIR = ../bin
    COV_DIR = ../
    COV_COAN_DIR = ../
    #LIBDIR_TOP = $(ROOTDIR)/lib
    #INCDIR_TOP = $(ROOTDIR)/inc

    #LIB_DIRS_ROOT = $(ROOTDIR)
    #LIBS_ROOT = $(ROOTDIR)
else
    #$(warning "Build from the top level makefile*************************************" )
    #OBJDIR = ../obj
    #LIBDIR = ../lib
    OBJDIR = $(patsubst %/build,%,$(subst $(WORKSPACE_ROOT_DIR),$(OUTPUT_ROOT_DIR_PREFIX),$(CURDIR)))/obj
    LIBDIR = $(patsubst %/build,%,$(subst $(WORKSPACE_ROOT_DIR),$(OUTPUT_ROOT_DIR_PREFIX),$(CURDIR)))/lib
    EXE_DIR = $(patsubst %/build,%,$(subst $(WORKSPACE_ROOT_DIR),$(OUTPUT_ROOT_DIR_PREFIX),$(CURDIR)))/bin

    COV_COAN_DIR = $(patsubst %/build,%,$(subst $(WORKSPACE_ROOT_DIR),$(OUTPUT_ROOT_DIR_PREFIX),$(CURDIR)))
    COV_DIR = $(patsubst %/build,%,$(subst $(WORKSPACE_ROOT_DIR),$(OUTPUT_ROOT_DIR_PREFIX)/builds/pmc_comexpress/pmc_device,$(CURDIR)))

    #LIBDIR_TOP = $(OUTPUT_ROOT_DIR_PREFIX)/applib/lib
    #INCDIR_TOP = $(OUTPUT_ROOT_DIR_PREFIX)/applib/inc
endif

SWGO_TESTS=
#define the path to link *.o files for specific sub project, this if statment only fix digi project problem
ifeq ($(OUTPUT_ROOT_DIR_PREFIX),)
    ifeq ($(MAKE_TESTS),YES)
	SWGO_TESTS=$(shell find $(TESTS_DIR) -name "*.o"  \( ! -name main.o \) -type f -print)	
    endif 
    #$(warning "Build from the bottom level makefile***********************************" )
    SUBPROJ_OBJS = $(foreach dir,$(SUBPROJ),$(dir)/obj/*.o)
else
    ifeq ($(MAKE_TESTS),YES)
	SWGO_TESTS=$(shell find $(OUTPUT_ROOT_DIR_PREFIX)/applib/tests -name "*.o"  \( ! -name main.o \) -type f -print)	
    endif 
    #$(warning "Build from the top level makefile*************************************" )
    SUBPROJ_OBJS = $(foreach dir,$(SUBPROJ),$(subst $(ROOTDIR),$(OUTPUT_ROOT_DIR_PREFIX)/applib,$(dir))/obj/*.o)
endif

#-----------------------------------------------------------------------------------------
# Dynamically adding INCLUDES, LIB_DIRS and LIBS for all unit test makefiles (also for examples)
#-----------------------------------------------------------------------------------------
get_includes = $(addsuffix /inc,$(dir))
subproj_dirs := $(SUBPROJ)
subproj_includes := $(foreach dir,$(subproj_dirs),$(get_includes))
Isubproj_includes := $(foreach dir,$(subproj_includes),$(addprefix -I,$(dir)))
INCLUDES += -I. -I$(INCDIR) -I$(SRCDIR)
INCLUDES += $(Isubproj_includes)
ifeq ($(MAKE_TESTS),YES)
    INCLUDES += $(foreach dir,$(shell find $(TESTS_DIR) -name inc -type d -print),$(addprefix -I,$(dir)))
#enable quick assert glag for tests build	
    EXTRA_CFLAGS = -DPMC_EMULATION_QUICK_ASSERT -DUSE_FW_LOG
endif 

get_lib_dirs = $(addsuffix /lib,$(dir))
subproj_dirs := $(SUBPROJ)
#mapping
ifeq ($(OUTPUT_ROOT_DIR_PREFIX),)
    #$(warning "Build from the bottom level makefile***********************************" )
    subproj_lib_dirs := $(foreach dir,$(subproj_dirs),$(get_lib_dirs))
else
    #$(warning "Build from the top level makefile*************************************" )
    original_subproj_lib_dirs := $(foreach dir,$(subproj_dirs),$(get_lib_dirs))
    subproj_lib_dirs := $(foreach dir,$(original_subproj_lib_dirs),\
                             $(subst $(ROOTDIR),$(OUTPUT_ROOT_DIR_PREFIX)/applib,$(dir))\
                         )
endif

#$(warning subproj_lib_dirs = $(subproj_lib_dirs))

#subproj_lib_dirs := $(foreach dir,$(subproj_dirs),$(get_lib_dirs))
Lsubproj_lib_dirs := $(foreach dir,$(subproj_lib_dirs),$(addprefix -L,$(dir)))
LIB_DIRS += -L.
LIB_DIRS += $(Lsubproj_lib_dirs)
#$(warning LIB_DIRS = $(LIB_DIRS))

#get lib names dynamically for fixing example build problem
get_lib_name =$(notdir $(patsubst %/lib,%,$(dir)))
lib_names := $(foreach dir, $(subproj_lib_dirs),$(get_lib_name))
# $(warning *************************************************)
# $(warning)
# $(warning lib_names = $(lib_names))

#get_libs = $(wildcard,$(dir)/*.a)
#subproj_libs := $(foreach dir,$(subproj_lib_dirs),$(wildcard $(dir)/*.a))
#subproj_libs := $(foreach dir,$(subproj_lib_dirs),$(wildcard $(dir)/lib$(get_lib_name).a))
subproj_libs := $(foreach dir,$(subproj_lib_dirs),$(addprefix $(dir)/,lib$(get_lib_name).a))

LIBS += $(subproj_libs)
#$(warning LIBS = $(LIBS))

#-----------------------------------------------------------------------------------
# End of adding INCLUDES, LIB_DIRS and LIBS for all unit test makefiles (also for examples)
#-----------------------------------------------------------------------------------

#----------------------------------------------------------------------------------
# A patch for example build problem, in order to support two models of build (top and bottom)
# we need a mapping of lib path here
# define the path to link *.o files and load libs
#------------------------------------------------------------------------------------
LIB_DIR_PREFIX :=
ifeq ($(OUTPUT_ROOT_DIR_PREFIX),)
    #$(warning "Build from the bottom level makefile***********************************" )
    LIB_DIR_PREFIX = $(ROOTDIR)
else
    #$(warning "Build from the top level makefile*************************************" )
    LIB_DIR_PREFIX = $(OUTPUT_ROOT_DIR_PREFIX)/applib
endif

#$(warning LIB_DIR_PREFIX = $(LIB_DIR_PREFIX))


# directory definitions for the io header file generation.
IO_DIR    = ../src/io

# directory with rda xml files
IOXML_DIR = ../src/ioxml
IOXML_DIR_FILTERED = $(IOXML_DIR)-filtered

# directory with generated parameter definition files.
IOXML_GENERATED_DIR = ../src/ioxml/ioxml_generated

# location of the perl scripts to generate the header files.
SCRIPTS_DIR = $(ROOTDIR)/../scripts

# list of all the perl scripts.
SCRIPTS_SRC = $(patsubst %,$(SCRIPTS_DIR)/%, codegen.pm rda.pm utils.pm codeGen.prl paramset.pm)

# location of the "blocks.xml" file.
BLOCKSXML := $(IOXML_DIR)/blocks.xml
BLOCKSXML_FILTERED := $(IOXML_DIR_FILTERED)/blocks.xml

# Default RDA directory
ifeq ($(strip $(PMC_RDA_ROOT)), )
     PMC_RDA_ROOT = /proj/digi_bby_sw/users/rda
endif


# which perl version to run (requires perl 5.10 or better for XML::SIMPLE
PERL := /usr/local/bin/perl5.10.1

XSLTPROC := /bin/xsltproc

COVERAGE_DIR := $(ROOTDIR)/../coverage
COVERAGE_LCOV_INFO := $(COVERAGE_DIR)/lcov.info

# collect the xml from the pmc_ws and place a copy of the rda xml into the src/ioxml directory.
define collect_xml
    @echo "collect the xml from the pmc_ws and place a copy of the rda xml into the src/ioxml directory."
    $(SCRIPTS_DIR)/do_collect_xml.cmd $(WS_RDAXML) $(IOXML_DIR)
endef

define collect_xml_generic
    @echo "collect the xml from the pmc_ws and place a copy of the rda xml into the src/ioxml directory."
    $(SCRIPTS_DIR)/do_collect_xml_generic.cmd $(WS_RDAXML) $(IOXML_DIR)
endef

define lcov_clean
     @echo "Invoking: LCOV CLEAN "		
     ln -fs  $(COV_COAN_DIR)/src $(COV_DIR)
     ln -fs $(COV_COAN_DIR)/inc $(COV_DIR)
     if test -d $(COV_COAN_DIR)/revb_io; then ln -fs $(COV_COAN_DIR)/revb_io $(COV_DIR); fi 
     rm -rf .lcov
endef

define lcov_capture
    -if test -e .lcov ; then echo "skip"; else echo "lcov capture coverage results."; mkdir -p $(COVERAGE_DIR); touch $(COVERAGE_LCOV_INFO); $(SCRIPTS_DIR)/coverage/lcov --ignore-errors gcov --capture --directory $(COV_DIR)/obj >>  $(COVERAGE_LCOV_INFO); fi
endef



# create a list of blocks in $(IOXML_GENERATED_DIR)/blocks.xml
# after this step the user should copy $(IOXML_GENERATED_DIR)/blocks.xml to $(IOXML_DIR) and edit $(IOXML_DIR)/blocks.xml as needed
define build_block_definitions
    @echo "build_block_definitions - create a list of blocks in $(IOXML_GENERATED_DIR)/blocks.xml"
    @echo "build_block_definitions - after this step the user should copy $(IOXML_GENERATED_DIR)/blocks.xml to $(IOXML_DIR) and edit $(IOXML_DIR)/blocks.xml as needed"
    if test -d $(IOXML_DIR) ; then $(PERL) -I$(SCRIPTS_DIR) $(SCRIPTS_DIR)/codeGen.prl -rda $(RDAXML) -out $(IOXML_GENERATED_DIR) -generate_block ; else echo no ioxml_dir $(IOXML_DIR); fi 
endef

# fix my default block definitions
define fix_block_definitions
    if test -d $(IOXML_DIR) ; then $(XSLTPROC) -o $(BLOCKSXML) $(IOXML_DIR)/blocks.xslt $(IOXML_GENERATED_DIR)/blocks.xml; else echo no ioxml_dir $(IOXML_DIR); fi 
endef

RDAXML_FILTERED = $(subst ioxml,ioxml-filtered,$(RDAXML))

# for each tsb in $(BLOCKSXML), creates the corresponding header definitions in $(IO_DIR)
# BW: removed -dev PM5440 from filter_xml.cmd
define build_io_headers
    @echo "build_io_headers - for each tsb in $(BLOCKSXML), creates the corresponding header definitions in $(IO_DIR) and $(IO_DIR)-int"
    @echo "build_io_headers - **** FOR THIS TO WORK YOU MUST UNZIP /scripts/rda_filtering.zip *****"
    -if test -d $(IOXML_DIR) ; then rm $(IO_DIR)/*.[hc]; else echo no ioxml_dir $(IOXML_DIR); fi 
    @echo "build_io_headers - build $(IOXML_DIR_FILTERED)"
    -if test -d $(IOXML_DIR_FILTERED) ; then rm $(IOXML_DIR_FILTERED)/*; fi
    mkdir -p $(IOXML_DIR_FILTERED)
    -cp $(IOXML_DIR)/* $(IOXML_DIR_FILTERED)/
    -for file in $(IOXML_DIR_FILTERED)/*.xml ; do \
	if ! [[ $$file =~ "blocks.xml" ]] ; then $(PERL) -I$(SCRIPTS_DIR) $(SCRIPTS_DIR)/rda_filtering/methodologies/digi_rda/scripts/filter_xml.cmd -i $$file -o $$file -filt $(SCRIPTS_DIR)/filters.txt -nowarn -dev all -audience external -toolpath $(SCRIPTS_DIR)/rda_filtering/methodologies -keep_attrib 1; fi \
	done
    @echo "build_io_headers - build $(IO_DIR)"
    if test -d $(IOXML_DIR_FILTERED) ; then $(PERL) -I$(SCRIPTS_DIR) $(SCRIPTS_DIR)/codeGen.prl -generate_headers -block $(BLOCKSXML_FILTERED) -rda $(RDAXML_FILTERED) -out $(IO_DIR) $(IOXML_OPTS); else echo no ioxml_dir $(IOXML_DIR_FILTERED); fi 
    @echo "build_io_headers - build $(IO_DIR)-int"
    -if test -d $(IOXML_DIR) ; then rm $(IO_DIR)-int/*.[hc]; else echo no ioxml_dir $(IOXML_DIR); fi 
    if test -d $(IOXML_DIR) ; then $(PERL) -I$(SCRIPTS_DIR) $(SCRIPTS_DIR)/codeGen.prl -generate_headers -block $(BLOCKSXML) -rda $(RDAXML) -extract_reserved -extract_descriptions -out $(IO_DIR)-int $(IOXML_OPTS); else echo no ioxml_dir $(IOXML_DIR); fi 
endef


define build_indirects
    @echo "build_indirects - generates indirect access operations based on $(IOXML_DIR)/indirects.xml."
    @echo "                  Outputs files *_ind.c to $(SRCDIR)"
    if test -d $(IOXML_DIR) ; then $(PERL) $(SCRIPTS_DIR)/indirect/indirect_gen.prl -template $(SCRIPTS_DIR)/indirect/template -xml $(IOXML_DIR)/indirects.xml -output ..; fi
endef

define rm_indirects
    @echo "rm_indirects - removes _ind.c files in $(SRCDIR) and _ind.h files in $(INCDIR)"
    rm -i $(SRCDIR)/*_ind.c
    rm -i $(INCDIR)/*_ind.h
endef

# collect the xml from the pmc_ws and place a copy of the rda xml into the src/ioxml directory.
collect_xml:
	$(collect_xml)

collect_xml_generic:
	$(collect_xml_generic)
    
    
# create a list of blocks in $(IOXML_GENERATED_DIR)/blocks.xml
# after this step the user should copy $(IOXML_GENERATED_DIR)/blocks.xml to $(IOXML_DIR) and edit $(IOXML_DIR)/blocks.xml as needed
build_block_definitions:
	$(build_block_definitions)

fix_block_definitions:
	$(fix_block_definitions)

# for each tsb xml file in $(IOXML_PATCHED_DIR), creates the corresponding header definitions in $(IO_DIR)
build_io_headers:
	$(build_io_headers)

# regenerate_headers rebuilds the io headers for this project and every sub project.
regenerate_headers: subproj
	$(build_io_headers)

fetch_fix_and_generate:
	$(collect_xml)
	$(build_block_definitions)
	$(fix_block_definitions)
	$(build_io_headers)

build_block_fix_and_generate:
	$(build_block_definitions)
	$(fix_block_definitions)
	$(build_io_headers)

fix_and_generate:
	$(fix_block_definitions)
	$(build_io_headers)

# generate indirect access based on $(IOXML_DIR)/indirects.xml
indirects:
	$(build_indirects)

# remove *_ind.c files in $(SRCDIR)
rmindirects:
	$(rm_indirects)

lcov_clean: subproj
	$(lcov_clean)

lcov_capture: subproj
	$(lcov_capture)

# ----- Define OS specific definitions
SWIG = swig

# default compiler processor arch is -m64 (64-bit), you can overwirte it in the command line by PROC_ARCH = -m32 (32-bit)
ifeq ($(PROC_ARCH),)
PROC_ARCH = -m64
BUILD_OPTS += -DPMC_M64
endif

# default compiler is gcc, you can overwrite it in command line by GNU_CC = g++
ifeq ($(GNU_CC),)
GNU_CC = gcc
endif

CC = $(CROSS)$(GNU_CC)
DEP_OPTS = -MM $(INCLUDES)

ifeq ($(OBJDUMP),)
OBJDUMP = objdump -h -w
endif

# ----- User defined variables
# PMC_DEBUG should be "-g" to enable debugging, unset otherwise
# PMC_OPTIMISE any optimise flag supported by gcc -O1, or -O2, unset for none.

# ----- Set the defaults
PMC_OPTIMISE = -O0
PMC_DEBUG    = -ggdb

# ----- DEBUG sets PMC_DEBUG and PMC_OPTIMISE
ifeq ($(strip $(DEBUG)), YES)
    PMC_OPTIMISE = -O0
    PMC_DEBUG    = -g3 -DDEBUG
endif

ifeq ($(strip $(PMC_OPTIMISE)), )
    PMC_OPTIMISE = -O2
    PMC_DEBUG    =
endif

# Eventually, all the release builds would have to be:
# -O2
# - without debugging information
ifeq ($(strip $(RELEASE)), YES)
    PMC_OPTIMISE = -O2 -DPMC_EMULATION_COREDUMP_ASSERT -DUSE_BURST_WRITE -DNO_IO_LOGGING -DLOGGING_DISABLED -DPMC_NO_ATOMIC_FUNCTION_STATS -DNO_CHANGE_SETS
    PMC_DEBUG    = -DDEBUG
else
    # if you're compiling with -O0 force ALWAYS_INLINE to be blank otherwise
    # the code size is gigantic.
    PMC_OPTIMISE += -DALWAYS_INLINE=""
endif

# ----- Define common OS definitions
#set extra options if the compiler is g++
GPLUSPLUS_OPTS =
#ifeq ($(GNU_CC), g++)
#    GPLUSPLUS_OPTS = -x c 
#endif

CC_OPTS = $(GPLUSPLUS_OPTS) -Wall -Werror -Wno-error=unused-variable -ansi -fno-strict-aliasing $(INCLUDES) \
          $(PMC_DEBUG) $(PMC_OPTIMISE) $(PROC_ARCH) $(BUILD_OPTS)
CC_SWIG_OPTS = -Wall -Werror -Wl,--no-undefined -fno-strict-aliasing $(INCLUDES) \
               $(SWIG_LIB_PATH) $(SWIG_LIB_LINK) $(PMC_DEBUG) $(PMC_OPTIMISE) $(PROC_ARCH) $(BUILD_OPTS) $(BUILD_VERSION)
CC_SWIG_OPTS += -DINLINE=inline -DINSIDE_SWIG_COMPILE
SWIG_OPTS = -tcl -w305 -DINSIDE_SWIG_COMPILE

# ----- Add  no_pic ompile switch and define
ifneq ($(PMC_PIC),NO)
CC_OPTS      += -fPIC 
CC_SWIG_OPTS += -fPIC 
endif

# enable coverage
ifeq ($(GCOV), YES)
CC_OPTS      += --coverage
CC_SWIG_OPTS += --coverage
endif  

# ----- Define CODE_ARCH definitions
# Verification is the current default
# BUILD_OPTS = -DPMC_VERIFICATION

# When no CODE_ARCH is specified (typical customer use) default to SIMULATION
# prior to FCA

#mapping make BUILD_TARGET=PMC_VERIFICATION TO CODE_ARCH=PMC_VERIFCATION TARGET_PLATFORM=PMC_SPECMAN
ifeq ($(BUILD_TARGET), PMC_VERIFICATION)
CODE_ARCH = PMC_VERIFICATION
TARGET_PLATFORM = PMC_SPECMAN
$(warning Varible BUILD_TARGET is abandoned. Please use:make CODE_ARCH=PMC_VERIFICATION TARGET_PLATFORM=PMC_SPECMAN )
export WARNING = BUILD_TARGET is abandoned. Please use: make CODE_ARCH=PMC_VERIFICATION TARGET_PLATFORM=PMC_SPECMAN
# VERIFICATION needs different CC_SWIG flags as it has some undefined symbols
CC_SWIG_OPTS = -Wall -Werror -fPIC -fno-strict-aliasing $(INCLUDES) \
               $(PMC_DEBUG) $(PMC_OPTIMISE) $(BUILD_OPTS) $(BUILD_VERSION)
endif

#set default TARGET_PLATFORM for CODE_ARCH=PMC_VERIFICATION if user run the build from the bottom level
ifeq ($(CODE_ARCH), PMC_VERIFICATION)
ifeq ($(TARGET_PLATFORM),)
TARGET_PLATFORM = PMC_SPECMAN
# VERIFICATION needs different CC_SWIG flags as it has some undefined symbols
CC_SWIG_OPTS = -Wall -Werror -fPIC -fno-strict-aliasing $(INCLUDES) \
               $(PMC_DEBUG) $(PMC_OPTIMISE) $(BUILD_OPTS) $(BUILD_VERSION)
endif
endif

ifeq ($(CODE_ARCH), PMC_VERIFICATION)
# VERIFICATION needs different CC_SWIG flags as it has some undefined symbols
CC_SWIG_OPTS = -Wall -Werror -fPIC -fno-strict-aliasing $(INCLUDES) \
               $(PMC_DEBUG) $(PMC_OPTIMISE) $(BUILD_OPTS) $(BUILD_VERSION)
endif

# An error infomation for people still use old varible name BUILD_TARGET, We hope we can remove it finally!
ifneq ($(BUILD_TARGET),)
ifneq ($(BUILD_TARGET), PMC_VERIFICATION)
$(error Varible BUILD_TARGET is abandoned. Please use: CODE_ARCH=[PMC_VERIFICATION | PMC_EMULATION | PMC_DEVICE] )
endif
endif


#set gcc flags for CODE_ARCH
#PMC_DEVICE is the default value for CODE_ARCH
#ifeq ($(CODE_ARCH), )
#BUILD_OPTS += -DPMC_DEVICE
#endif

# if user build from the bottom level, then
# mapping BUILD_TARGET=PMC_VERIFICATION to 
# CODE_ARCH=PMC_VERIFICATION TARGET_PLATFORM=PMC_SPECMAN
# -DPMC_VERIFICATION  -DPMC_SPECMAN
ifeq ($(CODE_ARCH), PMC_VERIFICATION)
BUILD_OPTS += -DPMC_VERIFICATION
# Use the set/get headers from the io-int directory instead of the /io directory
# Substitute /io-int for /io in the INCLUDES in the module makefile
#INCLUDES := $(patsubst %/io,%/io-int,$(INCLUDES))
endif

ifeq ($(CODE_ARCH), PMC_EMULATION)
BUILD_OPTS += -DPMC_EMULATION -DPMC_SS_EMULATION
IOINCDIR = $(ROOTDIR)/dev/digi/revb_io
endif 

ifeq ($(TARGET_PLATFORM), PMC_SW_SIMULATION)
BUILD_OPTS += -DPMC_SW_SIMULATION
BUILD_OPTS += -DDIGI_MPMO_MPMA_UPDATE -DDIGI_MPMO_MPMA_UPDATE_2
BUILD_OPTS += -DDIGI_MPMO_ACT_IN_EPMM -DDIGI_ACT_IN_ENET_PROV -DDIGI_ACT_IN_MAP_PROV
IOINCDIR = $(ROOTDIR)/dev/digi/revb_io
endif

ifeq ($(TARGET_PLATFORM), PMC_SPECMAN)
BUILD_OPTS += -DPMC_SPECMAN
IOINCDIR = $(ROOTDIR)/dev/digi/revb_io-int
endif

ifeq ($(TARGET_PLATFORM), PMC_COMEXPRESS)
BUILD_OPTS += -DPMC_COMEXPRESS
IOINCDIR = $(ROOTDIR)/dev/digi/revb_io
endif

ifeq ($(TARGET_PLATFORM), PMC_CUSTOMER_OS)
BUILD_OPTS += -DPMC_CUSTOMER_OS
IOINCDIR = $(ROOTDIR)/dev/digi/revb_io
endif

ifeq ($(TARGET_PLATFORM), PMC_PALLADIUM)
BUILD_OPTS += -DPALLADIUM
endif

ifeq ($(DIGI_API_ONLY), YES)
BUILD_OPTS += -DDIGI_API_ONLY
endif

ifeq ($(CPB_EMULATION), YES)
BUILD_OPTS += -DCPB_EMULATION
endif

ifeq ($(PMC_SS_EMULATION), YES)
BUILD_OPTS += -DPMC_SS_EMULATION
endif
ifeq ($(MAKE_TESTS),YES)
BUILD_OPTS += -DPMC_TESTS -DUSE_FW_LOG
endif
ifeq ($(DEF_TESTS),YES)
BUILD_OPTS += -DPMC_TESTS -DUSE_FW_LOG
endif

ifeq ($(SNCP),YES)
EXTRA_CFLAGS = -DPMC_EMULATION_COREDUMP_ASSERT -DUSE_BURST_WRITE -O2 -DNO_IO_LOGGING -DLOGGING_DISABLED -DPMC_NO_ATOMIC_FUNCTION_STATS
endif

# Simulate compile using internal headers
# Compile in the same manner as PMC_SW_SIMULATION, but use the internal set/get headers.
ifeq ($(CODE_ARCH), PMC_SW_SIMULATION_INT)
BUILD_OPTS = -DPMC_SW_SIMULATION -DPMC_SW_SIMULATION_INT
# Use the set/get headers from the io-int directory instead of the /io directory
# Substitute /io-int for /io in the INCLUDES in the module makefile
#INCLUDES := $(patsubst %/io,%/io-int,$(INCLUDES))
endif

ifeq ($(CODE_ARCH), mips)
BUILD_OPTS =   $(PMC_OPTIMIZE) -fno-unroll-loops -ansi
BUILD_OPTS +=  --param large-function-growth=400
BUILD_OPTS += -DPMC_CUSTOMER_OS -DNO_CONFIG_CACHE -DLOGGING_DISABLED -DNO_IO_LOGGING -DNO_CONFIG_CACHE -DNO_CHANGE_SETS 
#BUILD_OPTS += -UPMC_VERIFICATION -UPMC_SW_SIMULATION
endif

INCLUDES := -I$(IOINCDIR) $(INCLUDES)

ifneq ($(MAKECMDGOALS), clean)

      ifeq ($(CODE_ARCH),)
          $(error You must define a CODE_ARCH=[PMC_VERIFICATION | PMC_EMULATION | PMC_DEVICE])
      endif

      ifneq ($(CODE_ARCH), PMC_VERIFICATION)

           ifeq ($(TARGET_PLATFORM),)
               $(error You must define a TARGET_PLATFORM=[PMC_SW_SIMULATION | PMC_SPECMAN | PMC_COMEXPRESS | PMC_CUSTOMER_OS | PMC_PALLADIUM ])
           endif

      endif

endif

# Private build version string
ifeq ($(VERSION), )
TARBALL_VERSION = 
ifeq ($(DEBUG), YES)
BUILD_VERSION = -DBUILD_VERSION="Private Build (`whoami`, `date`) - Debug Enabled"
else
BUILD_VERSION = -DBUILD_VERSION="Private Build (`whoami`, `date`)"
endif

# Jenkins build version string
else
SVN_URL_STRING = $(subst http://,,$(SVN_URL))
TARBALL_VERSION = _$(VERSION)
ifeq ($(DEBUG), YES)
BUILD_VERSION = -DBUILD_VERSION="Build $(VERSION) ($(SVN_URL_STRING), Revision $(SVN_REV)) - Debug Enabled"
else
BUILD_VERSION = -DBUILD_VERSION="Build $(VERSION) ($(SVN_URL_STRING), Revision $(SVN_REV))"
endif
endif

ifeq ($(AR),)
AR = ar
endif

ifeq ($(AR_OPTS),)
AR_OPTS = -r
endif

LINK = $(CC)
# Link the math library
LINK_OPTS = -lm $(PROC_ARCH) -Map=$(OBJDIR)/$*.map
LIBS_GCOV = 
ifeq ($(GCOV), YES)
	LIBS_GCOV += -lgcov
endif
ifeq ($(TARGET_PLATFORM), PMC_COMEXPRESS)
	LINK_OPTS += -lpthread
endif
ifeq ($(TARGET_PLATFORM), PMC_SW_SIMULATION)
	LINK_OPTS += -lpthread
endif

# makefile debug information
ifeq ($(SHOW_TRACE_VERBOSE), YES)
$(warning CURDIR = $(CURDIR))
$(warning OBJDIR = $(OBJDIR))
$(warning LIBDIR = $(LIBDIR))
$(warning CODE_ARCH = $(CODE_ARCH))
$(warning TARGET_PLATFORM = $(TARGET_PLATFORM))
$(warning SUBPROJ_OBJS = $(SUBPROJ_OBJS))
$(warning INCLUDES = $(INCLUDES))
$(warning subproj_lib_dirs = $(subproj_lib_dirs))
$(warning LIB_DIRS = $(LIB_DIRS))
$(warning LIBS = $(LIBS))
$(warning LIBS_GCOV = $(LIBS_GCOV))
$(warning LIB_DIR_PREFIX = $(LIB_DIR_PREFIX))
$(warning EXE_DIR = $(EXE_DIR))
endif

# ----- Headers/Footers --------------------------------------------------------
SWIG_HEADER = @echo 'Invoking: SWIG for target: $@'
SWIG_FOOTER = @echo 'Finished: SWIG for target: $@'

SWIG_CC_HEADER = @echo 'Invoking: Creating shared lib with SWIG for target: $@'
SWIG_CC_FOOTER = @echo 'Finished: Creating shared lib with SWIG for target: $@'

CC_HEADER = @echo 'Invoking: Compiler for target: $@'
CC_FOOTER = @echo 'Finished: Compiler for target: $@'

AR_HEADER = @echo 'Invoking: Archiver for target: $@'
AR_FOOTER = @echo 'Finished: Archiver for target: $@'

LINK_HEADER = @echo 'Invoking: Linker for target: $@'
LINK_FOOTER = @echo 'Finished: Linker for target: $@'

DEP_HEADER = @echo 'Invoking: Dependency Builder for target: $@'
DEP_FOOTER = @echo 'Finished: Dependency Builder for target: $@'

REL_HEADER = @echo 'Invoking: Creating release'
REL_FOOTER = @echo 'Finished: Creating release'

DOXYGEN_HEADER = @echo 'Invoking: Creating doxygen'
DOXYGEN_FOOTER = @echo 'Finished: Creating doxygen'


CLEAN_HEADER = @echo 'Invoking: Clean for target: $(OBJECT_NAME)'
CLEAN_FOOTER = @echo 'Finished: Clean for target: $(OBJECT_NAME)'


# ----- Rules ------------------------------------------------------------------
# Declare any phony targets (those which are not really files) so that if a file
# was to be made of the same name the targe won't be skipped. Any sub-projects 
# and sub-tests ($SUBPROJ, $SUBTEST) also have to be phony so they get excuted
# rather than just looking at the directory name (which does exist)
.PHONY: default all clean clean-test lint subproj unit_test $(SUBPROJ) $(UNIT_TESTS) headers

# The following ensures that makefiles don't delete intermediate files (e.g. .o)
.SECONDARY:

# The following maps OBJS and DEPS names that are in the $(OBJDIR) folder
# to the name in the $(SRCDIR). Note that the rules below need to include
# these paths as well for the dependency building.
SWGS=$(subst $(SRCDIR),${SRCDIR}, ${SSOURCES:.i=_swig.c} )
SWGO=$(subst $(SRCDIR),${OBJDIR}, ${SSOURCES:.i=_swig.o} )
OBJS=$(subst $(SRCDIR),${OBJDIR}, ${CSOURCES:.c=.o} ${ASMSOURCES:.mip=.o} )
ASMS=$(subst $(SRCDIR),${OBJDIR}, ${CSOURCES:.c=.s} )
DEPS=$(subst $(SRCDIR),${OBJDIR}, ${CSOURCES:.c=.d} )

.PHONY : $(SRCDIR)/digi_swig.c
# Create _swig.c files from SWIG .i files
# This rule looks for any dependency file changes as well as any header file changes to determine
# whether to rebuild digi_swig.c. This is very broad, but since there isn't a dependency builder
# for digi_swig.i there isn't a simple solution.
$(OBJDIR)/digi_swig.c : $(SRCDIR)/digi.i $(wildcard $(OBJDIR)/*.d) $(wildcard $(SRCDIR)/*.h) $(wildcard $(INCDIR)/*.h)
	$(SWIG_HEADER)
	$(SWIG) $(SWIG_OPTS) $(CFLAGS)  $(EXTRA_CFLAGS) $(INCLUDES) $(BUILD_OPTS) -o $@ $<
	cat $(SRCDIR)/digi_swig_additional.c >> $@
	$(SWIG_FOOTER)

# Create .o object files from C swig files
$(OBJDIR)/%_swig.o : $(OBJDIR)/%_swig.c
	$(CC_HEADER)
	mkdir -p $(OBJDIR)
	$(CC) $(CC_SWIG_OPTS) $(CFLAGS)  $(EXTRA_CFLAGS) -o $@ -c $<
	$(CC_FOOTER)

# Create .o object files from C files
$(OBJDIR)/%.o : $(SRCDIR)/%.c $(OBJDIR)/%.d $(wildcard $(SRCDIR)/*.h) $(wildcard $(INCDIR)/*.h)
	$(CC_HEADER)
	mkdir -p $(OBJDIR)
	$(CC) $(CC_OPTS) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ -c $(abspath $<)
	$(CC_FOOTER)

# Create .d dependencies files from C files
$(OBJDIR)/%.d: $(SRCDIR)/%.c $(wildcard $(SRCDIR)/*.h) $(wildcard $(INCDIR)/*.h)
	$(DEP_HEADER)
	mkdir -p $(OBJDIR)
	$(CC) $(DEP_OPTS) $(CC_OPTS) $(CFLAGS) $< > $@.$$$$; \
	$(SED) 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@;
	$(DEP_FOOTER)

# Create .s assembly files from C files (just used to inspect the assembly language)
$(OBJDIR)/%.s : $(SRCDIR)/%.c
	$(CC_HEADER)
	mkdir -p $(OBJDIR)
	$(CROSS)objdump -hd $(OBJDIR)/$*.o > $@
	$(CC_FOOTER)

# Create .a library files from .o object files
$(LIBDIR)/%.a: $(OBJS)
	$(AR_HEADER)
	mkdir -p $(LIBDIR)
	$(AR) $(AR_OPTS) $@ $(OBJS)
	$(OBJDUMP) $@ > $(OBJDIR)/$*.map
	$(AR_FOOTER)

# Create .so.2 library files from .o object files. The .2 refers to major release 2.nn
$(LIBDIR)/%.so: $(LIBDIR)/%.a
	$(AR_HEADER)
	mkdir -p $(LIBDIR)
	$(CC) $(PROC_ARCH) -shared -Wl,-soname,libdigi.so.2 -o $@.2.$(TARBALL_VERSION) $(OBJS) $(SUBPROJ_OBJS) $(LIBS_GCOV)
	$(AR_FOOTER)

#This TARGET create rule is moved to here from examples/build and unit_test/build makefile
#This rule is used for create executable file from libs
# EXE_ONLY target is used for linking with libdigi 
EXE_ONLY:$(TARGET_EXE)
$(TARGET_EXE): $(OBJS) $(subst .o,.d,$(OBJS)) $(LIBS)
	$(LINK_HEADER)
	mkdir -p $(EXE_DIR)
	$(LINK) $(LINK_OPTS) $(EXTRA_LFLAGS) -o $@ $(OBJS) $(LIB_DIRS) $(LIBS) $(LIBS_GCOV)
	$(LINK_FOOTER)


# Build subprojects of current project		
subproj: $(SUBPROJ)
	@echo "Invoking: Building subprojects of $(OBJECT_NAME)"
	@for folder in $(SUBPROJ); do \
		(echo "Building subproject $$folder"; cd -P $$folder/build && $(MAKE) SHELL=$(SHELL) --no-print-directory $(MAKECMDGOALS)) || exit 1; \
	done
	@echo "Finished: Building subprojects of $(OBJECT_NAME)"


# if building 'assembly' targets, also assemble subprojects (note: assembly targets are merely for inspecting the assembly language)	
assembly: subproj $(ASMS)

# Clean-up. If new intermediate program outputs are generated (e.g. listing files) they
# need to be added to this list.
clean: subproj
	$(CLEAN_HEADER)
	@-rm -rf $(OBJS)
	$(RM) $(TARGET) $(LIBTARGET) $(OBJS) $(DEPS) $(ASMS) $(SWGS) $(SWGO) $(SWIG_TARGET) $(TARGET_EXE)
	@-rm -rf $(OBJDIR)
	$(CLEAN_FOOTER)

unit_test: subproj $(TARGET) $(UNIT_TESTS)
	@echo "Invoking: Building unit_tests of $(OBJECT_NAME)"
	@for folder in $(UNIT_TESTS); do \
		(echo "Building unit_test $$folder"; cd $$folder/build && $(MAKE) SHELL=$(SHELL) --no-print-directory -$(MAKEFLAGS) || exit 1); \
	done
	@echo "Finished: Building unit_tests of $(OBJECT_NAME)"

clean_unit_test: clean $(UNIT_TESTS)
	@echo "Invoking: Cleaning unit_tests of $(OBJECT_NAME)"
	@for folder in $(UNIT_TESTS); do \
		(echo "Cleaning unit_test $$folder"; cd $$folder/build && $(MAKE) SHELL=$(SHELL) --no-print-directory -$(MAKEFLAGS) clean || exit 1); \
	done
	@echo "Finished: Cleaning unit_tests of $(OBJECT_NAME)"

