# Makefile for a library

# Give this object a name so that the output can be traced
OBJECT_NAME = RGMP

# Set the Root directory of the project tree relative to this makefile
ROOTDIR = ../../../../

# Set the include directory for auto-generated files
IOINCDIR = $(SRCDIR)/io

# List all the sub-projects to be rebuilt. e.g. dependent libraries
# Need to include SUBPROJ/UNIT_TESTS before rules to get them included in .phony
UNIT_TESTS = $(SRCDIR)/unit_test/rgmp_unit_test

SUBPROJ = $(ROOTDIR)/blk/ohfs_remove \
          $(ROOTDIR)/blk/rxjc_proc \
          $(ROOTDIR)/blk/cpact_obuf \
          $(ROOTDIR)/blk/mp_mgen \
          $(ROOTDIR)/blk/cfc \
          $(ROOTDIR)/blk/util_gen_db \
          $(ROOTDIR)/pmc_shared

# Following are used to clean the subprojects
CLEAN_SUBPROJ = $(SUBPROJ:%=cleanproj.%)

CLEAN_TEST = $(UNIT_TEST:%=cleantest.%)

.PHONY: $(CLEAN_TEST)

TARGET = $(LIBDIR)/librgmp.a

INCLUDES = -I. \
           -I$(ROOTDIR)/pmc_shared/inc \
           -I$(ROOTDIR)/dev/digi/inc \
           -I$(ROOTDIR)/blk/ohfs_remove/inc \
           -I$(ROOTDIR)/blk/ohfs_remove/src \
           -I$(ROOTDIR)/blk/ohfs_remove/src/io \
           -I$(ROOTDIR)/blk/rxjc_proc/inc \
           -I$(ROOTDIR)/blk/rxjc_proc/src \
           -I$(ROOTDIR)/blk/rxjc_proc/src/io \
           -I$(ROOTDIR)/blk/cpact_obuf/inc \
           -I$(ROOTDIR)/blk/cpact_obuf/src \
           -I$(ROOTDIR)/blk/cpact_obuf/src/io \
           -I$(ROOTDIR)/blk/mp_mgen/inc \
           -I$(ROOTDIR)/blk/mp_mgen/src \
           -I$(ROOTDIR)/blk/mp_mgen/src/io \
           -I$(ROOTDIR)/blk/cfc/inc \
           -I$(ROOTDIR)/blk/cfc/src \
           -I$(ROOTDIR)/blk/cfc/src/io \
           -I$(ROOTDIR)/blk/util_gen_db/inc \
           -I$(ROOTDIR)/blk/util_gen_db/src \
	       -I$(INCDIR) \
	       -I$(SRCDIR) \
	       -I$(IOINCDIR) \
	       -I$(ROOTDIR)/blk/util_global/inc

# List the source files for making the target
CSOURCES = $(SRCDIR)/rgmp.c

# Specify top level RDA XML
RDAXML = $(IOXML_DIR)/pm55_60_22_map.xml
#WS_RDAXML = ../../../../../../pmc_ws/odtu_dmx/odtu_dmx_pm55_60_18_a/tsb/ip/docs/rda/pm55_60_18_map.xml

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
