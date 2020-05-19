/******************************************************************************

  halfint_test.cpp

  M. A. Caprio
  University of Notre Dame

******************************************************************************/

#include <string>
#include <algorithm>

#include "fmt/format.h"

#include "halfint.h"
#include "am/halfint_fmt.h"

int main(int argc, char **argv)
{

  // HalfInt tests
  std::cout << HalfInt(3) << " "  << HalfInt(3,1) << " " << HalfInt(3,2) << std::endl;
  std::cout << TwiceValue(HalfInt(3,2)) << std::endl;
  std::cout << std::max(HalfInt(5,2),HalfInt(1,2)) << std::endl;
  std::cout << std::min(HalfInt(5,2),HalfInt(1,2)) << std::endl;
  std::cout << HalfInt(-1,2) << " -> " << abs(HalfInt(-1,2)) << std::endl;
  std::cout << HalfInt(7,2) << " -> " << abs(HalfInt(7,2)) << std::endl;
  std::cout << -HalfInt(1,2) << std::endl;
  std::cout << HalfInt(1)+HalfInt(1,2) << std::endl;
  // UNNECESSARY: std::cout << zero + HalfInt(1,2) << std::endl;
  std::cout << 0+HalfInt(1,2) << std::endl;
  std::cout << 1+HalfInt(1,2) << std::endl;
  //std::cout << "double... " << 1.0 + DValue(HalfInt(1,2)) << std::endl;
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
  // should cause compiler failure:
  // std::cout << "fallacious but lucky... 1.0 + HalfInt(1,2) = " << 1.0 + HalfInt(1,2) << std::endl;
  // std::cout << "fallacious and not lucky... 0.5 + HalfInt(1,2) = " << 0.5 + HalfInt(1,2) << std::endl;
  // std::cout << "****" << std::endl;

  // std::cout << HalfInt(7,4) << std::endl; 	// causes throw

  // std::cout << HalfInt(4,2).IValue() << " " << IValue(HalfInt(4,2)) << std::endl;
  // std::cout << HalfInt(1,2).IValue() << std::endl; 	// causes throw


  // std::cout << "****" << std::endl;
  //
  // std::cout << HalfIntBound(HalfInt(1,2),HalfInt(3,2)) << std::endl;
  // std::cout <<	HalfIntBound(HalfInt(1,2),HalfInt(5,2))
  // 	*HalfIntBound(HalfInt(3,2),HalfInt(7,2))
  //      <<	HalfIntBound(HalfInt(1,2),HalfInt(5,2))
  // 	*HalfIntBound(HalfInt(7,2),HalfInt(9,2))
  //      << std::endl;
  // std::cout << "****" << std::endl;

  std::cout << int(HalfInt(4,2)) << " " << int(HalfInt(3,2)) << " " << int(HalfInt(-3,2)) << std::endl;
  std::cout << "****" << std::endl;

  // comparison (with automatic upgrade of int)
  std::cout << "comparison: " << (HalfInt(1,2) >= 0) << " " << (HalfInt(1,2) >= 1) << std::endl;
  std::cout << "****" << std::endl;
  
  // hat arithmetic
  std::cout << Hat(HalfInt(1,2)) << " " << Hat(1) << std::endl;
  std::cout << "****" << std::endl;

  // parity sign
  std::cout << ParitySign(-1) << std::endl;
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
