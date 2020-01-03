////////////////////////////////////////////////////////////////
// am.i
//
// SWIG input for wrapping am library
//
// Language: SWIG input
//
// Patrick J. Fasano
// University of Notre Dame
//
// + 12/30/19 (pjf): Created initial version.
////////////////////////////////////////////////////////////////
%module am
%include "typemaps.i"
%include "exception.i"
%include "std_string.i"
%include "std_vector.i"
%include "std_pair.i"
//%include "numpy.i"

// enable docstrings
%feature("autodoc", "1");

// default exception handler
%exception {
  try {
    $action
  } catch (const std::invalid_argument& e) {
    SWIG_exception(SWIG_ValueError, e.what());
  } catch (const std::exception& e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  }
};

// include headers
%{
#define SWIG_FILE_WITH_INIT
#include "halfint.h"
#include "am.h"
#include "wigner_gsl.h"
#include "racah_reduction.h"
%}

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
  std::string __repr__()
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
  HalfInt __abs__() { return abs(*$self); }
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
// %include "racah_reduction.h"  // DISABLED(pjf) -- contains unsafe assert()
// %include "rme.h"  // DISABLED(pjf) -- contains unsafe assert()

