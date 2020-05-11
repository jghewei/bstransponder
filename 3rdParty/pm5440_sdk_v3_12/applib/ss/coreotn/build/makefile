# Makefile for a library

# Give this object a name so that the output can be traced
OBJECT_NAME = COREOTN

# Set the Root directory of the project tree relative to this makefile
ROOTDIR = ../../..

# Set the include directory for auto-generated files
IOINCDIR = $(SRCDIR)/io

# List all the sub-projects to be rebuilt. e.g. dependent libraries
# Need to include SUBPROJ/UNIT_TESTS before rules to get them included in .phony
UNIT_TESTS = $(SRCDIR)/unit_test/coreotn_unit_test

SUBPROJ =

# Following are used to clean the subprojects
CLEAN_SUBPROJ = $(SUBPROJ:%=cleanproj.%)

CLEAN_TEST = $(UNIT_TEST:%=cleantest.%)

.PHONY: $(CLEAN_TEST)

TARGET = $(LIBDIR)/libcoreotn.a

INCLUDES = -I. \
           -I$(ROOTDIR)/pmc_shared/inc \
           -I$(ROOTDIR)/dev/digi/inc \
           -I$(ROOTDIR)/blk/util_global/inc \
           -I$(ROOTDIR)/blk/util_global/src \
           -I$(ROOTDIR)/ss/cpb/inc \
           -I$(ROOTDIR)/ss/cpb/src \
           -I$(ROOTDIR)/ss/cpb/src/io \
           -I$(ROOTDIR)/blk/odu_tfrm/inc \
           -I$(ROOTDIR)/blk/odu_tfrm/src \
           -I$(ROOTDIR)/blk/odu_tfrm/src/io \
           -I$(ROOTDIR)/blk/odu_rfrm/inc \
           -I$(ROOTDIR)/blk/odu_rfrm/src \
           -I$(ROOTDIR)/blk/odu_rfrm/src/io \
           -I$(ROOTDIR)/blk/odtu_mux/inc \
           -I$(ROOTDIR)/blk/odtu_mux/src \
           -I$(ROOTDIR)/blk/odtu_mux/src/io \
           -I$(ROOTDIR)/blk/odtu_dmx/inc \
           -I$(ROOTDIR)/blk/odtu_dmx/src \
           -I$(ROOTDIR)/blk/odtu_dmx/src/io \
           -I$(ROOTDIR)/blk/ohfs_remove/inc \
           -I$(ROOTDIR)/blk/ohfs_remove/src \
           -I$(ROOTDIR)/blk/ohfs_remove/src/io \
           -I$(ROOTDIR)/blk/ohfs_insert/inc \
           -I$(ROOTDIR)/blk/ohfs_insert/src \
           -I$(ROOTDIR)/blk/ohfs_insert/src/io \
           -I$(ROOTDIR)/blk/rxjc_proc/inc \
           -I$(ROOTDIR)/blk/rxjc_proc/src \
           -I$(ROOTDIR)/blk/rxjc_proc/src/io \
           -I$(ROOTDIR)/blk/txjc_proc/inc \
           -I$(ROOTDIR)/blk/txjc_proc/src \
           -I$(ROOTDIR)/blk/txjc_proc/src/io \
           -I$(ROOTDIR)/blk/cpact_obuf/inc \
           -I$(ROOTDIR)/blk/cpact_obuf/src \
           -I$(ROOTDIR)/blk/cpact_obuf/src/io \
           -I$(ROOTDIR)/blk/expand_ibuf/inc \
           -I$(ROOTDIR)/blk/expand_ibuf/src \
           -I$(ROOTDIR)/blk/expand_ibuf/src/io \
           -I$(ROOTDIR)/blk/mp_mgen/inc \
           -I$(ROOTDIR)/blk/mp_mgen/src \
           -I$(ROOTDIR)/blk/mp_mgen/src/io \
           -I$(ROOTDIR)/blk/sts_sw/inc \
           -I$(ROOTDIR)/blk/sts_sw/src \
           -I$(ROOTDIR)/blk/sts_sw/src/io \
           -I$(ROOTDIR)/blk/odtu_dmx/rgmp/inc \
           -I$(ROOTDIR)/blk/odtu_dmx/rgmp/src \
           -I$(ROOTDIR)/blk/odtu_dmx/rgmp/src/io \
           -I$(ROOTDIR)/blk/odtu_mux/tgmp/inc \
           -I$(ROOTDIR)/blk/odtu_mux/tgmp/src \
           -I$(ROOTDIR)/blk/odtu_mux/tgmp/src/io \
           -I$(ROOTDIR)/blk/oduksc/inc \
           -I$(ROOTDIR)/blk/oduksc/src \
           -I$(ROOTDIR)/blk/oduksc/src/io \
           -I$(ROOTDIR)/blk/odujat/inc \
           -I$(ROOTDIR)/blk/odujat/src \
           -I$(ROOTDIR)/blk/odujat/src/io \
           -I$(ROOTDIR)/blk/mpmo/inc \
           -I$(ROOTDIR)/blk/mpmo/src \
           -I$(ROOTDIR)/blk/mpmo/src/io \
           -I$(ROOTDIR)/blk/mpma/inc \
           -I$(ROOTDIR)/blk/mpma/src \
           -I$(ROOTDIR)/blk/mpma/src/io \
           -I$(ROOTDIR)/blk/util_opsa/inc \
           -I$(ROOTDIR)/blk/util_opsa/src \
           -I$(ROOTDIR)/blk/cfc/inc \
           -I$(ROOTDIR)/blk/cfc/src \
           -I$(ROOTDIR)/blk/cfc/src/io \
           -I$(ROOTDIR)/blk/scbs3/inc \
           -I$(ROOTDIR)/blk/scbs3/src \
           -I$(ROOTDIR)/blk/scbs3/src/io \
           -I$(ROOTDIR)/blk/util_schd/inc \
           -I$(ROOTDIR)/blk/util_schd/src \
           -I$(ROOTDIR)/blk/util_gen_db/inc \
           -I$(ROOTDIR)/blk/util_gen_db/src \
           -I$(ROOTDIR)/blk/odu_rcp/inc \
           -I$(ROOTDIR)/blk/enet_fege/inc \
           -I$(ROOTDIR)/blk/odu_ohp/inc \
	   -I$(INCDIR) \
	   -I$(SRCDIR) \
	   -I$(IOINCDIR)

# List the source files for making the target
CSOURCES = $(SRCDIR)/coreotn.c \
	   			 $(SRCDIR)/odu_struct.c \
	   			 $(SRCDIR)/coreotn_ghao.c

# Specify top level RDA XML
RDAFILE = pm55_82_09_ad_map.xml
RDAXML = $(IOXML_DIR)/$(RDAFILE)
WS_RDAXML = $(PMC_RDA_ROOT)/coreotn/tsb/ip/docs/rda/$(RDAFILE)

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
