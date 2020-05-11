# Makefile for a library

# Give this object a name so that the output can be traced
OBJECT_NAME = ENET

# Set the Root directory of the project tree relative to this makefile
ROOTDIR = ../../..

# Set the io header directory for auto-generated files
IOINCDIR = $(SRCDIR)/io

# List all the sub-projects to be rebuilt. e.g. dependent libraries
# Need to include SUBPROJ/UNIT_TESTS before rules to get them included in .phony
UNIT_TESTS = $(SRCDIR)/unit_test/enet_unit_test

SUBPROJ = 

# Following are used to clean the subprojects
CLEAN_SUBPROJ = $(SUBPROJ:%=cleanproj.%)

CLEAN_TEST = $(UNIT_TEST:%=cleantest.%)

.PHONY: $(CLEAN_TEST)

TARGET = $(LIBDIR)/libenet.a

INCLUDES = -I. \
           -I$(ROOTDIR)/pmc_shared/inc \
           -I$(ROOTDIR)/blk/emac/inc \
           -I$(ROOTDIR)/blk/etime/inc \
           -I$(ROOTDIR)/blk/epmm/inc \
           -I$(ROOTDIR)/blk/mpma/inc \
           -I$(ROOTDIR)/blk/mpmo/inc \
           -I$(ROOTDIR)/blk/gsup43_c73/inc \
           -I$(ROOTDIR)/blk/scbs3/inc \
           -I$(ROOTDIR)/blk/empei_i/inc \
           -I$(ROOTDIR)/blk/empei_e/inc \
           -I$(ROOTDIR)/blk/eclass/inc \
           -I$(ROOTDIR)/blk/etrans/inc \
           -I$(ROOTDIR)/blk/exco3m/inc \
           -I$(ROOTDIR)/blk/exco4m/inc \
           -I$(ROOTDIR)/blk/cfc/inc \
           -I$(ROOTDIR)/blk/util_global/inc \
           -I$(ROOTDIR)/blk/util_gen_db/inc	\
           -I$(ROOTDIR)/blk/util_opsa/inc \
           -I$(ROOTDIR)/blk/util_schd/inc \
	   -I$(INCDIR) \
	   -I$(SRCDIR) \
	   -I$(IOINCDIR) 
   

# List the source files for making the target
CSOURCES = $(SRCDIR)/enet.c

# Specify top level RDA XML           
RDAFILE = lw70_60_94_line_map.xml
RDAXML = $(IOXML_DIR)/$(RDAFILE)
WS_RDAXML = $(PMC_RDA_ROOT)/enet/tsbs/enet_top_regs/docs/rda/$(RDAFILE)

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
