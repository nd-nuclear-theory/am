/****************************************************************
  halfint.h

  Defines arithmetic type HalfInt which stores integer or half-integer values,
  as needed, e.g., for angular momentum quantum numbers.

  Language: C++17

  Ke Cai, Veerle Hellemans, Mark A. Caprio
  University of Notre Dame

  Created by Ke Cai on 6/18/10.
  10/04/10 stylistic changes (vh):
    (1) removed "using namespace std" in Halfint.h and added to Halfint.cpp
    (2) copy constructor & copy assignment operator don't need to be defined:
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
  10/13/17 (pjf):
    - Add integer multiplication arithmetic assignment operator *=.
    - Add binary integer multiplication operator * as global function
      implemented in terms of *=.
  02/27/19 (pjf):
    - Add STL hashing.
  12/30/19 (pjf):
    - Replace assertions and exits with exceptions, for clean use
      from Python.
  03/31/20 (pjf):
    - Make most HalfInt operations `constexpr` when using C++14.
    - Change implementation of ParitySign to use bitwise operations
      and thus be `constexpr`-safe.
    - Mark conversions from float/double -> HalfInt as deleted to prevent
      ill-defined behavior.
    - Add Phase() for complex phase.
  08/05/21 (pjf): Allow HalfInt to be constructed from arbitrary integral types.
  08/09/21 (pjf): Templatize conversion operators.
  09/24/21 (pjf): Add user-defined literal _hi.
  03/07/22 (pjf):
    - Add pow(), ceil(), and floor().
    - Require C++17.
  04/10/24 (pjf):
    - Inline halfint.cpp to make library pure header.
    - Add min(), max(), and minmax() functions for ADL.
****************************************************************/

#ifndef HALFINT_H_
#define HALFINT_H_

#include <cmath> // for sqrt
#include <cstdlib>
#include <algorithm>
#include <complex>
#include <functional>  // for hash
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <stdexcept>
#include <type_traits>
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
  constexpr HalfInt() : twice_value_(0) {};

  // conversion from integer
  // EX: HalfInt(1) constructs 2/2, HalfInt(2) constructs 4/2
  template<typename T, std::enable_if_t<std::is_integral_v<T>, T>* = nullptr>
  constexpr HalfInt(T);

  // construct from numerator and denominator
  // EX: HalfInt(1,2) constructs 1/2, HalfInt(2, 2) constructs 1,
  //     HalfInt(2,1) constructs 4/2
  template<
    typename T, typename U,
    std::enable_if_t<std::is_arithmetic_v<T>, T>* = nullptr,
    std::enable_if_t<std::is_integral_v<U>, U>* = nullptr
    >
  constexpr HalfInt(T, U);

  // prevent construction from floating-point
  // EX: HalfInt(1.5) --> compile error
  HalfInt(float) = delete;
  HalfInt(double) = delete;

  // these are currently not deleted, since it can be useful to
  // construct a HalfInt using, e.g., HalfInt(2*j,2) when j is float
  // template<typename T> HalfInt(float, T) = delete;
  // template<typename T> HalfInt(double, T) = delete;
  // template<typename T> HalfInt(T, float) = delete;
  // template<typename T> HalfInt(T, double) = delete;

  ////////////////////////////////////////////////////////////////
  // accessors and conversion operators
  ////////////////////////////////////////////////////////////////

  // The accessors are used also to provide nonmenber "accessor"
  // functions by the same name, to permit more natural mathematical
  // notation, e.g., IsInteger(h).

  constexpr int TwiceValue() const
  // return twice value as type int
  {
    return twice_value_;
  };

  constexpr bool IsInteger() const
  // Test for integer value.
  {
    return !(twice_value_%2);
  }

  template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
  constexpr explicit operator T() const
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

  template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
  constexpr explicit operator T() const
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
    return static_cast<T>(twice_value_)/2;
  }


  ////////////////////////////////////////////////////////////////
  // arithmetic assignment operators
  ////////////////////////////////////////////////////////////////

  // note: operator = is synthesized assignment operator

  // operators return reference to allow for chained (a+=b)+=c structures
  constexpr HalfInt& operator +=(const HalfInt&);
  constexpr HalfInt& operator -=(const HalfInt&);
  constexpr HalfInt& operator *=(const int&);

  ////////////////////////////////////////////////////////////////
  // unary arithmetic operators
  ////////////////////////////////////////////////////////////////

  constexpr HalfInt operator +() const;
  constexpr HalfInt operator -() const;

  constexpr HalfInt& operator ++(); // prefix ++, increments then returns reference
  constexpr HalfInt operator ++(int); // postfix ++, returns value then increments
  constexpr HalfInt& operator --(); // prefix --, increments then returns reference
  constexpr HalfInt operator --(int); // postfix --, returns value then increments

  ////////////////////////////////////////////////////////////////
  // string conversion
  ////////////////////////////////////////////////////////////////

  inline std::string Str() const
  {
    std::ostringstream ss;

    if (IsInteger())
      ss << twice_value_/2;
    else
      ss << twice_value_ << "/2";

    return ss.str();
  }

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

