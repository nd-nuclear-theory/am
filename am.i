////////////////////////////////////////////////////////////////
// am.i
//
// SWIG input for wrapping am library
//
// Language: SWIG input
//
// SWIG command for generating am_wrap.cpp and am.py:
//
//     > swig -python -c++ -cppext cpp -builtin -fastdispatch -py3 am.i
//
// Patrick J. Fasano
// University of Notre Dame
//
// + 12/30/19 (pjf): Created initial version.
// + 04/10/20 (pjf): Re-enable racah_reduction.h and rme.h after
//   removing assertions.
// + 05/18/20 (pjf): Correctly expose hashing so that dicts work
//   as expected.
////////////////////////////////////////////////////////////////
%module am
%include "typemaps.i"
%include "exception.i"
%include "std_complex.i"
%include "std_pair.i"
%include "std_string.i"
%include "std_vector.i"
//%include "numpy.i"

// enable docstrings
%feature("autodoc", "1");

// default exception handler
%exception {
  try {
    $action
  } catch (const std::invalid_argument& e) {
    SWIG_exception(SWIG_ValueError, e.what());
  } catch (const std::domain_error& e) {
    SWIG_exception(SWIG_ValueError, e.what());
  } catch (const std::exception& e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  }
};

// include headers
%{
#define SWIG_FILE_WITH_INIT
#include <sstream>
#include "halfint.h"
#include "am.h"
#include "wigner_gsl.h"
#include "racah_reduction.h"
#include "rme.h"
%}

// ignore global am constants
%ignore am::kPi;
%ignore am::kSqrt4Pi;
%ignore am::kInvSqrt4Pi;

////////////////////////////////////////////////////////////////
// Set up Python slots for HalfInt, for implementing the Python
// Number Protocol.
//
// See http://www.swig.org/Doc4.0/Python.html#Python_builtin_overloads
// and https://docs.python.org/3/c-api/typeobj.html#c.PyNumberMethods
////////////////////////////////////////////////////////////////

// allow implicit conversions of Python types to HalfInt
%implicitconv HalfInt;

// implement conversion from HalfInt back to Python types
%feature("python:slot", "nb_int", functype="unaryfunc") HalfInt::operator int;
%rename(__int__) HalfInt::operator int;
%ignore HalfInt::operator float;
%feature("python:slot", "nb_float", functype="unaryfunc") HalfInt::operator double;
%rename(__float__) HalfInt::operator double;

// provide string representations
%feature("python:slot", "tp_str", functype="reprfunc") HalfInt::Str;
%rename(__str__) HalfInt::Str;
%feature("python:slot", "tp_repr", functype="reprfunc") HalfInt::__repr__;
%extend HalfInt {
  std::string __repr__() const
  {
    std::ostringstream ss;
    ss << "HalfInt(";
    if ($self->IsInteger())
      ss << $self->TwiceValue()/2;
    else
      ss << $self->TwiceValue() << ",2";
    ss << ")";
    return ss.str();
  }
}

// register abs(const HalfInt&) as provider of absolute
%feature("python:slot", "nb_absolute", functype="unaryfunc") HalfInt::__abs__;
%extend HalfInt {
  HalfInt __abs__() const { return abs(*$self); }
}

// allow hashing of HalfInt for use in dict
%ignore hash_value;
%feature("python:slot", "tp_hash", functype="hashfunc") HalfInt::__hash__;
%extend HalfInt {
  std::size_t __hash__() const { return hash_value(*$self); }
}

// extend HalfInt with arithmetic operations, which will be exposed to Python
//
// NOTE (pjf): Currently SWIG does not handle operators in such a way that
// using right-operators (like __radd__) is simple. This code is left here for
// when SWIG has better support. See SWIG #1694.
// %extend HalfInt {
//   HalfInt operator+(const HalfInt& a) { return (*$self) + a; };
//   HalfInt operator+(const int& a) { return (*$self) + a; };
//   HalfInt operator-(const HalfInt& a) { return (*$self) - a; };
//   HalfInt operator-(const int& a) { return (*$self) - a; };
//   HalfInt operator*(const int& a) { return (*$self) * a; };
// }

