# Makefile for a library

# Give this object a name so that the output can be traced
OBJECT_NAME = EXCO3M

# Set the Root directory of the project tree relative to this makefile
ROOTDIR = ../../..

# Set the include directory for auto-generated files
IOINCDIR = $(SRCDIR)/io

# List all the sub-projects to be rebuilt. e.g. dependent libraries
# Need to include SUBPROJ/UNIT_TESTS before rules to get them included in .phony
UNIT_TESTS = $(SRCDIR)/unit_test/exco3m_test

SUBPROJ = $(ROOTDIR)/pmc_shared

# Following are used to clean the subprojects
CLEAN_SUBPROJ = $(SUBPROJ:%=cleanproj.%)

CLEAN_TEST = $(UNIT_TEST:%=cleantest.%)

.PHONY: $(CLEAN_TEST)

TARGET = $(LIBDIR)/libexco3m.a

INCLUDES = -I. \
           -I$(ROOTDIR)/pmc_shared/inc \
           -I$(ROOTDIR)/dev/digi/inc \
           -I$(ROOTDIR)/blk/util_global/inc \
	       -I$(INCDIR) \
	       -I$(SRCDIR) \
	       -I$(IOINCDIR)

# List the source files for making the target
CSOURCES = $(SRCDIR)/exco3m.c \
           $(SRCDIR)/exco3m_status.c

# Specify top level RDA XML           
RDAFILE = pm55_60_69_reg.xml
RDAXML = $(IOXML_DIR)/$(RDAFILE)
WS_RDAXML = $(PMC_RDA_ROOT)/exco3m/docs/rda/$(RDAFILE)

# Include the base set of rules for the project.
include $(ROOTDIR)/rules.mk

all: subproj $(TARGET)

# Generate and include dependencies. The '-' before include causes it to be bypassed if not available
# on the first iteration
ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEPS)
endif
