#!/usr/bin/env python

"""
setup.py file for am SWIG wrapper
"""

from distutils.core import setup, Extension


am_module = Extension(
    '_am',
    sources=['am_wrap.cpp', 'halfint.cpp', 'am.cpp'],
    libraries=['gsl']
)

setup (name = 'am',
       version = '0.1',
       author      = "Patrick J. Fasano",
       description = """Angular Momentum library""",
       ext_modules = [am_module],
       py_modules = ["am"],
       )