////////////////////////////////////////////////////////////////
// extend HalfInt with arithmetic operations, manually setting slots
//
// Here we manually implement Python slot support. We take advantage of SWIG's
// function wrapping. The actual arithmetic operations live in dunder functions
// (e.g. __sadd__, __dadd__) where the prefix (s->"sinistra"->left,
// d->"dextra"->right) indicates which function they're implementing. __sadd__
// implements __add__, while __dadd__ implements __radd__. The functions must
// be declared as "maybecall" so that they return/raise "NotImplemented" for
// unknown Python types.
%pythonmaybecall __sadd__;
%pythonmaybecall __dadd__;
%pythonmaybecall __ssub__;
%pythonmaybecall __dsub__;
%pythonmaybecall __smul__;
%pythonmaybecall __dmul__;
%extend HalfInt {
  HalfInt __sadd__(const HalfInt& a) { return (*$self) + a; }
  HalfInt __sadd__(const int& a)     { return (*$self) + a; }
  HalfInt __dadd__(const HalfInt& a) { return a + (*$self); }
  HalfInt __dadd__(const int& a)     { return a + (*$self); }
  HalfInt __ssub__(const HalfInt& a) { return (*$self) - a; }
  HalfInt __ssub__(const int& a)     { return (*$self) - a; }
  HalfInt __dsub__(const HalfInt& a) { return a - (*$self); }
  HalfInt __dsub__(const int& a)     { return a - (*$self); }
  HalfInt __smul__(const int& a)     { return (*$self) * a; }
  HalfInt __dmul__(const int& a)     { return a * (*$self); }
}

// Here we add dispatch functions, which check the types of the arguments and
// call the appropriate underlying call, i.e. if the first argument is HalfInt
// call __sadd__ while if the second argument is HalfInt call __dadd__.
%feature("python:nb_add", functype="binaryfunc") HalfInt "HalfInt_dispatch_add";
%feature("python:nb_subtract", functype="binaryfunc") HalfInt "HalfInt_dispatch_sub";
%feature("python:nb_multiply", functype="binaryfunc") HalfInt "HalfInt_dispatch_mul";
// We have to forward declare the wrappers which will be generated, since these
// lines get inserted above the actual wrapper code. For functions which have
// overloads, we have to use the binaryfunc closure to re-marshal arguments.
%{
extern "C" {
SWIGINTERN PyObject *_wrap_HalfInt___sadd__(PyObject *self, PyObject *args);
SWIGINTERN PyObject *_wrap_HalfInt___dadd__(PyObject *self, PyObject *args);
SWIGINTERN PyObject *_wrap_HalfInt___ssub__(PyObject *self, PyObject *args);
SWIGINTERN PyObject *_wrap_HalfInt___dsub__(PyObject *self, PyObject *args);
SWIGINTERN PyObject *_wrap_HalfInt___smul__(PyObject *self, PyObject *args);
SWIGINTERN PyObject *_wrap_HalfInt___dmul__(PyObject *self, PyObject *args);
}
PyObject* HalfInt_dispatch_add(PyObject* a, PyObject* b) {
  void *argp1 = 0;
  void *argp2 = 0;
  int res1 = 0;
  int res2 = 0;
  res1 = SWIG_ConvertPtr(a, &argp1,SWIGTYPE_p_HalfInt, 0 |  0 );
  if (SWIG_IsOK(res1)) return SwigPyBuiltin_binaryfunc_closure(_wrap_HalfInt___sadd__, a, b);
  res2 = SWIG_ConvertPtr(b, &argp2,SWIGTYPE_p_HalfInt, 0 |  0 );
  if (SWIG_IsOK(res2)) return SwigPyBuiltin_binaryfunc_closure(_wrap_HalfInt___dadd__, b, a);
  PyErr_Clear();
  Py_INCREF(Py_NotImplemented);
  return Py_NotImplemented;
}
PyObject* HalfInt_dispatch_sub(PyObject* a, PyObject* b) {
  void *argp1 = 0;
  void *argp2 = 0;
  int res1 = 0;
  int res2 = 0;
  res1 = SWIG_ConvertPtr(a, &argp1,SWIGTYPE_p_HalfInt, 0 |  0 );
  if (SWIG_IsOK(res1)) return SwigPyBuiltin_binaryfunc_closure(_wrap_HalfInt___ssub__, a, b);
  res2 = SWIG_ConvertPtr(b, &argp2,SWIGTYPE_p_HalfInt, 0 |  0 );
  if (SWIG_IsOK(res2)) return SwigPyBuiltin_binaryfunc_closure(_wrap_HalfInt___dsub__, b, a);
  PyErr_Clear();
  Py_INCREF(Py_NotImplemented);
  return Py_NotImplemented;
}
PyObject* HalfInt_dispatch_mul(PyObject* a, PyObject* b) {
  void *argp1 = 0;
  void *argp2 = 0;
  int res1 = 0;
  int res2 = 0;
  res1 = SWIG_ConvertPtr(a, &argp1,SWIGTYPE_p_HalfInt, 0 |  0 );
  if (SWIG_IsOK(res1)) return _wrap_HalfInt___smul__(a, b);
  res2 = SWIG_ConvertPtr(b, &argp2,SWIGTYPE_p_HalfInt, 0 |  0 );
  if (SWIG_IsOK(res2)) return _wrap_HalfInt___dmul__(b, a);
  PyErr_Clear();
  Py_INCREF(Py_NotImplemented);
  return Py_NotImplemented;
}
%}
////////////////////////////////////////////////////////////////