template<typename T, std::enable_if_t<std::is_integral_v<T>, T>*>
constexpr inline HalfInt::HalfInt(T value)
  : twice_value_(2*value)
{}

template<typename T, typename U,
    std::enable_if_t<std::is_arithmetic_v<T>, T>*,
    std::enable_if_t<std::is_integral_v<U>, U>*>
constexpr inline HalfInt::HalfInt(T numerator, U denominator)
  : twice_value_((2/denominator)*numerator)
{
  if (!((denominator==1)||(denominator==2)))
    {
      throw std::invalid_argument("HalfInt constructed with denominator not 1 or 2");
    }
}

////////////////////////////////////////////////////////////////
// literal
////////////////////////////////////////////////////////////////

template<char... str>
constexpr HalfInt operator""_hi()
{
    constexpr char chars[sizeof...(str)] = {str...};
    constexpr bool has_dot = ((str == '.') || ...);
    static_assert(
        !has_dot
        || (sizeof...(str)>=1 && chars[sizeof...(str)-1]=='.')
        || (sizeof...(str)>=2 && chars[sizeof...(str)-2]=='.'),
        "only one digit allowed after decimal point in HalfInt literal"
      );
    constexpr char last_c = (sizeof...(str)>0) ? chars[sizeof...(str)-1] : 0;
    static_assert(
        !has_dot
        || (last_c == '.' || last_c == '0' || last_c == '5'),
        "HalfInt literal must be half-integer"
      );

    HalfInt h{0};
    int i = 0;
    for (char c : chars)
    {
        ++i;
        if (c == '.')
            break;
        h *= 10;
        h += c -'0';
    }
    if (has_dot && last_c == '5')
        h += HalfInt(1,2);
    return h;
}

////////////////////////////////////////////////////////////////
// arithmetic assignment operators
////////////////////////////////////////////////////////////////

constexpr inline HalfInt& HalfInt::operator +=(const HalfInt& b)
{
  twice_value_ += b.twice_value_;
  return *this;
}

constexpr inline HalfInt& HalfInt::operator -=(const HalfInt& b)
{
  twice_value_ -= b.twice_value_;
  return *this;
}

constexpr inline HalfInt& HalfInt::operator *=(const int& b)
{
  twice_value_ *= b;
  return *this;
}

////////////////////////////////////////////////////////////////
// nonmember accessor functions
////////////////////////////////////////////////////////////////

constexpr inline int TwiceValue(const HalfInt& h)
{
  return (h.TwiceValue());
}

constexpr inline bool IsInteger(const HalfInt& h)
{
  return (h.IsInteger());
}

////////////////////////////////////////////////////////////////
// unary arithmetic operators
////////////////////////////////////////////////////////////////

constexpr inline HalfInt HalfInt::operator + () const
{
  return *this;
};

constexpr inline HalfInt HalfInt::operator - () const
{
  HalfInt negative;
  negative.twice_value_ = -twice_value_;
  return negative;
};

constexpr inline HalfInt& HalfInt::operator ++ ()
{
  twice_value_ += 2;
  return *this;
}

constexpr inline HalfInt HalfInt::operator ++ (int)
{
  HalfInt h = *this;
  twice_value_ += 2;
  return h;
}

constexpr inline HalfInt& HalfInt::operator -- ()
{
  twice_value_ -= 2;
  return *this;
}

constexpr inline HalfInt HalfInt::operator -- (int)
{
  HalfInt h = *this;
  twice_value_ -= 2;
  return h;
}


////////////////////////////////////////////////////////////////
// binary arithmetic operators
////////////////////////////////////////////////////////////////

constexpr inline HalfInt operator + (const HalfInt& a, const HalfInt& b)
{
  HalfInt sum(a);
  sum += b;
  return sum;
}

constexpr inline HalfInt operator - (const HalfInt& a, const HalfInt& b)
{
  HalfInt sum(a);
  sum -= b;
  return sum;
}

constexpr inline HalfInt operator * (const HalfInt& a, const int& b)
{
  HalfInt product(a);
  product *= b;
  return product;
}

constexpr inline HalfInt operator * (const int& a, const HalfInt& b)
{
  HalfInt product(b);
  product *= a;
  return product;
}

////////////////////////////////////////////////////////////////
// relational operators
////////////////////////////////////////////////////////////////

