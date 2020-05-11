# Makefile for a library

# Give this object a name so that the output can be traced
OBJECT_NAME = t8

# Set the Root directory of the project tree relative to this makefile
ROOTDIR = ../../..

# Set the include directory for auto-generated files
IOINCDIR = $(SRCDIR)/io

# List all the sub-projects to be rebuilt. e.g. dependent libraries
# Need to include SUBPROJ/UNIT_TESTS before rules to get them included in .phony
UNIT_TESTS = 

SUBPROJ =

# Following are used to clean the subprojects
CLEAN_SUBPROJ = $(SUBPROJ:%=cleanproj.%)

CLEAN_TEST = $(UNIT_TEST:%=cleantest.%)

.PHONY: $(CLEAN_TEST)

TARGET = $(LIBDIR)/libt8.a

INCLUDES = -I. \
           -I$(ROOTDIR)/pmc_shared/inc \
	   -I$(INCDIR) \
	   -I$(SRCDIR) \
	   -I$(IOINCDIR) \
	   -I$(ROOTDIR)/blk/util_global/inc 

# List the source files for making the target
CSOURCES = $(SRCDIR)/t8.c

# Specify top level RDA XML           
RDAFILE = PM20_82_24_4TO1_APB_BRIDGE.xml
RDAXML = $(IOXML_DIR)/$(RDAFILE)
WS_RDAXML = $(PMC_RDA_ROOT)/titan8_6G_lr_serdes_wrapper/tsb/ip/docs/rda/$(RDAFILE)

# Include the base set of rules for the project.
include $(ROOTDIR)/rules.mk

all: subproj $(TARGET)

subproj: $(SUBPROJ)

# Generate and include dependencies. The '-' before include causes it to be bypassed if not available
# on the first iteration
ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEPS)
endif
