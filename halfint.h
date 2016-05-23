/****************************************************************
  halfint.h                       

  Defines arithmetic type HalfInt which stores integer or half-integer values,
  as needed, e.g., for angular momentum quantum numbers.

  Language: C++11

  Ke Cai, Veerle Hellemans, Mark A. Caprio
  University of Notre Dame
                                  
  Created by Ke Cai on 6/18/10.   
  10/04/10 stylistic changes (vh): 
    (1) removed "using namespace std" in Halfint.h and added to Halfint.cpp
    (2) copy constructor & copy assignment operator don't need to be defined : 
        the default copy constructor & assignment operator provide memberwise
        copy & assignment
    (3) changed arguments passed by value to arguments passed by const reference
        for various functions
    (4) getnum() and display() made const member functions
  11/27/10+ (mac) full overhaul of functionality plus stylistic changes:
    Goals:
      - Make HalfInt behave as much like a built-in arithmetic type as possible
      - Eliminate redundant features
    - Remove member function hat() in favor of global function hat()
    - Remove member function absolute() in favor of global function abs()
    - Remove set() member function as redundant to assignment operator
    - Remove maximum() member function as redundant to standard algorithm max()
    - Implement full set of unary operators +, -, ++, --
    - Implement full set of arithmetic assignment operators += and -=
    - Implement stream output operator << for HalfInt
    - Change binary arithmetic operators + and - to global functions
        for aesthetics and to allow use with automatic integer->HalfInt 
        conversion, and reimplement in terms of += and -=
    - Remove static constant definitions zero and one, since are redundant to
      integers 0 and 1 (due to automatic conversions from
      int to HalfInt)
    - Rename data member numerator to twice_value_, with accessor twice_value().
      The old name "numerator" was ambiguous.  It assumed
      implicitly that we always meant a numerator relative to
      a "denominator" of 2 (not 1!).
    - Inline all member functions
    - Pull out angular momentum related functions to angular_momentum.h
  12/8/10 (mac):
    - Rename value accessors to TwiceValue() and DValue()
  12/15/10 (mac):
    - Add value accessor IValue() which gives *truncated* integer value
  12/24/10 (mac):
    - Eliminate member function display() as redundant to stream output 
    - Fix sign error just introduced to abs(HalfInt)
  2/16/11 (mac):
    - Add nonmember versions of accessor functions
  2/21/11 (mac):
    - Replace exception generation with exit()
  2/22/11 (mac):
    - Fix operator<< to return output stream
  3/7/16 (mac):
    - Add integer typecast operator.
  3/8/16 (mac):
    - Update indentation to emacs standard.
    - Add string conversion member function.
    - Add pair and vector convenience types.
    - Add float() and double() conversion operators.
  5/15/16 (mac):
    - Add hash_value definition for HalfInt.
  5/23/16 (mac):
    - Remove deprecated IValue and DValue accessors and 
      nonmember accessors.
****************************************************************/

#ifndef HALFINT_H_
#define HALFINT_H_

