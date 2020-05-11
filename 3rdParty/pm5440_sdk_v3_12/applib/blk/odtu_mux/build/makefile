# Makefile for a library

# Give this object a name so that the output can be traced
OBJECT_NAME = ODTU_MUX

# Set the Root directory of the project tree relative to this makefile
ROOTDIR = ../../..

# Set the include directory for auto-generated files
IOINCDIR = $(SRCDIR)/io

# List all the sub-projects to be rebuilt. e.g. dependent libraries
# Need to include SUBPROJ/UNIT_TESTS before rules to get them included in .phony
UNIT_TESTS = $(SRCDIR)/unit_test/odtu_mux_unit_test

SUBPROJ = $(ROOTDIR)/blk/ohfs_insert \
          $(ROOTDIR)/blk/txjc_proc \
          $(ROOTDIR)/blk/expand_ibuf \
          $(ROOTDIR)/blk/sts_sw \
          $(ROOTDIR)/blk/mp_mgen \
          $(ROOTDIR)/blk/cfc \
          $(ROOTDIR)/blk/util_global \
          $(ROOTDIR)/blk/util_opsa \
          $(ROOTDIR)/blk/odtu_mux/tgmp \
          $(ROOTDIR)/blk/util_gen_db \
          $(ROOTDIR)/pmc_shared

# Following are used to clean the subprojects
CLEAN_SUBPROJ = $(SUBPROJ:%=cleanproj.%)

CLEAN_TEST = $(UNIT_TEST:%=cleantest.%)

.PHONY: $(CLEAN_TEST)

TARGET = $(LIBDIR)/libodtu_mux.a

INCLUDES = -I. \
           -I$(ROOTDIR)/pmc_shared/inc \
           -I$(ROOTDIR)/dev/digi/inc \
           -I$(ROOTDIR)/blk/ohfs_insert/inc \
           -I$(ROOTDIR)/blk/ohfs_insert/src \
           -I$(ROOTDIR)/blk/ohfs_insert/src/io \
           -I$(ROOTDIR)/blk/txjc_proc/inc \
           -I$(ROOTDIR)/blk/txjc_proc/src \
           -I$(ROOTDIR)/blk/txjc_proc/src/io \
           -I$(ROOTDIR)/blk/expand_ibuf/inc \
           -I$(ROOTDIR)/blk/expand_ibuf/src \
           -I$(ROOTDIR)/blk/expand_ibuf/src/io-int \
           -I$(ROOTDIR)/blk/expand_ibuf/src/io \
           -I$(ROOTDIR)/blk/sts_sw/inc \
           -I$(ROOTDIR)/blk/sts_sw/src \
           -I$(ROOTDIR)/blk/sts_sw/src/io \
           -I$(ROOTDIR)/blk/mp_mgen/inc \
           -I$(ROOTDIR)/blk/mp_mgen/src \
           -I$(ROOTDIR)/blk/mp_mgen/src/io \
           -I$(ROOTDIR)/blk/cfc/inc \
           -I$(ROOTDIR)/blk/cfc/src \
           -I$(ROOTDIR)/blk/cfc/src/io \
           -I$(ROOTDIR)/blk/odtu_mux/tgmp/inc \
           -I$(ROOTDIR)/blk/odtu_mux/tgmp/src \
           -I$(ROOTDIR)/blk/odtu_mux/tgmp/src/io \
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
CSOURCES = $(SRCDIR)/odtu_mux.c

# Specify top level RDA XML
RDAFILE = pm55_60_17_map.xml
RDAXML = $(IOXML_DIR)/$(RDAFILE)
WS_RDAXML = $(PMC_RDA_ROOT)/odtu_mux/tsb/ip/docs/rda/$(RDAFILE)

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
