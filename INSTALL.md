# am installation guide #

Mark A. Caprio  
Department of Physics, University of Notre Dame
Patrick J. Fasano  
Physics Division, Argonne National Laboratory

+ 09/07/2020 (mac): Created.
+ 05/31/2023 (mac): Expand and update instructions.
+ 08/17/2023 (pjf): Switch to CMake instructions.

----------------------------------------------------------------

# 1. Standalone compilation and installation (optional)

Configure the project using CMake using:

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  % cmake -B build/ .
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Some supporting modules (`gsl` and `fmt`) must be installed or otherwise be made
visible to CMake.

To compile the library itself:

  ~~~~~~~~~~~~~~~~
  % cmake --build build/
  ~~~~~~~~~~~~~~~~

To compile the test codes:

  ~~~~~~~~~~~~~~~~
  % cmake --build build/ -- tests
  ~~~~~~~~~~~~~~~~

These test codes can then be run:

  ~~~~~~~~~~~~~~~~
  % ./build/am_test
  % ./build/halfint_test
  ~~~~~~~~~~~~~~~~

To install the library (here with prefix `~/install`):
  ~~~~~~~~~~~~~~~~
  % cmake --install build/ --prefix ~/install
  ~~~~~~~~~~~~~~~~

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

