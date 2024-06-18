# am installation guide #

Mark A. Caprio
Department of Physics, University of Notre Dame
Patrick J. Fasano
Physics Division, Argonne National Laboratory

+ 09/07/2020 (mac): Created.
+ 05/31/2023 (mac): Expand and update instructions.
+ 08/17/2023 (pjf): Switch to CMake instructions.
+ 09/30/2023 (mac): Provide explanation of C++ vs. Python installation.
+ 06/18/2024 (mac/keo): Add cloning directions and triage user between Python and C++ installation.

----------------------------------------------------------------

This package serves double duty as a Python module and a C++ library.  Depending
on your intended use of the `am` package, you might need to follow the
installation instructions in either Section 1 or Section 2 below, or both.

(1) If you will be using `am` as a python module, see Section 1.  This includes
the case where you need `am` as a supporting module for some other Python
package, such as `mcscript-ncci` or `mfdnres`.

(2) If you wish to build and/or install `am` as a C++ library, see Section 2.
However, as a C++ library, `am` is often simply used as a submodule of a larger
project (such as `shell`).  In that case, you do not need to install it
separately at all.  The only time you are likely to need to build and/or install
`am` separately is if (a) you are doing development work on `am` and want to be
able to compile it on its own or (b) you want to install `am` as a standalone
library so that you can link to it from other projects without including it as a
submodule.

# 0. Retrieving and installing source

  Change to the directory where you want the repository to be installed,
  e.g.,

  ~~~~~~~~~~~~~~~~
  % cd ~/code
  ~~~~~~~~~~~~~~~~

  Clone the `mfdnres` repository.

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  % git clone https://github.com/nd-nuclear-theory/am.git
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# 1. Installation as python module

For use as a Python module, install using distutils/pip:

   ~~~~~~~~~~~~~~~~
   % pip install --user .
   ~~~~~~~~~~~~~~~~

However, this installation requires compiling C++ code.  If you are on an HPC
cluster, make sure you have loaded the appropriate modules for whatever compiler
you wish to use.

Moreover, the code requires the GSL library.  If GSL is not installed systemwide
(e.g., in `/usr/local`), compilation will fail, with an error such as `fatal
error: gsl/gsl_sf_coupling.h: No such file or directory`.  If you are on an HPC
cluster, you may need to load an appropriate module).  Then you will need to set
`CFLAGS` and/or `LDFLAGS` so that distutils/pip can find GSL, wherever it
happens to be installed, e.g.:

   ~~~~~~~~~~~~~~~~
   % env CFLAGS="-I$GSL_DIR/include -L$GSL_DIR/lib" pip install -v --user .
   ~~~~~~~~~~~~~~~~

If installation was successful, you should then be able to import the `am`
module, from within your Python interpreter, without error:

   ~~~~~~~~~~~~~~~~
   >>> import am
   ~~~~~~~~~~~~~~~~
   
You should also be able to run the test code without generating any errors:

   ~~~~~~~~~~~~~~~~
   % python3 am_test.py
   ~~~~~~~~~~~~~~~~


# 2. Standalone compilation and installation as C++ library

First, some supporting modules (`gsl` and `fmt`) must be installed or otherwise be made
visible to CMake.  For `fmt`:

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  % git clone https://github.com/fmtlib/fmt.git
  % cd fmt
  % cmake -B build/ .
  % cmake --build build/
  % cd ..
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Then, back in the `am` code directory, configure the project using CMake using:

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  % cmake -B build/ .
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

However, if you have not installed the dependencies, but only built them, you
will need to tell CMake where to find the built-but-not-installed libraries:

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  % cmake -B build . \
      -Dfmt_DIR=~/code/fmt/build
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

