# Makefile for a library

# Give this object a name so that the output can be traced
OBJECT_NAME = PMC_SHARED

# Set the Root directory of the project tree relative to this makefile
ROOTDIR = ../..

# List all the sub-projects to be rebuilt. e.g. dependent libraries
# Need to include SUBPROJ/UNIT_TESTS before rules to get them included in .phony
UNIT_TESTS =

SUBPROJ =

# Following are used to clean the subprojects
CLEAN_SUBPROJ = $(SUBPROJ:%=cleanproj.%)

CLEAN_TEST = $(UNIT_TEST:%=cleantest.%)

.PHONY: $(CLEAN_TEST)

TARGET = $(LIBDIR)/libpmc_shared.a

INCLUDES += -I. \
           -I$(ROOTDIR)/pmc_shared/inc \
           -I$(INCDIR)

# List the source files for making the target
CSOURCES = $(SRCDIR)/change_sets.c \
           $(SRCDIR)/config_cache.c \
           $(SRCDIR)/int_cache.c \
           $(SRCDIR)/l1_sys.c \
           $(SRCDIR)/l2_sys.c \
           $(SRCDIR)/l3_sys.c \
           $(SRCDIR)/l4_sys.c \
           $(SRCDIR)/l5_sys.c \
           $(SRCDIR)/pmc_log.c \
           $(SRCDIR)/pmc_handle.c \
           $(SRCDIR)/pmc_sys.c \
           $(SRCDIR)/pmc_ctxt_mgmt.c \
           $(SRCDIR)/pmc_bitarray.c \
           $(SRCDIR)/shadow_registers.c \
           $(SRCDIR)/pmc_htable.c 


# A warning infomation for people still use old varible name BUILD_TARGET, We hope can remove it finally!
ifneq ($(BUILD_TARGET),)
CODE_ARCH = $(BUILD_TARGET)
$(warning Varible BUILD_TARGET is abandoned. Please use:CODE_ARCH=[PMC_VERIFICATION | PMC_COMEXPRESS | PMC_DEVICE] )
export WARNING = BUILD_TARGET is abandoned. Please use:CODE_ARCH=[PMC_VERIFICATION | PMC_COMEXPRESS | PMC_DEVICE]
endif

#ifeq ($(TARGET_PLATFORM), )
#CSOURCES += $(SRCDIR)/pmc_sw_simulation.c
#endif 

ifeq ($(TARGET_PLATFORM), PMC_SW_SIMULATION)
CSOURCES += $(SRCDIR)/pmc_sw_simulation.c
endif 

#need update for PMC_COMEXPRESS
ifeq ($(TARGET_PLATFORM), PMC_COMEXPRESS)
CSOURCES += $(SRCDIR)/pmc_emulation.c
endif 

ifeq ($(TARGET_PLATFORM), PMC_SPECMAN)
#we do not have this source file in SVN
#this file to replace previous pmc_verification.c
CSOURCES += $(SRCDIR)/pmc_verification.c
endif 

ifeq ($(TARGET_PLATFORM), PMC_CUSTOMER_OS)
CSOURCES += $(SRCDIR)/pmc_customer_os.c
endif 

#debug info for pmc_shared
# $(warning in pmc_shared = +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++)
# $(warning CURDIR = $(CURDIR))
# $(warning OBJDIR = $(OBJDIR))
# $(warning LIBDIR = $(LIBDIR))
# $(warning CODE_ARCH = $(CODE_ARCH))
# $(warning TARGET_PLATFORM = $(TARGET_PLATFORM))
# $(warning SUBPROJ_OBJS = $(SUBPROJ_OBJS))
# $(warning INCLUDES = $(INCLUDES))
# $(warning CSOURCES = $(CSOURCES))
# $(warning subproj_lib_dirs = $(subproj_lib_dirs))
# $(warning LIB_DIRS = $(LIB_DIRS))
# $(warning LIBS = $(LIBS))
# $(warning LIB_DIR_PREFIX = $(LIB_DIR_PREFIX))


# Include the base set of rules for the project.
include $(ROOTDIR)/rules.mk

all: subproj $(TARGET)

#test: $(TEST)

subproj: $(SUBPROJ)

#$(SUBTEST):
#	(echo "Building subtest $@"; cd $@/build && $(MAKE) --no-print-directory -$(MAKEFLAGS) || exit 1);
#	$(MAKE) -C $@
#	@echo "Start - Building subprojects"
#	@for folder in $<; do \
#		(echo "Building subproject $$folder"; cd $$folder && $(MAKE) --no-print-directory -$(MAKEFLAGS) || exit 1); \
#	done
#	@echo "Complete - Building subprojects"
#$(SUBPROJ):
#	(echo "Building subproj $@"; cd $@/build && $(MAKE) --no-print-directory -$(MAKEFLAGS) || exit 1);
#test: unit_test
#$(TARGET): subproj $(OBJS) $(USER_OBJS)
#	$(AR_HEADER)
##	mkdir -p ../lib
#	$(AR) $(AR_OPTS) $@ $(OBJS) $(USER_OBJS) $(LIBS)
#	$(AR_FOOTER)
#
#$(CLEAN_SUBPROJ): cleanproj.%:
#	(echo "Cleaning subproj $*"; cd $*/build && $(MAKE) --no-print-directory -$(MAKEFLAGS) clean || exit 1);
##	make -C $* clean
#
#$(CLEAN_TEST): cleantest.%:
#	(echo "Cleaning unit_test $*"; cd $*/build && $(MAKE) --no-print-directory -$(MAKEFLAGS) clean || exit 1);
#	make -C $* clean
#        
#clean: subproj 
#	$(RM) $(TARGET) $(OBJS) $(DEPS)
#	
#clean_test: $(CLEAN_TEST)

# Generate and include dependencies. The '-' before include causes it to be bypassed if not available
# on the first iteration
ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEPS)
endif
