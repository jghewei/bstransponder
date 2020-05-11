#-------------------------------------------------------------------------------
#
# Description: This makefile builds from the top level all the sub projects
#              and optionally tests.
#
#-------------------------------------------------------------------------------

# Give this object a name so that the output can be traced
OBJECT_NAME = DIGI_ADPT

# Set the Root directory of the project tree relative to this makefile
ROOTDIR = ../..

# Set the filename of program to be generated
TARGET = $(LIBDIR)/libadapter.a
SWIG_TARGET = $(LIBDIR)/libadpt_swig.so

# List the source files for making the target
CSOURCES = $(wildcard $(SRCDIR)/*.c $(SRCDIR)/*.cpp)


SSOURCES = $(SRCDIR)/digi_adpt.i


SUBPROJ = $(ROOTDIR)/dev/digi\
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
          $(ROOTDIR)/blk/acb_ft \
          $(ROOTDIR)/ss/sifd \
          $(ROOTDIR)/blk/d8_tx_2x \
          $(ROOTDIR)/blk/d8_rx_2x \
          $(ROOTDIR)/blk/t8 \
          $(ROOTDIR)/blk/c8 \
          $(ROOTDIR)/blk/odu_ohp \
          $(ROOTDIR)/blk/odu_rcp \
          $(ROOTDIR)/blk/enet_fege

ifeq ($(ADPT_TEST),YES)
SUBPROJ += $(ROOTDIR)/adapter/test
BUILD_OPTS += -DADPT_TEST
endif

# Add -I to each subproj directory
SUBPROJ_INCLUDES = $(foreach dir,$(SUBPROJ),-I$(dir)/inc)

# Add in any addition include directories to those that may be in rules
INCLUDES = -I. \
           -I$(ROOTDIR)/adapter/inc \
           -I$(ROOTDIR)/inc \
           -I$(ROOTDIR)/dev/digi/src \
           $(SUBPROJ_INCLUDES)

# Add in any additional flags to those in rules
CC_OPTS +=
export CFLAGS
AR_OPTS +=
ifeq ($(TARGET_PLATFORM),PMC_COMEXPRESS)
ADPT_FLAGS += -lpthread
else
ADPT_FLAGS +=
endif

# Include the base set of rules for the project.
include $(ROOTDIR)/rules.mk

.PHONY : all

# Set building conditions for SWIG_TARGET
ifeq ($(LIB_ADPT),YES)
all: subproj $(TARGET) swig
INCLUDES += -I$(ROOTDIR)/../scripts/tcl/inc
SWIG_LIB_PATH = -L$(ROOTDIR)/../scripts/tcl/lib
SWIG_LIB_LINK = -ltcl8.5 -lm
else
all: subproj $(TARGET)
	@echo "COMPLETED: DIGI Adapter Build for CODE_ARCH=$(CODE_ARCH) TARGET_PLATFORM=$(TARGET_PLATFORM)"
endif

#all: $(CSOURCES) $(EXECUTABLE)
# This is to rebuild the target as well as all the tests - CURRENTLY NOT FUNCTIONAL
test: unit_test all

# Create libdigi.a and archive all subproj objects
$(TARGET): $(OBJS) subproj $(SUBPROJ)
	$(AR_HEADER)
	mkdir -p $(LIBDIR)
	$(AR) $(AR_OPTS) $@ $(OBJS) $(SUBPROJ_OBJS)
	$(AR_FOOTER)

swig: $(SWGO) $(OBJDIR)/digi_adpt_swig.c 
	$(SWIG_CC_HEADER)
	$(CC) -shared $(CC_SWIG_OPTS) $(CFLAGS) $(SWGO) $(TARGET) -o $(SWIG_TARGET) $(ADPT_FLAGS)
	$(SWIG_CC_FOOTER)
	@echo "$$WARNING"

$(OBJDIR)/digi_adpt_swig.c : $(SRCDIR)/digi_adpt.i $(wildcard $(OBJDIR)/*.d) $(wildcard $(SRCDIR)/*.h) $(wildcard $(INCDIR)/*.h)
	$(SWIG_HEADER)
	cp $(ROOTDIR)/dev/digi/src/digi.i $(SRCDIR)
	cp $(ROOTDIR)/dev/digi/src/swigpig_accessors.i $(SRCDIR)
	$(SWIG) $(SWIG_OPTS) $(CFLAGS) $(INCLUDES) $(BUILD_OPTS) -o $@ $<
	$(SWIG_FOOTER)


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
