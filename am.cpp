/****************************************************************
  am.cpp

  Mark A. Caprio
  University of Notre Dame

  See header file for documentation.

****************************************************************/

#include "am.h"

namespace am {

  HalfInt::vector ProductAngularMomenta(const HalfInt& j1, const HalfInt& j2)
  {
    // find triangle range
    HalfInt j_min = abs(j1-j2);
    HalfInt j_max = j1+j2;

    // allocate storage for results
    HalfInt::vector result;
    int entries = int(j_max-j_min)+1;
    result.reserve(entries);

    // store results
    for (HalfInt j = j_min; j <= j_max; ++j)
      result.push_back(j);
    return result;
  }

} // namespace
