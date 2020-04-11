/****************************************************************
  halfint.cpp

  Ke Cai, Veerle Hellemans, Mark A. Caprio
  University of Notre Dame

  See header file for documentation.

****************************************************************/

#include "halfint.h"

#include <sstream>

std::string HalfInt::Str() const
  {
    std::ostringstream ss;

    if (IsInteger())
      ss << twice_value_/2;
    else
      ss << twice_value_ << "/2";

    return ss.str();
  }

std::ostream& operator<< (std::ostream& os, const HalfInt& h)
{
  return os << h.Str();
}

std::ostream& operator<< (std::ostream& os, const HalfInt::pair& r)
{
  return os << "(" << r.first << "," << r.second << ")";
}

