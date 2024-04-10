#!/usr/bin/env python

"""
setup.py file for am SWIG wrapper
"""

from distutils.core import setup, Extension


am_module = Extension(
    '_am',
    sources=['am_wrap.cpp'],
    libraries=['gsl', 'gslcblas', 'm'],
    extra_compile_args=['-std=c++17', '-g', '-O2', '-DAM_EXCEPTIONS']
)

setup (name = 'am',
       version = '0.1',
       author      = "Patrick J. Fasano",
       description = """Angular Momentum library""",
       ext_modules = [am_module],
       py_modules = ["am"],
       )
