#-------------------------------------------------------------------------------
#
# Description: This makefile is only used for building libdig.a 
#              and libdigi_swig.so.
#
#-------------------------------------------------------------------------------
SHELL = /bin/bash
# Give this object a name so that the output can be traced
OBJECT_NAME = DIGI

# Set the Root directory of the project tree relative to this makefile
ROOTDIR = ../../..

# Set the include directory for auto-generated files
IOINCDIR = $(SRCDIR)/io

# Set the filename of program to be generated
TARGET = $(LIBDIR)/libdigi.a
TARGET_SO = $(LIBDIR)/libdigi.so
SWIG_TARGET = $(LIBDIR)/libdigi_swig.so

ifeq ($(PROC_ARCH), -m64)
TARGET_PO = $(LIBDIR)/libdigi.po
endif

#EXECUTABLE=$(OBJDIR)/main.out

# List the source files for making the target
CSOURCES = $(SRCDIR)/digi.c \
	       $(SRCDIR)/digi_otn.c \
	       $(SRCDIR)/digi_dcsu.c \
	       $(SRCDIR)/digi_map.c \
	       $(SRCDIR)/digi_cbr.c \
	       $(SRCDIR)/digi_cbr_fc1200.c \
	       $(SRCDIR)/digi_pgmrclk.c \
	       $(SRCDIR)/digi_serdes.c \
	       $(SRCDIR)/hostmsg.c \
	       $(SRCDIR)/hostmsg_hal.c \
	       $(SRCDIR)/digi_fw.c \
	       $(SRCDIR)/digi_fw_log.c \
	       $(SRCDIR)/digi_enet.c\
	       $(SRCDIR)/digi_reset.c \
	       $(SRCDIR)/digi_opsa.c \
	       $(SRCDIR)/digi_pmon.c \
	       $(SRCDIR)/digi_sifd.c \
	       $(SRCDIR)/digi_util.c \
	       $(SRCDIR)/digi_loopback.c \
	       $(SRCDIR)/digi_timesync.c \
	       $(SRCDIR)/digi_resource.c \
	       $(SRCDIR)/digi_resource_util.c \
	       $(SRCDIR)/digi_cmf.c \
	       $(SRCDIR)/digi_ghao.c \
	       $(SRCDIR)/digi_ghao_line_map.c \
	       $(SRCDIR)/digi_ghao_line_line.c \
	       $(SRCDIR)/digi_ghao_firmware.c \
	       $(SRCDIR)/digi_ghao_helper.c

SSOURCES = $(SRCDIR)/digi.i

# Specify top level RDA XML
RDAFILE = pm5440_map.xml
RDAXML = $(IOXML_DIR)/$(RDAFILE)
WS_RDAXML = $(PMC_RDA_ROOT)/digi/device/ip/docs/rda/$(RDAFILE)

# Generate the object files list from the source files list
# part of rules.mk OBJS = $(SOURCES:.c=.o)

# List all the sub-projects to be rebuilt. e.g. dependent libraries
# Need to include SUBPROJ/UNIT_TESTS before rules to get them included in .phony
# Since this is a long list add alphabetically so it can be checked for
# duplicates.
UNIT_TESTS =
SUBPROJ = \
	  $(ROOTDIR)/blk/acb_ft \
          $(ROOTDIR)/blk/c8 \
          $(ROOTDIR)/blk/cbr_fsgm \
          $(ROOTDIR)/blk/cfc \
          $(ROOTDIR)/blk/cpact_obuf \
          $(ROOTDIR)/blk/cpuif \
          $(ROOTDIR)/blk/d8_rx_2x \
          $(ROOTDIR)/blk/d8_tx_2x \
          $(ROOTDIR)/blk/eclass \
          $(ROOTDIR)/blk/emac \
          $(ROOTDIR)/blk/empei_e \
          $(ROOTDIR)/blk/empei_i \
          $(ROOTDIR)/blk/empei_wrap \
          $(ROOTDIR)/blk/epmm \
          $(ROOTDIR)/blk/etime \
          $(ROOTDIR)/blk/etrans \
          $(ROOTDIR)/blk/exco3m \
          $(ROOTDIR)/blk/exco4m \
          $(ROOTDIR)/blk/expand_ibuf \
          $(ROOTDIR)/blk/fc1200_pmon\
          $(ROOTDIR)/blk/fc800_pmg\
          $(ROOTDIR)/blk/fec_pmon \
          $(ROOTDIR)/blk/gsup43_c73 \
          $(ROOTDIR)/blk/mgmt_fege \
          $(ROOTDIR)/blk/mp_mgen \
          $(ROOTDIR)/blk/mpma \
          $(ROOTDIR)/blk/mpmo \
          $(ROOTDIR)/blk/odtu_dmx \
          $(ROOTDIR)/blk/odtu_dmx/rgmp \
          $(ROOTDIR)/blk/odtu_mux \
          $(ROOTDIR)/blk/odtu_mux/tgmp \
          $(ROOTDIR)/blk/odu_rfrm \
          $(ROOTDIR)/blk/odu_tfrm \
          $(ROOTDIR)/blk/odujat \
          $(ROOTDIR)/blk/oduksc \
          $(ROOTDIR)/blk/ohfs_insert \
          $(ROOTDIR)/blk/ohfs_remove \
          $(ROOTDIR)/blk/otn_prbs \
          $(ROOTDIR)/blk/otu_frm \
          $(ROOTDIR)/blk/rxjc_proc \
          $(ROOTDIR)/blk/s16 \
          $(ROOTDIR)/blk/scbs3 \
          $(ROOTDIR)/blk/sdh_pmg \
          $(ROOTDIR)/blk/sfi51_d8_rx_18x \
          $(ROOTDIR)/blk/sfi51_d8_tx_18x \
          $(ROOTDIR)/blk/sts_sw \
          $(ROOTDIR)/blk/t8 \
          $(ROOTDIR)/blk/txjc_proc \
          $(ROOTDIR)/blk/util_gen_db \
          $(ROOTDIR)/blk/util_global \
          $(ROOTDIR)/blk/util_opsa \
          $(ROOTDIR)/blk/util_schd \
          $(ROOTDIR)/pmc_shared \
          $(ROOTDIR)/ss/cbrc \
          $(ROOTDIR)/ss/coreotn \
          $(ROOTDIR)/ss/cpb \
          $(ROOTDIR)/ss/enet \
          $(ROOTDIR)/ss/lifd \
          $(ROOTDIR)/ss/lineotn \
          $(ROOTDIR)/ss/mapotn \
	  $(ROOTDIR)/ss/sifd \
	  $(ROOTDIR)/blk/odu_rcp \
	  $(ROOTDIR)/blk/enet_fege \
	  $(ROOTDIR)/blk/odu_ohp

