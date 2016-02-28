/******************************************************************************
  
  Created by Mark A. Caprio, 2/16/11.

******************************************************************************/

using namespace std;

#include "am.h"
#include "wigner_gsl.h"
#include "halfint.h"


int main(int argc, char **argv)
{

	// hat arithmetic
	cout << Hat(HalfInt(1,2)) << " " << Hat(1) << endl;

	// angular momentum tests
        // Examples: {1, 2, 2}, {1, 0, 2}, {1, 2, 3/2}, {1, 3/2, 3/2}
	cout << AllowedTriangle(1,2,2) << " " 
	     << AllowedTriangle(1,0,2) << " "
	     << AllowedTriangle(1,2,HalfInt(3,2)) << " " 
	     << AllowedTriangle(1,HalfInt(3,2),HalfInt(3,2)) << " " 
	     << endl;
        vector<HalfInt> v = ProductAngularMomenta(2,HalfInt(3,2));
	for (vector<HalfInt>::iterator i = v.begin(); i != v.end(); i++)
	{
		cout << (*i) << "\t";
	}
	cout << endl;

	cout << ParitySign(-1) << endl;
	cout << "****" << endl;

	// GSL coupling tests
	cout << "Wigner 3-J: Expect 0.276026..." << endl;
	cout << Wigner3J(2, HalfInt(3,2), HalfInt(5,2), +2, -HalfInt(1,2), -HalfInt(3,2)) << endl;
	cout << "CG: Expect 0.676123..." << endl;
	cout << ClebschGordan(2, +2, HalfInt(3,2), -HalfInt(1,2), HalfInt(5,2), +HalfInt(3,2)) << endl;
	cout << "Wigner 6-J: Expect 0.0757095..." << endl;
	cout << Wigner6J(2, HalfInt(5,2), HalfInt(9,2), 5, HalfInt(5,2), HalfInt(7,2)) << endl;
	cout << "Unitary 6-J: Expect 0.677166..." << endl;
	cout << Unitary6J(2, HalfInt(5,2), HalfInt(9,2), 5, HalfInt(5,2), HalfInt(7,2)) << endl;
	cout << "Wigner 9-J: Expect -0.00197657..." << endl;
	cout << Wigner9J(6,3,7,4,5,3,9,8,10) << endl;
	cout << "Unitary 9-J: Expect -0.364006..." << endl;
	cout << Unitary9J(6,3,7,4,5,3,9,8,10) << endl;


	cout << "****" << endl;

	// termination
	return 0;
}
