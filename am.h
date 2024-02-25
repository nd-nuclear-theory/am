/****************************************************************
  am.h

  Angular momentum algebra utility functions.

  Mark A. Caprio
  University of Notre Dame

  + 12/02/10 (mac): Created, code extracted from halfint originally by
    Ke Cai on 6/18/10:
    - Renamed functions addj() to ProductAngularMomenta()
      and validTriple() to AllowedTriangle()
    - Simplified implementation of ProductAngularMomenta()
      and AllowedTriangle()
  + 02/20/11 (mac): Revisions to integration with halfint.
  + 02/27/16 (mac): Restructuring:
    - Absorb halfint into am project, removing halfint_bound.
    - Remove TriangleBound, to eliminate dependence on halfint_bound.
    - Rename angular_momentum to am.
  + 03/08/16 (mac):
    - Enclose in namespace.
    - Revise expression for ParitySign.
    - Move Hat and ParitySign back to HalfInt so as not to
      require namespace qualification.
    - Add angular momentum range arithmetic (superseding old HalfIntBound).
  + 05/23/16 (mac): Add dim() for angular momentum dimension.
  + 06/=8/16 (mac): Update compilation guard directive.
  + 07/04/16 (mac): Fix name AngularMomentumRangeIntersection.
  + 07/05/16 (mac): Extend AngularMomentumRangeIntersection to arbitrary number
    of arguments.
  + 04/10/20 (pjf): Make operations `constexpr` when using C++14.

****************************************************************/

#ifndef AM_H_
#define AM_H_

#include <algorithm>
#include <utility>
#include <vector>

#include "halfint.h"

// constexpr is not flexible enough in C++11; only enable it if
// compiling with C++14 or later
#ifndef CXX14_CONSTEXPR
  #if __cplusplus >= 201402L
    #define CXX14_CONSTEXPR constexpr
  #else
    #define CXX14_CONSTEXPR
  #endif
#endif

namespace am {

  ////////////////////////////////////////////////////////////////
  // dimension factor
  ////////////////////////////////////////////////////////////////

  // dim
  //
  // Overloading: Versions for both halfint and int arguments are
  // provided, though the int version is strictly unnecessary due to
  // automatic type conversion from halfint to int.

  // Should Hat be moved from halfint.h (and global namespace) to here
  // (and am namespace), as well?

  CXX14_CONSTEXPR inline
  int dim(const HalfInt& j)
  {
    return TwiceValue(j)+1;
  }

  CXX14_CONSTEXPR inline
  int dim(int j)
  {
    return 2*j+1;
  }


  ////////////////////////////////////////////////////////////////
  // triangle inequality and coupling
  ////////////////////////////////////////////////////////////////

  CXX14_CONSTEXPR inline
  bool AllowedTriangle(const HalfInt& h1, const HalfInt& h2, const HalfInt& h3)
  // Test if three HalfInts are coupled legally, i.e., they form a closed triangle.
  //
  // Also checks if their combined parity is valid.
  //
  // Returns true if both triangle condition and parity condition are met, false if either is not met
  {
    bool triangular = ((abs(h1-h2) <= h3) && (h3 <= (h1+h2)));
    bool proper_integrity = IsInteger(h1 + h2 + h3);
    return triangular && proper_integrity;
  }

  template<
      typename T, typename U,
      typename R = typename std::common_type<T,U>::type,
      std::enable_if_t<
          std::is_constructible<HalfInt, R>::value
          || std::is_convertible<R, HalfInt>::value
        >* = nullptr
    >
  CXX14_CONSTEXPR inline
  std::vector<R> ProductAngularMomenta(const T j1, const U j2)
  // Create a vector of angular momenta that j1 and j2 can be coupled to under the triangle inequality.
  {
    // find triangle range
    R j_min = abs(static_cast<R>(j1)-static_cast<R>(j2));
    R j_max = static_cast<R>(j1)+static_cast<R>(j2);

    // allocate storage for results
    std::vector<R> result;
    int entries = int(j_max-j_min)+1;
    result.reserve(entries);

    // store results
    for (R j = j_min; j <= j_max; ++j)
      result.push_back(j);
    return result;
  }

  template<
      typename T, typename U,
      typename R = typename std::common_type<T,U>::type,
      std::enable_if_t<
          std::is_constructible<HalfInt, R>::value
          || std::is_convertible<R, HalfInt>::value
        >* = nullptr
    >
  CXX14_CONSTEXPR inline
  std::pair<R,R> ProductAngularMomentumRange(const T j1, const U j2)
  // Generate range of angular momenta allowed by triangle inequality.
  //
  // Arguments:
  //   j1, j2 (HalfInt) : angular momenta to couple (should be nonnegative)
  //
  // Returns:
  //   (HalfInt::pair) :  angular momentum range
  {
    return std::pair<R,R>(abs(j1-j2),j1+j2);
  }

  template<
      typename T, typename U,
      typename R = typename std::common_type<T,U>::type,
      std::enable_if_t<
          std::is_constructible<HalfInt, R>::value
          || std::is_convertible<R, HalfInt>::value
        >* = nullptr
    >
  CXX14_CONSTEXPR inline
  std::pair<R,R> AngularMomentumRangeIntersection(
      const std::pair<T,T>& r1, const std::pair<U,U>& r2
    )
  // Obtain intersection of two angular momentum ranges.
  //
  // Lower bound is max of lower bounds, and upper bound is min of upper bounds.
  //
  // Arguments:
  //   r1, r2 (HalfInt::pair) : angular momentum ranges
  //
  // Returns:
  //   (HalfInt::pair) :  angular momentum range
  {
    return std::pair<R,R>(std::max(r1.first,r2.first),std::min(r1.second,r2.second));
  }

  template <
      typename T, typename... Args,
      typename R = T,
      std::enable_if_t<
          std::is_constructible<HalfInt, R>::value
          || std::is_convertible<R, HalfInt>::value
        >* = nullptr
    >
  CXX14_CONSTEXPR inline
  std::pair<R,R> AngularMomentumRangeIntersection(
      const std::pair<T,T>& r1, Args&&... args
    )
  // Recursive extension of AngularMomentumRangeIntersection to
  // arbitrary number of arguments...
  {
    std::pair<R,R> r = AngularMomentumRangeIntersection(std::forward<Args>(args)...);
    return AngularMomentumRangeIntersection(r1,r);
  }

  // Debugging: This fails...  Apparently need template?  To check
  // C++11 variadic syntax...
  //
  // inline
  //   HalfInt::pair AngularMomentumRangeIntersection(
  //       const HalfInt::pair& r1, const HalfInt::pair&... args
  //     )



}  // namespace am

#endif  // AM_H_
