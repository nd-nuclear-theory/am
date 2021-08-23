################################################################
# project name
################################################################

project_name := am

################################################################
# modules -- list of directories in which to search 
# for module.mk include files
################################################################

# libraries

# Caution: Order of libraries is important since used also in linking.
# Calling library must precede callee library in listing.  That is,
# the most "basic" libraries must go last in the list (unless, of
# course, they are only template libraries, so nobody needs to link to
# them).

modules += . ../fmt

MAKEFILE_STANDALONE = true

################################################################
# extras -- list of extra files to be included
# in distribution tar file
################################################################

extras := README.md LICENSE.md

################################################################
# additional project-specific make settings and rules
################################################################

# gsl
LDLIBS += -lgsl
LDLIBS += -lgslcblas
CPPFLAGS += -DHAVE_INLINE

################################
# "neighboring" libraries
################################

# put parent directory in include path
#   This allows use of #include<> and relative path for current
#   module header files.  Putting this *before* $(install_prefix)/include insures
#   latest development versions are found before any installed versions.
#
# Note: This doesn't let us find *libraries* in the parent's
# subdirectories, though.  These must be defined separately through

CPPFLAGS += -I..