#include <cassert>
#include <cmath> // for sqrt
#include <cstdlib>
#include <functional>  // for hash
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class HalfInt
{

 public:

  ////////////////////////////////////////////////////////////////
  // typedefs
  ////////////////////////////////////////////////////////////////

  // convenience types
  typedef std::pair<HalfInt,HalfInt> pair;  // used by am
  typedef std::vector<HalfInt> vector;  // used by am

  ////////////////////////////////////////////////////////////////
  // constructors
  ////////////////////////////////////////////////////////////////

  // note: Copy constructor is synthesized copy constructor 
  // since only data member needs copying

  // default constructor: initializes value to zero
  HalfInt() : twice_value_(0) {}; 

  // conversion from integer
  // EX: HalfInt(1) constructs 2/2, HalfInt(2) constructs 4/2
  HalfInt(int);

  // construct from numerator and denominator
  // EX: HalfInt(1,2) constructs 1/2, HalfInt(2, 2) constructs 1,
  //     HalfInt(2,1) constructs 4/2
  HalfInt(int, int);

  ////////////////////////////////////////////////////////////////
  // accessors and conversion operators
  ////////////////////////////////////////////////////////////////

  // The accessors are used also to provide nonmenber "accessor"
  // functions by the same name, to permit more natural mathematical
  // notation, e.g., IsInteger(h).

  int TwiceValue() const
  // return twice value as type int
  {
    return twice_value_;
  };

  bool IsInteger() const
  // Test for integer value.
  {
    return !(twice_value_%2);
  }

  // int IValue() const
  // // return value as type int, by truncation
  // //
  // // This is the "safe" version with test for integer value, as
  // // opposed to the conversion operator int().
  // //
  // // DEPRECATED in favor of conversion operator, except in cases where
  // // safety check is desired.  But, even there, an
  // // assert(h.IsInteger()) would likely be preferable.
  // {
  //   if ((twice_value_ % 2) != 0)
  //     {
  //       std::cerr << "HalfInt::IValue called for nonintegral HalfInt value" << std::endl;
  //       std::exit(EXIT_FAILURE);
  //     }
  //   return twice_value_/2;
  // };

  explicit operator int() const 
  // conversion to int by truncation
  //
  // See also: http://stackoverflow.com/questions/4824278/c-defining-a-type-cast
  //
  // Only permit explicit conversion, else an expression of type
  // <int> + <HalfInt> is an ambiguous overload of operator +.
  // Requires -std=c++0x.
  {
    return twice_value_/2;
  }

  ////////////////////////////////////////////////////////////////
  // floating point accessors and conversion operators
  ////////////////////////////////////////////////////////////////

  // double DValue() const
  // // return value as type double
  // //
  // // DEPRECATED in favor of conversion operator.
  // {
  //   return static_cast<double>(twice_value_)/2;
  // };

  explicit operator float() const 
  // conversion operators for float
  //
  // See also: http://stackoverflow.com/questions/4824278/c-defining-a-type-cast
  //
  // Only permit explicit conversion, else an expression of type
  // <int> + <HalfInt> is an ambiguous overload of operator +.
  // Requires -std=c++0x.
  //
  // Note made pre-C++11: Defining an operator double(), for
  // automatic conversion to double, naively seems desirable.
  // However, this would lead to ambiguous overloads, in
  // combination of automatic conversion from int to HalfInt,
  // e.g., 1 + 1/2 -> 2/2 + 1/2 -> 3/2 vs. 1 + 1/2 -> 1.0 + 0.5
  // -> 1.5.
  {
    return static_cast<float>(twice_value_)/2;
  }

  explicit operator double() const 
  // conversion operators for float
  //
  // See notes for float().
  {
    return static_cast<double>(twice_value_)/2;
  }


  ////////////////////////////////////////////////////////////////
  // arithmetic assignment operators
  ////////////////////////////////////////////////////////////////

  // note: operator = is synthesized assignment operator 

  // operators return reference to allow for chained (a+=b)+=c structures
  HalfInt& operator +=(const HalfInt&);
  HalfInt& operator -=(const HalfInt&);

  ////////////////////////////////////////////////////////////////
  // unary arithmetic operators
  ////////////////////////////////////////////////////////////////

  HalfInt operator +() const;
  HalfInt operator -() const;

  HalfInt& operator ++(); // prefix ++, increments then returns reference
  HalfInt operator ++(int); // postfix ++, returns value then increments
  HalfInt& operator --(); // prefix --, increments then returns reference
  HalfInt operator --(int); // postfix --, returns value then increments

  ////////////////////////////////////////////////////////////////
  // string conversion
  ////////////////////////////////////////////////////////////////
  
  std::string Str() const;

  ////////////////////////////////////////////////////////////////
  // data
  ////////////////////////////////////////////////////////////////

 private:
  // contains one member, which stores twice the actual value, since this is
  // guaranteed to be an integer.
  int twice_value_;

};

////////////////////////////////////////////////////////////////
// constructors
////////////////////////////////////////////////////////////////

inline HalfInt::HalfInt(int value)
{
  twice_value_ = 2*value;
}

inline HalfInt::HalfInt(int numerator, int denominator)
{
  if (!((denominator==1)||(denominator==2)))
    {
      std::cerr << "HalfInt constructed with denominator not 1 or 2" << std::endl;
      std::exit(EXIT_FAILURE);
    }
  twice_value_ = (2/denominator)*numerator;
}

////////////////////////////////////////////////////////////////
// arithmetic assignment operators
////////////////////////////////////////////////////////////////

inline HalfInt& HalfInt::operator +=(const HalfInt& b)
{
  twice_value_ += b.twice_value_;
  return *this;
}

inline HalfInt& HalfInt::operator -=(const HalfInt& b)
{
  twice_value_ -= b.twice_value_;
  return *this;
}

////////////////////////////////////////////////////////////////
// nonmember accessor functions
////////////////////////////////////////////////////////////////

// Compare use of conversion operators or member accessor functions.

inline int TwiceValue(const HalfInt& h)
{
  return (h.TwiceValue());
}

inline bool IsInteger(const HalfInt& h)
{
  return (h.IsInteger());
}

// inline int IValue(const HalfInt& h)
// // DEPRECATED in favor of C++11 conversion operator.
// {
//   return (h.IValue());
// }

