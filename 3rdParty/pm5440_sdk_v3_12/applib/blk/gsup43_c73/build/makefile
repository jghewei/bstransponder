# Makefile for a library

# Give this object a name so that the output can be traced
OBJECT_NAME = GSUP43_C73

# Set the Root directory of the project tree relative to this makefile
ROOTDIR = ../../..

# Set the include directory for auto-generated files
IOINCDIR = $(SRCDIR)/io

# List all the sub-projects to be rebuilt. e.g. dependent libraries
# Need to include SUBPROJ/UNIT_TESTS before rules to get them included in .phony
UNIT_TESTS = $(SRCDIR)/unit_test/gsup43_c73_test

SUBPROJ = $(ROOTDIR)/pmc_shared

# Following are used to clean the subprojects
CLEAN_SUBPROJ = $(SUBPROJ:%=cleanproj.%)

CLEAN_TEST = $(UNIT_TEST:%=cleantest.%)

.PHONY: $(CLEAN_TEST)

TARGET = $(LIBDIR)/libgsup43_c73.a

INCLUDES = -I. \
           -I$(ROOTDIR)/pmc_shared/inc \
           -I$(ROOTDIR)/dev/digi/inc \
           -I$(ROOTDIR)/blk/util_global/inc \
	       -I$(INCDIR) \
	       -I$(SRCDIR) \
	       -I$(IOINCDIR)

# List the source files for making the target
CSOURCES = $(SRCDIR)/gsup43_c73.c \
           $(SRCDIR)/gsup43_c73_counters.c \
           $(SRCDIR)/gsup43_c73_status.c

# Specify top level RDA XML           
RDAFILE = pm30_60_120_ecbi_reg.xml
RDAXML = $(IOXML_DIR)/$(RDAFILE)
WS_RDAXML = $(PMC_RDA_ROOT)/gsup43_c73/tsb/ip/docs/rda/$(RDAFILE)

# Include the base set of rules for the project.
include $(ROOTDIR)/rules.mk

# Generate byte aligned xml
# The byte aligned XML is needed because the original TSB xml is uses sequential addressing
# Use 'make gen_byte_xml' to generate the byte aligned xml before calling 'make regenerate_headers'
define gen_byte_xml
    @echo "Generate byte aligned XML"
    $(SCRIPTS_DIR)/align_ecbi_to_byte_aligned_addr.cmd $(RDAXML) $(RDAXML)
endef

gen_byte_xml:
	$(gen_byte_xml)

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
