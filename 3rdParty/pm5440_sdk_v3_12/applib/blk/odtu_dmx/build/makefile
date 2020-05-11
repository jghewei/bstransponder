# Makefile for a library

# Give this object a name so that the output can be traced
OBJECT_NAME = ODTU_DMX

# Set the Root directory of the project tree relative to this makefile
ROOTDIR = ../../..

# Set the include directory for auto-generated files
IOINCDIR = $(SRCDIR)/io

# List all the sub-projects to be rebuilt. e.g. dependent libraries
# Need to include SUBPROJ/UNIT_TESTS before rules to get them included in .phony
UNIT_TESTS = $(SRCDIR)/unit_test/odtu_dmx_unit_test

SUBPROJ = $(ROOTDIR)/blk/ohfs_remove \
          $(ROOTDIR)/blk/rxjc_proc \
          $(ROOTDIR)/blk/cpact_obuf \
          $(ROOTDIR)/blk/sts_sw \
          $(ROOTDIR)/blk/mp_mgen \
          $(ROOTDIR)/blk/cfc \
          $(ROOTDIR)/blk/util_global \
          $(ROOTDIR)/blk/util_opsa \
          $(ROOTDIR)/blk/util_gen_db \
          $(ROOTDIR)/blk/odtu_dmx/rgmp \
          $(ROOTDIR)/pmc_shared

# Following are used to clean the subprojects
CLEAN_SUBPROJ = $(SUBPROJ:%=cleanproj.%)

CLEAN_TEST = $(UNIT_TEST:%=cleantest.%)

.PHONY: $(CLEAN_TEST)

TARGET = $(LIBDIR)/libodtu_dmx.a

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
           -I$(ROOTDIR)/blk/sts_sw/inc \
           -I$(ROOTDIR)/blk/sts_sw/src \
           -I$(ROOTDIR)/blk/sts_sw/src/io \
           -I$(ROOTDIR)/blk/mp_mgen/inc \
           -I$(ROOTDIR)/blk/mp_mgen/src \
           -I$(ROOTDIR)/blk/mp_mgen/src/io \
           -I$(ROOTDIR)/blk/cfc/inc \
           -I$(ROOTDIR)/blk/cfc/src \
           -I$(ROOTDIR)/blk/cfc/src/io \
           -I$(ROOTDIR)/blk/odtu_dmx/rgmp/inc \
           -I$(ROOTDIR)/blk/odtu_dmx/rgmp/src \
           -I$(ROOTDIR)/blk/odtu_dmx/rgmp/src/io \
           -I$(ROOTDIR)/blk/util_global/inc \
           -I$(ROOTDIR)/blk/util_global/src \
           -I$(ROOTDIR)/blk/util_opsa/inc \
           -I$(ROOTDIR)/blk/util_opsa/src \
           -I$(ROOTDIR)/blk/util_opsa/src/io \
           -I$(ROOTDIR)/blk/util_gen_db/inc \
           -I$(ROOTDIR)/blk/util_gen_db/src \
           -I$(INCDIR) \
	       -I$(SRCDIR) \
	       -I$(IOINCDIR)

# List the source files for making the target
CSOURCES = $(SRCDIR)/odtu_dmx.c 

# Specify top level RDA XML
RDAFILE = pm55_60_18_map.xml
RDAXML = $(IOXML_DIR)/$(RDAFILE)
WS_RDAXML = $(PMC_RDA_ROOT)/odtu_dmx/tsb/ip/docs/rda/$(RDAFILE)

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
