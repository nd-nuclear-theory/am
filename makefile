################################
# project configuration
################################

libname = am

# modules -- header-only
modules_h = wigner_gsl wigner2_gsl

# modules -- header-plus-object 
modules_ho = halfint am

# programs
programs = am_test
CC := $(CXX)

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
LDLIBS +=  -lhalfint -lgsl

# dependencies
angular_momentum.o: angular_momentum.h 

