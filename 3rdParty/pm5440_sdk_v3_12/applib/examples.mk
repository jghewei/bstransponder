# This file contains all the common rules for examples and digi top unit tests

#-----------------------------------------------------------------------------------------
# Dynamically adding INCLUDES, LIB_DIRS and LIBS for all unit test makefiles (also for examples)
#-----------------------------------------------------------------------------------------
ifeq ($(INCLUDE_COMMON_INC),)
INCLUDES +=-I. \
        -I$(ROOTDIR)/dev/digi/src\
        -I$(ROOTDIR)/inc
endif
