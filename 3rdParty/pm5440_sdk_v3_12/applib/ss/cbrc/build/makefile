# Makefile for a library

# Give this object a name so that the output can be traced
OBJECT_NAME = CBRC

# Set the Root directory of the project tree relative to this makefile
ROOTDIR = ../../..

# Set the include directory for auto-generated files
IOINCDIR = $(SRCDIR)/io

# List all the sub-projects to be rebuilt. e.g. dependent libraries
# Need to include SUBPROJ/UNIT_TESTS before rules to get them included in .phony
UNIT_TESTS = $(SRCDIR)/unit_test/cbrc_unit_test \
			 $(SRCDIR)/unit_test/coverage \
			 $(SRCDIR)/unit_test/oam

SUBPROJ =

# Following are used to clean the subprojects
CLEAN_SUBPROJ = $(SUBPROJ:%=cleanproj.%)

CLEAN_TEST = $(UNIT_TEST:%=cleantest.%)

.PHONY: $(CLEAN_TEST)

TARGET = $(LIBDIR)/libcbrc.a

INCLUDES = -I. \
           -I$(ROOTDIR)/pmc_shared/inc \
           -I$(ROOTDIR)/dev/digi/inc \
           -I$(ROOTDIR)/blk/util_global/inc \
           -I$(ROOTDIR)/blk/util_opsa/inc \
           -I$(ROOTDIR)/blk/util_schd/inc \
           -I$(ROOTDIR)/blk/util_gen_db/inc \
	  -I$(ROOTDIR)/blk/util_global/inc \
           -I$(ROOTDIR)/blk/sdh_pmg/inc \
           -I$(ROOTDIR)/blk/cbr_fsgm/inc \
           -I$(ROOTDIR)/blk/cbr_fsgm/src/io\
		   -I$(ROOTDIR)/blk/fc800_pmg/inc   \
		   -I$(ROOTDIR)/blk/fc800_pmg/src/io \
		   -I$(ROOTDIR)/blk/fc1200_pmon/inc   \
		   -I$(ROOTDIR)/blk/fc1200_pmon/src/io \
           -I$(ROOTDIR)/blk/mpma/inc \
           -I$(ROOTDIR)/blk/mpmo/inc \
           -I$(ROOTDIR)/blk/scbs3/inc \
           -I$(ROOTDIR)/blk/cfc/inc \
           -I$(ROOTDIR)/blk/mpma/src \
           -I$(ROOTDIR)/blk/mpma/src/io \
           -I$(ROOTDIR)/blk/mpmo/src \
           -I$(ROOTDIR)/blk/mpmo/src/io \
           -I$(ROOTDIR)/blk/scbs3/src \
           -I$(ROOTDIR)/blk/scbs3/src/io \
           -I$(ROOTDIR)/blk/cfc/src \
           -I$(ROOTDIR)/blk/cfc/src/io \
           -I$(ROOTDIR)/blk/sdh_pmg/src/io \
	       -I$(INCDIR) \
	       -I$(SRCDIR) \
	       -I$(IOINCDIR)

# List the source files for making the target
#CSOURCES = $(SRCDIR)/cbrc.c

CSOURCES = $(SRCDIR)/cbrc.c \
           $(SRCDIR)/mld_prbs_ind.c

# Specify top level RDA XML           
RDAFILE = pm70_82_17_map.xml
RDAXML = $(IOXML_DIR)/$(RDAFILE)
WS_RDAXML = $(PMC_RDA_ROOT)/cbrc/tsb/ip/docs/rda/$(RDAFILE)


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