constexpr inline bool operator == (const HalfInt& h1, const HalfInt& h2)
{
  return h1.TwiceValue() == h2.TwiceValue();
}

constexpr inline bool operator < (const HalfInt& h1, const HalfInt& h2)
{
  return h1.TwiceValue() < h2.TwiceValue();
}

constexpr inline bool operator > (const HalfInt& h1, const HalfInt& h2)
{
  return !((h1==h2)||(h1<h2));
}

constexpr inline bool operator >= (const HalfInt& h1, const HalfInt& h2)
{
  return !(h1 < h2);
}

constexpr inline bool operator <= (const HalfInt& h1, const HalfInt& h2)
{
  return ((h1 < h2)||(h1 == h2));
}

constexpr inline bool operator != (const HalfInt& h1, const HalfInt& h2)
{
  return !(h1 == h2);
}

////////////////////////////////////////////////////////////////
// hashing (support for Boost and STL automatic hash lookup)
////////////////////////////////////////////////////////////////

inline
std::size_t hash_value(const HalfInt& h)
// Hash a HalfInt by its internal integer representation.
//
// Using the official hasher seems to be overkill, as g++ appears to
// just hash an integer to itself, though this does guarantee
// appropriate restriction of the range to size_t.
{
  return std::hash<int>()(TwiceValue(h));
}

// Provide declaration of hash instantiation inside std namespace as workaround
// for bug in implementation of C++11 template instantiation in gcc versions
// prior to 7.0:
//
//    https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56480
//    https://stackoverflow.com/a/25594741

namespace std
{
  template <>
    struct hash<HalfInt>;
}

template <>
struct std::hash<HalfInt>
{
  inline
  std::size_t operator()(const HalfInt& h) const
  {
    return std::hash<int>()(TwiceValue(h));
  }
};


////////////////////////////////////////////////////////////////
// arithmetic functions
////////////////////////////////////////////////////////////////

template<
    typename... Ts,
    typename R = typename std::common_type_t<Ts...>,
    std::enable_if_t<
#ifndef SWIG
        (std::is_same_v<typename std::decay_t<Ts>,HalfInt> || ...) &&
#endif
        (std::is_constructible_v<HalfInt, R>
        || std::is_convertible_v<R, HalfInt>)
      >* = nullptr
  >
constexpr inline
R min(const Ts&... t)
{
  return std::min(static_cast<R>(t)...);
}

template<
    typename... Ts,
    typename R = typename std::common_type_t<Ts...>,
    std::enable_if_t<
#ifndef SWIG
        (std::is_same_v<typename std::decay_t<Ts>,HalfInt> || ...) &&
#endif
        (std::is_constructible_v<HalfInt, R>
        || std::is_convertible_v<R, HalfInt>)
      >* = nullptr
  >
constexpr inline
R max(const Ts&... t)
{
  return std::max(static_cast<R>(t)...);
}

template<
    typename... Ts,
    typename R = typename std::common_type_t<Ts...>,
    std::enable_if_t<
#ifndef SWIG
        (std::is_same_v<typename std::decay_t<Ts>,HalfInt> || ...) &&
#endif
        (std::is_constructible_v<HalfInt, R>
        || std::is_convertible_v<R, HalfInt>)
      >* = nullptr
  >
constexpr inline
std::pair<R,R> minmax(const Ts&... t)
{
  return std::minmax(static_cast<R>(t)...);
}

constexpr inline HalfInt abs(const HalfInt& h)
// Calculate absolute value.
{
  if (h.TwiceValue() < 0)
    return -h;
  else
    return h;
}

template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
inline T pow(T x, const HalfInt& j)
// Calculate x raised to HalfInt power
{
  return std::pow(x, static_cast<T>(j));
}

template<class T>
std::complex<T> pow(const std::complex<T>& x, const HalfInt& j)
// Calculate x raised to HalfInt power
{
  return std::pow(x, static_cast<T>(j));
}

inline double ceil(const HalfInt& j)
// Calculate ceiling of j
{
  if (IsInteger(j))
    return TwiceValue(j)/2.;
  else
    return (TwiceValue(j)+1)/2.;
}

