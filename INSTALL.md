# am installation guide #

Mark A. Caprio, Patrick J. Fasano  
Department of Physics, University of Notre Dame

+ 09/07/2020 (mac): Created.
+ 05/31/2023 (mac): Expand and update instructions.

----------------------------------------------------------------

# 1. Standalone compilation (optional)

Standalone compilation is only useful if you are doing testing or development.
Otherwise, ignore these instructions!

For standalone compilation, use the `ndmakefile` module, e.g.:

  ~~~~~~~~~~~~~~~~
  % ln -s ../ndmakefile/makefile
  % ln -s ../ndconfig/config-gnu.mk config.mk
  % make all
  ~~~~~~~~~~~~~~~~

Some supporting modules (such as `ndmakefile` and `fmt`) will also need to be
cloned.  In the `fmt` library, be sure to check out the `submodule` branch.

To run a test code:

  ~~~~~~~~~~~~~~~~
  % make programs-test
  % ./am_test
  ~~~~~~~~~~~~~~~~

The package also provides CMake support.

# 2. Installation as python module

For use as a Python module, install using distutils/pip:

   ~~~~~~~~~~~~~~~~
   % pip install --user .
   ~~~~~~~~~~~~~~~~

However, if GSL is not installed systemwide (e.g., in `/usr/local`), note that
you may first need to set `CFLAGS` and/or `LDFLAGS` so that distutils/pip can
find GSL, e.g.:

   ~~~~~~~~~~~~~~~~
   % env CFLAGS="-I$GSL_DIR/include -L$GSL_DIR/lib" pip install -v --user .
   ~~~~~~~~~~~~~~~~

If installation was successful, you should be able to import the `am` module without error:

   ~~~~~~~~~~~~~~~~
   >>> import am
   ~~~~~~~~~~~~~~~~
   
You should also be able to run the test code without generating any errors:

   ~~~~~~~~~~~~~~~~
   % python3 am_test.py
   ~~~~~~~~~~~~~~~~

