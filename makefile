################################
# project configuration
################################

libname = am

# modules -- header-only
modules_h = wigner_gsl wigner2_gsl

# modules -- header-plus-object 
modules_ho = halfint am

# programs
programs = halfint_test am_test
CC := $(CXX)

CXXFLAGS = -std=c++0x

# set flag for linking to FORTRAN
# need_fortran = 

################################
# common definitions
################################

COMMON_MAKE_DIR ?= .
include $(COMMON_MAKE_DIR)/common.mk

################################
# options and dependencies
################################

# program linking
CC := $(CXX)

# external libraries
LDFLAGS += -L.
LDLIBS +=  -lam -lgsl