# Add -I to each subproj directory
SUBPROJ_INCLUDES = $(foreach dir,$(SUBPROJ),-I$(dir)/inc)

# Add in any addition include directories to those that may be in rules
INCLUDES = -I. \
           -I$(ROOTDIR)/dev/digi/inc \
           $(SUBPROJ_INCLUDES)

# Include the base set of rules for the project.
include $(ROOTDIR)/rules.mk

# Add in any additional flags to those in rules
CC_OPTS +=
export CFLAGS
AR_OPTS +=

.PHONY : all

# Set building conditions for SWIG_TARGET
ifeq ($(TARGET_PLATFORM), PMC_CUSTOMER_OS)
all: $(TARGET) $(TARGET_SO) $(TARGET_PO)
	# @echo "COMPLETED: DIGI SDK Build for CODE_ARCH=$(CODE_ARCH) TARGET_PLATFORM=$(TARGET_PLATFORM)"
else
all: $(TARGET) $(TARGET_SO) $(TARGET_PO)
INCLUDES += -I$(ROOTDIR)/../scripts/tcl/inc
SWIG_LIB_PATH = -L$(ROOTDIR)/../scripts/tcl/lib
SWIG_LIB_LINK = -ltcl8.5 -lm -lpthread
endif

# compile emul_accessors only as part of emulation 
# 2012-09-07: Include in all releases for Apps/PV use of external images
#ifeq ($(CODE_ARCH), PMC_EMULATION)
CSOURCES +=	$(SRCDIR)/pv_accessors.c
#endif

#all: $(CSOURCES) $(EXECUTABLE)
# This is to rebuild the target as well as all the tests - CURRENTLY NOT FUNCTIONAL
test: unit_test all

swig: $(SWGO)
	$(SWIG_CC_HEADER)
	$(CC) -shared $(CC_SWIG_OPTS) $(CFLAGS) $(SWGO) $(SWGO_TESTS) $(TARGET) -o $(SWIG_TARGET)
	$(SWIG_CC_FOOTER)
	@echo "$$WARNING"

install:
	@echo "Copying public *.h and *.a contents into top level folders"
	mkdir -p $(LIBDIR_TOP)
	mkdir -p $(INCDIR_TOP)
	@for folder in $(SUBPROJ); do \
		(echo "Copying subproject public includes $$folder"; cp $$folder/inc/*_api.h $(INCDIR_TOP); ) \
	done
	cp ../inc/*_api.h $(INCDIR_TOP)
	# copy util_global.h for odu_struct_api.h - when coreotn finalized then remove
	cp $(ROOTDIR)/blk/util_global/inc/util_global.h $(INCDIR_TOP)
	cp $(ROOTDIR)/ss/lineotn/inc/lineotn_api.h $(INCDIR_TOP)
	cp $(ROOTDIR)/pmc_shared/inc/*.h $(INCDIR_TOP)
	# copy libdigi.a if it exists - note when making a release it doesn't exist so don't error
	-cp ../lib/libdigi.a $(LIBDIR_TOP)
	@echo "Done - copying public *.h and *.a contents into top level folders"

# Create libdigi.a and archive all subproj objects
$(TARGET): install $(OBJS) subproj $(SUBPROJ)
	$(AR_HEADER)
	mkdir -p $(LIBDIR)
	$(AR) $(AR_OPTS) $@ $(OBJS) $(SUBPROJ_OBJS)
	$(OBJDUMP) $@ > $*.map
	$(AR_FOOTER)

#Create portable object file
$(TARGET_PO): install $(OBJS) subproj $(SUBPROJ)
	$(LINK_HEADER)
	$(LD) -r -X -warn-common -r -o $(TARGET_PO) $(OBJS) $(SUBPROJ_OBJS)
	$(LINK_FOOTER)

# Include dependencies.
# NOTE: A '-' before include causes it to be bypassed if not available
# on the first iteration (without the '-' a "No file found" error will be output on
# the first pass
# NOTE: Consider wrapping in the following (and move to rules.mk) if dependencies
# are delaying ('clean' being the command to exclude)
#          ifnew (${MAKECMDGOALS},clean)
#          ...
#          endif
ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEPS)
# include $(SOURCES:.c=.d)
endif
