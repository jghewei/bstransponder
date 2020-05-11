# Makefile for a library

# Give this object a name so that the output can be traced
OBJECT_NAME = SIFD

# Set the Root directory of the project tree relative to this makefile
ROOTDIR = ../../..

# Set the include directory for auto-generated files
IOINCDIR = $(SRCDIR)/io

# List all the sub-projects to be rebuilt. e.g. dependent libraries
# Need to include SUBPROJ/UNIT_TESTS before rules to get them included in .phony
UNIT_TESTS =
#UNIT_TESTS = $(SRCDIR)/unit_test/sifd_unit_test

SUBPROJ =

# Following are used to clean the subprojects
CLEAN_SUBPROJ = $(SUBPROJ:%=cleanproj.%)

CLEAN_TEST = $(UNIT_TEST:%=cleantest.%)

.PHONY: $(CLEAN_TEST)

TARGET = $(LIBDIR)/libsifd.a

INCLUDES = -I. \
           -I$(ROOTDIR)/pmc_shared/inc \
           -I$(ROOTDIR)/blk/util_global/inc \
           -I$(ROOTDIR)/blk/util_opsa/inc \
           -I$(ROOTDIR)/blk/util_schd/inc \
           -I$(ROOTDIR)/blk/util_gen_db/inc \
           -I$(ROOTDIR)/blk/cfc/inc \
           -I$(ROOTDIR)/blk/mpma/inc \
           -I$(ROOTDIR)/blk/mpmo/inc \
           -I$(ROOTDIR)/blk/scbs3/inc \
           -I$(ROOTDIR)/dev/digi/inc \
	   -I$(INCDIR) \
	   -I$(SRCDIR) \
	   -I$(IOINCDIR)

# List the source files for making the target
CSOURCES = $(SRCDIR)/sifd.c  \
           $(SRCDIR)/sifd_ghao.c \

# Specify top level RDA XML
RDAFILE = pm70_82_13_map.xml
RDAXML = $(IOXML_DIR)/$(RDAFILE)
WS_RDAXML = $(PMC_RDA_ROOT)/tsb/ip/docs/rda/$(RDAFILE)

# Include the base set of rules for the project.
include $(ROOTDIR)/rules.mk

all: subproj $(TARGET)

#test: $(TEST)

subproj: $(SUBPROJ)

# Generate and include dependencies. The '-' before include causes it to be bypassed if not available
# on the first iteration
ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEPS)
endif
