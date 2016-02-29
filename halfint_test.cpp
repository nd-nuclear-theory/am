/******************************************************************************
  
  Created by M. A. Caprio
  University of Notre Dame
  November 27, 2010

  2/28/16 (mac) : Move into am library and remove halfint_bound.
 

******************************************************************************/

#include "halfint.h"

#include <string>
#include <algorithm>

using namespace std;

int main(int argc, char **argv)
{

	// HalfInt tests
	cout << HalfInt(3) << " "  << HalfInt(3,1) << " " << HalfInt(3,2) << endl;
	cout << TwiceValue(HalfInt(3,2)) << endl;
	cout << max(HalfInt(5,2),HalfInt(1,2)) << endl;
	cout << min(HalfInt(5,2),HalfInt(1,2)) << endl;
	cout << HalfInt(-1,2) << " -> " << abs(HalfInt(-1,2)) << endl;
	cout << HalfInt(7,2) << " -> " << abs(HalfInt(7,2)) << endl;
	cout << -HalfInt(1,2) << endl;
	cout << HalfInt(1)+HalfInt(1,2) << endl;
	// UNNECESSARY: cout << zero + HalfInt(1,2) << endl;
	cout << 0+HalfInt(1,2) << endl;
	cout << 1+HalfInt(1,2) << endl;
	cout << "double... " << 1.0 + DValue(HalfInt(1,2)) << endl;
	cout << "****" << endl;

 	// cout << HalfInt(7,4) << endl; 	// causes throw

	cout << HalfInt(4,2).IValue() << " " << IValue(HalfInt(4,2)) << endl;
	// cout << HalfInt(1,2).IValue() << endl; 	// causes throw


	// cout << "****" << endl;
	// 
	// cout << HalfIntBound(HalfInt(1,2),HalfInt(3,2)) << endl;
	// cout <<	HalfIntBound(HalfInt(1,2),HalfInt(5,2))
	// 	*HalfIntBound(HalfInt(3,2),HalfInt(7,2)) 
	//      <<	HalfIntBound(HalfInt(1,2),HalfInt(5,2))
	// 	*HalfIntBound(HalfInt(7,2),HalfInt(9,2)) 
	//      << endl;
	// cout << "****" << endl;

	// termination
	return 0;
}
