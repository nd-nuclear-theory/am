/******************************************************************************

  halfint_test.cpp

  M. A. Caprio, P. J. Fasano
  University of Notre Dame

******************************************************************************/

#include <string>
#include <algorithm>

#include "fmt/format.h"
#include "fmt/ranges.h"

#include "am/halfint.h"
#include "am/halfint_fmt.h"

int main(int argc, char **argv)
{

  unsigned int three = 3;
  // HalfInt arithmetic tests
  std::cout << HalfInt(3) << " "  << HalfInt(3,1) << " " << HalfInt(3,2) << std::endl;
  std::cout << 3_hi << " "  << 1.5_hi << " " << 12.5_hi << std::endl;
  std::cout << TwiceValue(HalfInt(3,2)) << " " << HalfInt(3,2).TwiceValue() << std::endl;
  std::cout << HalfInt(three) << " "  << HalfInt(three,1) << " " << HalfInt(three,2) << std::endl;
  std::cout << std::max(HalfInt(5,2),HalfInt(1,2)) << std::endl;
  std::cout << std::min(HalfInt(5,2),HalfInt(1,2)) << std::endl;
  std::cout << HalfInt(-1,2) << " -> " << abs(HalfInt(-1,2)) << std::endl;
  std::cout << HalfInt(7,2) << " -> " << abs(HalfInt(7,2)) << std::endl;
  std::cout << -HalfInt(1,2) << std::endl;
  std::cout << HalfInt(1)+HalfInt(1,2) << std::endl;
  std::cout << 0+HalfInt(1,2) << std::endl;
  std::cout << 1+HalfInt(1,2) << std::endl;
  std::cout << "double... " << 1.0 + double(HalfInt(1,2)) << std::endl;
  std::cout << "****" << std::endl;

  // check conversion to and from string
  std::cout << "check conversion to string and back" << std::endl;
  for (HalfInt j(-100000); j <= 100000; j += HalfInt(1,2))
  {
    float j_f;
    std::string j_str = fmt::format("{:f}", j);
    std::istringstream j_stream(j_str);
    j_stream >> j_f;
    HalfInt j_conv = HalfInt(2*j_f, 2);
    if (j!=j_conv)
      std::cout << fmt::format(
          "{:g} -> HalfInt(2*{:.1f},2) -> {:g}, {}",
          j, j_f, j_conv, j == j_conv
        ) << std::endl;
  }
  std::cout << "****" << std::endl;

  // fmt formatting
  std::cout << fmt::format("{}", 1.5_hi) << std::endl;;
  std::cout << fmt::format("{}", std::tuple(0.5_hi, 1.5_hi, 2.5_hi)) << std::endl;
  const std::tuple t{1.5_hi, 2, 2.5_hi};
  std::cout << fmt::format("{}", t) << std::endl;
  std::cout << "****" << std::endl;

  // should cause compiler failure:
  // std::cout << "fallacious but lucky... 1.0 + HalfInt(1,2) = " << 1.0 + HalfInt(1,2) << std::endl;
  // std::cout << "fallacious and not lucky... 0.5 + HalfInt(1,2) = " << 0.5 + HalfInt(1,2) << std::endl;
  // std::cout << "****" << std::endl;

  // invalid denominator
  // std::cout << HalfInt(7,4) << std::endl; 	// causes throw

  // integer truncation
  std::cout << int(HalfInt(4,2)) << " " << int(HalfInt(3,2)) << " " << int(HalfInt(-3,2)) << std::endl;
  std::cout << "****" << std::endl;

  // comparison (with automatic upgrade of int)
  std::cout << "comparison: " << (HalfInt(1,2) >= 0) << " " << (HalfInt(1,2) >= 1) << std::endl;
  std::cout << "****" << std::endl;

  // hat arithmetic
  std::cout << Hat(HalfInt(1,2)) << " " << Hat(1) << std::endl;
  std::cout << "****" << std::endl;

  // parity sign
  std::cout << ParitySign(-1) << " " << ParitySign(HalfInt(-2,2)) << std::endl;
  std::cout << "****" << std::endl;

  // complex phase
  std::cout << Phase(HalfInt(1,2)) << std::endl;
  std::cout << "***" << std::endl;

  // hashing
  std::cout << "hash " << HalfInt(1,2).Str() << " " << hash_value(HalfInt(1,2)) << " "
            << HalfInt(22,2).Str() << " "  << hash_value(HalfInt(22,2)) << std::endl;
  std::cout << "****" << std::endl;

  // termination
  return 0;
}
