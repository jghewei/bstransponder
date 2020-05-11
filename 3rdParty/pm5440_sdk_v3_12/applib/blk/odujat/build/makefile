# Makefile for a library

# Give this object a name so that the output can be traced
OBJECT_NAME = ODUJAT

# Set the Root directory of the project tree relative to this makefile
ROOTDIR = ../../..

# Set the include directory for auto-generated files
IOINCDIR = $(SRCDIR)/io

# List all the sub-projects to be rebuilt. e.g. dependent libraries
# Need to include SUBPROJ/UNIT_TESTS before rules to get them included in .phony
UNIT_TESTS =
#UNIT_TESTS = $(SRCDIR)/unit_test/oduksc_unit_test

SUBPROJ = $(ROOTDIR)/blk/oduksc \
          $(ROOTDIR)/blk/scbs3 \
          $(ROOTDIR)/blk/cfc \
          $(ROOTDIR)/pmc_shared

# Following are used to clean the subprojects
CLEAN_SUBPROJ = $(SUBPROJ:%=cleanproj.%)

CLEAN_TEST = $(UNIT_TEST:%=cleantest.%)

.PHONY: $(CLEAN_TEST)

TARGET = $(LIBDIR)/libodujat.a

INCLUDES = -I. \
           -I$(ROOTDIR)/pmc_shared/inc \
           -I$(ROOTDIR)/dev/digi/inc \
	       -I$(INCDIR) \
	       -I$(SRCDIR) \
	       -I$(IOINCDIR) \
	       -I$(ROOTDIR)/blk/oduksc/inc \
           -I$(ROOTDIR)/blk/oduksc/src \
           -I$(ROOTDIR)/blk/oduksc/src/io \
           -I$(ROOTDIR)/blk/scbs3/inc \
           -I$(ROOTDIR)/blk/scbs3/src \
           -I$(ROOTDIR)/blk/scbs3/src/io \
           -I$(ROOTDIR)/blk/cfc/inc \
           -I$(ROOTDIR)/blk/cfc/src \
           -I$(ROOTDIR)/blk/cfc/src/io \
           -I$(ROOTDIR)/blk/util_global/inc \
           -I$(ROOTDIR)/blk/util_gen_db/inc \
           -I$(ROOTDIR)/blk/util_schd/inc 

# List the source files for making the target
CSOURCES = $(SRCDIR)/odujat.c

# Specify top level RDA XML           
RDAFILE = pm20_15_101_map.xml
RDAXML = $(IOXML_DIR)/$(RDAFILE)
WS_RDAXML = $(PMC_RDA_ROOT)/odujat/odujat/tsb/ip/docs/rda/$(RDAFILE)

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