// extend HalfInt with comparison operations, which will be exposed to Python
%extend HalfInt {
  bool operator==(const HalfInt& h2) { return (*$self) == h2; };
  bool operator<(const HalfInt& h2) { return (*$self) < h2; };
  bool operator>(const HalfInt& h2) { return (*$self) > h2; };
  bool operator>=(const HalfInt& h2) { return (*$self) >= h2; };
  bool operator<=(const HalfInt& h2) { return (*$self) <= h2; };
  bool operator!=(const HalfInt& h2) { return (*$self) != h2; };
};

// instantiate STL templates for types defined in HalfInt
%template(HalfIntPair) std::pair<HalfInt, HalfInt>;  // HalfInt::pair
%template(HalfIntVector) std::vector<HalfInt>;  // HalfInt::vector

////////////////////////////////////////////////////////////////
// Include header files for am, for parsing by SWIG
////////////////////////////////////////////////////////////////

%include "halfint.h"
%include "am.h"
%include "wigner_gsl.h"
%include "racah_reduction.h"
%include "rme.h"

////////////////////////////////////////////////////////////////
// Instantiate am.h template functions
////////////////////////////////////////////////////////////////

%extend HalfInt {
  %template(HalfInt) HalfInt<int,nullptr>;
  %template(HalfInt) HalfInt<int,int,nullptr,nullptr>;
}

// instantiate STL templates for return types
%template(pairi) std::pair<int,int>;
%template(vectori) std::vector<int>;
namespace am {
%template(ProductAngularMomenta) ProductAngularMomenta<HalfInt,HalfInt,HalfInt,nullptr>;
%template(ProductAngularMomenta) ProductAngularMomenta<HalfInt,int,HalfInt,nullptr>;
%template(ProductAngularMomenta) ProductAngularMomenta<int,HalfInt,HalfInt,nullptr>;
%template(ProductAngularMomenta) ProductAngularMomenta<int,int,int,nullptr>;

%template(ProductAngularMomentumRange) ProductAngularMomentumRange<HalfInt,HalfInt,HalfInt,nullptr>;
%template(ProductAngularMomentumRange) ProductAngularMomentumRange<HalfInt,int,HalfInt,nullptr>;
%template(ProductAngularMomentumRange) ProductAngularMomentumRange<int,HalfInt,HalfInt,nullptr>;
%template(ProductAngularMomentumRange) ProductAngularMomentumRange<int,int,int,nullptr>;

%template(AngularMomentumRangeIntersection)
AngularMomentumRangeIntersection<int,int,int,nullptr>;
%template(AngularMomentumRangeIntersection)
AngularMomentumRangeIntersection<HalfInt,int,HalfInt,nullptr>;
%template(AngularMomentumRangeIntersection)
AngularMomentumRangeIntersection<int,HalfInt,HalfInt,nullptr>;
%template(AngularMomentumRangeIntersection)
AngularMomentumRangeIntersection<HalfInt,HalfInt,HalfInt,nullptr>;
};

