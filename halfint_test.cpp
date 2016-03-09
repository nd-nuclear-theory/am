/******************************************************************************
  
  Created by M. A. Caprio
  University of Notre Dame
  November 27, 2010

  2/28/16 (mac) : Move into am library and remove halfint_bound.
 

******************************************************************************/

#include "halfint.h"

#include <string>
#include <algorithm>

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
  std::cout << "double... " << 1.0 + DValue(HalfInt(1,2)) << std::endl;
  std::cout << "****" << std::endl;

  // std::cout << HalfInt(7,4) << std::endl; 	// causes throw

  std::cout << HalfInt(4,2).IValue() << " " << IValue(HalfInt(4,2)) << std::endl;
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

  // hat arithmetic
  std::cout << Hat(HalfInt(1,2)) << " " << Hat(1) << std::endl;
  std::cout << "****" << std::endl;

  // parity sign
  std::cout << ParitySign(-1) << std::endl;
  std::cout << "****" << std::endl;

  // termination
  return 0;
}
