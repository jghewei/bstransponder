# Makefile for a library

# Give this object a name so that the output can be traced
OBJECT_NAME = sfi51_d8_rx_18x

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

TARGET = $(LIBDIR)/libsfi51_d8_rx_18x.a

INCLUDES = -I. \
           -I$(ROOTDIR)/pmc_shared/inc \
	   -I$(INCDIR) \
	   -I$(SRCDIR) \
	   -I$(IOINCDIR) \
	   -I$(ROOTDIR)/blk/util_global/inc \
	   -I$(ROOTDIR)/blk/d8_rx_2x/inc 

# List the source files for making the target
CSOURCES = $(SRCDIR)/sfi51_d8_rx_18x.c

# Specify top level RDA XML           
RDAFILE = pm20_20_08_map.xml
RDAXML = $(IOXML_DIR)/$(RDAFILE)
WS_RDAXML = $(PMC_RDA_ROOT)/digi120_sfi51_rx_18x_glue/tsb/ip/docs/rda/$(RDAFILE)

# Include the base set of rules for the project.
include $(ROOTDIR)/rules.mk

all: subproj $(TARGET)

subproj: $(SUBPROJ)

# Generate and include dependencies. The '-' before include causes it to be bypassed if not available
# on the first iteration
ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEPS)
endif