// inline double DValue(const HalfInt& h) 
// // DEPRECATED in favor of C++11 conversion operator.
// {
//   return (h.DValue());
// }

////////////////////////////////////////////////////////////////
// unary arithmetic operators
////////////////////////////////////////////////////////////////

inline HalfInt HalfInt::operator + () const 
{
  return *this;
};

inline HalfInt HalfInt::operator - () const 
{
  HalfInt negative;
  negative.twice_value_ = -twice_value_;
  return negative;
};

inline HalfInt& HalfInt::operator ++ ()
{
  twice_value_ += 2;	
  return *this;
}

inline HalfInt HalfInt::operator ++ (int)
{
  HalfInt h = *this;
  twice_value_ += 2;
  return h;
}

inline HalfInt& HalfInt::operator -- ()
{
  twice_value_ -= 2;	
  return *this;
}

inline HalfInt HalfInt::operator -- (int)
{
  HalfInt h = *this;
  twice_value_ -= 2;
  return h;
}


////////////////////////////////////////////////////////////////
// binary arithmetic operators
////////////////////////////////////////////////////////////////

inline HalfInt operator + (const HalfInt& a, const HalfInt& b) 
{
  HalfInt sum(a);
  sum += b;
  return sum;
}

inline HalfInt operator - (const HalfInt& a, const HalfInt& b) 
{
  HalfInt sum(a);
  sum -=b;
  return sum;
}

////////////////////////////////////////////////////////////////
// relational operators
////////////////////////////////////////////////////////////////

inline bool operator == (const HalfInt& h1, const HalfInt& h2)
{
  return h1.TwiceValue() == h2.TwiceValue();
}

inline bool operator < (const HalfInt& h1, const HalfInt& h2)
{
  return h1.TwiceValue() < h2.TwiceValue();
}

inline bool operator > (const HalfInt& h1, const HalfInt& h2)
{
  return !((h1==h2)||(h1<h2));
}

inline bool operator >= (const HalfInt& h1, const HalfInt& h2)
{
  return !(h1 < h2);
}

inline bool operator <= (const HalfInt& h1, const HalfInt& h2)
{
  return ((h1 < h2)||(h1 == h2));
}

inline bool operator != (const HalfInt& h1, const HalfInt& h2)
{
  return !(h1 == h2);
}

////////////////////////////////////////////////////////////////
// hashing (support for Boost automatic hash lookup)
////////////////////////////////////////////////////////////////

inline
std::size_t hash_value(const HalfInt& h)
// Hash a HalfInt by its internal integer representation.
//
// Using the official hasher seems to be overkill, as g++ appears to
// just hash an integer to itself, though this does guarantee
// appropriate restriction of the range to size_t.
{
  std::hash<int> int_hash;
  return int_hash(TwiceValue(h));
}


////////////////////////////////////////////////////////////////
// arithmetic functions
////////////////////////////////////////////////////////////////

inline HalfInt abs(const HalfInt& h)
// Calculate absolute value.
{
  if (h.TwiceValue() < 0)
    return -h;
  else 
    return h;
}

// angular momentum hat symbol
//
// Overloading: Versions for both halfint and int arguments are
// provided, though the int version is strictly unnecessary due to
// automatic type conversion from halfint to int.

inline
double Hat(const HalfInt& j)
{
  return sqrt(static_cast<double>(TwiceValue(j)+1));
}

inline
double Hat(int j)
{
  return sqrt(static_cast<double>(2*j+1));
}

// phase sign (-)^sum

// DEBUGGING: Note that C++ % operator is not guaranteed positive
// definite unless both arguments are nonnegative.  Although taking
// abs() does not in general preserve modular equivalency class, it
// does suffice for the present purpose (checking evenness or
// oddness).  Without abs(), e.g., ParitySign(-1) can result in
// failure, from a remainder result other than 0 or 1.

inline 
int ParitySign(const HalfInt& sum)
{
  //int remainder = abs(IValue(sum)) % 2;
  assert(IsInteger(sum));
  int remainder = abs(int(sum)) % 2;
  // int sign = (remainder == 0) ? +1 : -1; // folklore says to avoid conditional for performance
  int sign = 1 - 2*remainder;
  return sign;
}

inline 
int ParitySign(int sum)
{
  int remainder = abs(sum) % 2;
  int sign = 1 - 2*remainder;
  return sign;
}

////////////////////////////////////////////////////////////////
// stream output
////////////////////////////////////////////////////////////////

// textual output to stream
// EX: HalfInt(3) or HalfInt(6,2) -> "3", HalfInt(3,2) -> "3/2"
std::ostream& operator<< (std::ostream&, const HalfInt&);

std::ostream& operator<< (std::ostream&, const HalfInt::pair&);

#endif
