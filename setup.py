#!/usr/bin/env python

"""
setup.py file for am SWIG wrapper
"""

from distutils.core import setup, Extension


am_module = Extension(
    '_am',
    sources=['python/am_wrap.cpp'],
    libraries=['gsl', 'gslcblas', 'm'],
    extra_compile_args=['-std=c++17', '-g', '-O2', '-DAM_EXCEPTIONS', '-I.']
)

# Package install debugging
# 05/28/25 (mac/seb)
#
# From commit c1d5e4, had python file python/am.py, and py_modules =
# ["am"]. This worked fine under python 3.10 with --user install.  It fails
# under python 3.12.3 with virtual environment.  Note that resulting am.py is site-packages/python/am.py.
#
# Setting py_modules = ["python/am"] leads to apparently successful install.
# Under python 3.10 --user install, resulting files are
# site-packages/am/python/am.py and site-packages/python/am.py, and package
# imports without complaining but is empty.  Under python 3.12.3, resulting
# files are site-packages/am.py and site-packages/python/am.py, and package
# works.
#
# Relocating the contents of the python subdirectory to top level works under
# python 3.12.3 (seb's original workaround) but breaks build under python 3.10:
#
#   x86_64-linux-gnu-gcc ... -c python/am_wrap.cpp ...
#   cc1plus: fatal error: python/am_wrap.cpp: No such file or directory
#
# So fix consistent with both versions seems to be to keep original file
# locations and py_modules = ["am"], but put symlinks to the files undder python
# in the top directory (ugh!).
#
# Then, under python 3.10 --user install, resulting file is just
# site-packages/am.py, and package works.  Under python 3.12.3, resulting files
# are site-packages/am.py and site-packages/python/am.py, and package works.

setup (name = 'am',
       version = '0.1',
       author      = "Patrick J. Fasano",
       description = """Angular Momentum library""",
       ext_modules = [am_module],
       py_modules = ["am"],
       )