inline double floor(const HalfInt& j)
// Calculate floor of j
{
  if (IsInteger(j))
    return TwiceValue(j)/2.;
  else
    return (TwiceValue(j)-1)/2.;
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
//
// Alternatively, we can assume that integers are stored in two's
// complement (which is guaranteed in C++20), and check oddness with
// a bitwise operation (int(sum)&1) or (TwiceValue(sum)&2). This also
// allows us to make the function `constexpr`.

constexpr inline
int ParitySign(const HalfInt& sum)
{
  if (!IsInteger(sum)) throw std::invalid_argument("complex phase encountered in am::ParitySign");

  //////// simple implementation ////////
  // int remainder = abs(int(sum)) % 2;
  // // int sign = (remainder == 0) ? +1 : -1; // folklore says to avoid conditional for performance
  // int sign = 1 - 2*remainder;

  //////// "clever" implementation ////////
  // (TwiceValue(sum)&2) -> 0 if sum is even, -> 2 if sum is odd
  int sign = 1 - (TwiceValue(sum)&2);

  return sign;
}

constexpr inline
int ParitySign(int sum)
{
  int remainder = sum&1;
  int sign = 1 - 2*remainder;
  return sign;
}

// complex phase (-1)^sum
//
// For half-integer values, the phase depends only on 2*J mod 4.
// This implies that the complex phase depends only on the two
// least-signficant bits of TwiceValue(sum). The least significant
// bit TwiceValue(sum)&1 is 0 if the phase is real, i.e. 2*J is even,
// and is 1 if the phase is imaginary. The second-least-significant
// bit TwiceValue(sum)&2 is 0 if the phase is positive, and 1 if the
// phase is negative.
constexpr inline
std::complex<double> Phase(const HalfInt& sum)
{
  // (TwiceValue(sum)&1) -> 0 if 2*sum is even, -> 1 if 2*sum is odd
  int im = TwiceValue(sum)&1;
  // (TwiceValue(sum)&2) -> 0 if sum mod 4 is 0 or 1, -> 2 if sum mod 4 is 2 or 3
  int sign = 1 - (TwiceValue(sum)&2);
  return std::complex<double>(sign*(1-im), sign*im);
}

////////////////////////////////////////////////////////////////
// stream output
////////////////////////////////////////////////////////////////

// textual output to stream
// EX: HalfInt(3) or HalfInt(6,2) -> "3", HalfInt(3,2) -> "3/2"
inline std::ostream& operator<< (std::ostream& os, const HalfInt& h)
{
  return os << h.Str();
}

inline std::ostream& operator<< (std::ostream& os, const HalfInt::pair& r)
{
  return os << "(" << r.first << "," << r.second << ")";
}

////////////////////////////////////////////////////////////////
// numeric limits
////////////////////////////////////////////////////////////////
namespace std
{
template<> class numeric_limits<HalfInt>
{
  static constexpr auto is_specialized = true;
  static constexpr auto is_signed = std::numeric_limits<int>::is_signed;
  static constexpr auto is_integer = false;
  static constexpr auto is_exact = true;
  static constexpr auto has_infinity = std::numeric_limits<int>::has_infinity;
  static constexpr auto has_quiet_NaN = std::numeric_limits<int>::has_quiet_NaN;
  static constexpr auto has_signaling_NaN = std::numeric_limits<int>::has_signaling_NaN;
  static constexpr auto has_denorm = std::numeric_limits<int>::has_denorm;
  static constexpr auto has_denorm_loss = std::numeric_limits<int>::has_denorm_loss;
  static constexpr auto round_style = std::numeric_limits<int>::round_style;
  static constexpr auto is_iec559 = std::numeric_limits<int>::is_iec559;
  static constexpr auto is_bounded = std::numeric_limits<int>::is_bounded;
  static constexpr auto is_modulo = std::numeric_limits<int>::is_modulo;
  static constexpr auto digits = std::numeric_limits<int>::digits - 1;
  static constexpr auto digits10 = int(float(digits) * 0.301029996);  // digits * log10(2)
  static constexpr auto max_digits10 = std::numeric_limits<int>::max_digits10;
  static constexpr auto radix = std::numeric_limits<int>::radix;
  static constexpr auto min_exponent = std::numeric_limits<int>::min_exponent;
  static constexpr auto min_exponent10 = std::numeric_limits<int>::min_exponent10;
  static constexpr auto max_exponent = std::numeric_limits<int>::max_exponent;
  static constexpr auto max_exponent10 = std::numeric_limits<int>::max_exponent10;
  static constexpr auto traps = false;
  static constexpr auto tinyness_before = std::numeric_limits<int>::tinyness_before;

  static constexpr HalfInt min() { return HalfInt(std::numeric_limits<int>::min(), 2); }
  static constexpr HalfInt max() { return HalfInt(std::numeric_limits<int>::max(), 2); }
  static constexpr HalfInt epsilon() { return 0; }
  static constexpr HalfInt round_error() { return 0; }
  static constexpr HalfInt infinity() { return 0; }
  static constexpr HalfInt quiet_NaN() { return 0; }
  static constexpr HalfInt signaling_NaN() { return 0; }
  static constexpr HalfInt denorm_min() { return 0; }
};

}  // namespace std

#endif  // HALFINT_H_
