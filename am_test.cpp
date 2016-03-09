/******************************************************************************
  am_test.cpp
 
  Created by Mark A. Caprio, 2/16/11.

******************************************************************************/

#include "am.h"
#include "wigner_gsl.h"

int main(int argc, char **argv)
{

	// hat arithmetic
	std::cout << Hat(HalfInt(1,2)) << " " << Hat(1) << std::endl;

	// angular momentum tests
        // Examples: {1, 2, 2}, {1, 0, 2}, {1, 2, 3/2}, {1, 3/2, 3/2}
	std::cout << am::AllowedTriangle(1,2,2) << " " 
	     << am::AllowedTriangle(1,0,2) << " "
	     << am::AllowedTriangle(1,2,HalfInt(3,2)) << " " 
	     << am::AllowedTriangle(1,HalfInt(3,2),HalfInt(3,2)) << " " 
	     << std::endl;

	std::cout << "product angular momenta" << std::endl;
	HalfInt::vector v = am::ProductAngularMomenta(2,HalfInt(3,2));
	for (HalfInt::vector::iterator i = v.begin(); i != v.end(); i++)
	{
		std::cout << (*i) << " ";
	}
	std::cout << std::endl;

	std::cout << ParitySign(-1) << std::endl;
	std::cout << "****" << std::endl;
	
	// angular momentum range arithmetic

	// GSL coupling tests
	std::cout << "Wigner 3-J: Expect 0.276026..." << std::endl;
	std::cout << am::Wigner3J(2, HalfInt(3,2), HalfInt(5,2), +2, -HalfInt(1,2), -HalfInt(3,2)) << std::endl;
	std::cout << "CG: Expect 0.676123..." << std::endl;
	std::cout << am::ClebschGordan(2, +2, HalfInt(3,2), -HalfInt(1,2), HalfInt(5,2), +HalfInt(3,2)) << std::endl;
	std::cout << "Wigner 6-J: Expect 0.0757095..." << std::endl;
	std::cout << am::Wigner6J(2, HalfInt(5,2), HalfInt(9,2), 5, HalfInt(5,2), HalfInt(7,2)) << std::endl;
	std::cout << "Unitary 6-J: Expect 0.677166..." << std::endl;
	std::cout << am::Unitary6J(2, HalfInt(5,2), HalfInt(9,2), 5, HalfInt(5,2), HalfInt(7,2)) << std::endl;
	std::cout << "Wigner 9-J: Expect -0.00197657..." << std::endl;
	std::cout << am::Wigner9J(6,3,7,4,5,3,9,8,10) << std::endl;
	std::cout << "Unitary 9-J: Expect -0.364006..." << std::endl;
	std::cout << am::Unitary9J(6,3,7,4,5,3,9,8,10) << std::endl;


	std::cout << "****" << std::endl;

	// termination
	return 0;
}
