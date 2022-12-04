""" Provide Python port of am_test.cpp unit tests.

    Language: Python 3

    Mark A. Caprio
    University of Notre Dame

    05/17/20 (mac): Created.
    12/04/22 (mac): Continue porting tests.

"""

import am

if (__name__=="__main__"):

    # // angular momentum tests
    # // Examples: {1, 2, 2}, {1, 0, 2}, {1, 2, 3/2}, {1, 3/2, 3/2}
    # std::cout << am::AllowedTriangle(1,2,2) << " "
    #         << am::AllowedTriangle(1,0,2) << " "
    #         << am::AllowedTriangle(1,2,HalfInt(3,2)) << " "
    #         << am::AllowedTriangle(1,HalfInt(3,2),HalfInt(3,2)) << " "
    #         << std::endl;

    print("{} {} {} {}".format(
        am.AllowedTriangle(1,2,2),
        am.AllowedTriangle(1,0,2),
        am.AllowedTriangle(1,2,am.HalfInt(3,2)),
        am.AllowedTriangle(1,am.HalfInt(3,2),am.HalfInt(3,2))
        ))
    
    # // angular momentum product
    # std::cout << "product angular momenta" << std::endl;
    # HalfInt::vector v = am::ProductAngularMomenta(2,HalfInt(3,2));
    # for (HalfInt::vector::iterator i = v.begin(); i != v.end(); i++)
    #   {
    #     std::cout << (*i) << " ";
    #   }
    # std::cout << std::endl;
    # std::cout << am::ProductAngularMomentumRange(2,HalfInt(3,2)) << std::endl;
    # std::cout << "****" << std::endl;
    print("am.ProductAngularMomentumRange(2, am.HalfInt(3,2)) = {}".format(am.ProductAngularMomentumRange(2, am.HalfInt(3,2))))
  
    # // angular momentum range arithmetic
    # std::cout << "range intersection" << std::endl;
    # constexpr HalfInt::pair r1(1,5);
    # constexpr HalfInt::pair r2(3,6);
    # constexpr HalfInt::pair r12 = am::AngularMomentumRangeIntersection(r1,r2);
    # std::cout << r1 << r2 << "->" << r12 << std::endl;
    # HalfInt::pair r3(4,7);
    # HalfInt::pair r123 = am::AngularMomentumRangeIntersection(r1,r2,r3);  // variadic form
    # std::cout << r1 << r2 << r3 << "->" << r123 << std::endl;
    # constexpr HalfInt::pair r1x(1,5);
    # constexpr HalfInt::pair r2x(7,9);
    # constexpr HalfInt::pair rx = am::AngularMomentumRangeIntersection(r1x,r2x);
    # std::cout << r1x << r2x << "->" << rx << std::endl;
    # std::cout << "****" << std::endl;
  
    # // GSL coupling tests
    # std::cout << "Wigner 3-J: Expect 0.276026..." << std::endl;
    # std::cout << am::Wigner3J(2, HalfInt(3,2), HalfInt(5,2), +2, -HalfInt(1,2), -HalfInt(3,2)) << std::endl;
    print("Wigner 3-J: Expect 0.276026...")
    print("am.Wigner3J(2, am.HalfInt(3,2), am.HalfInt(5,2), +2, -am.HalfInt(1,2), -am.HalfInt(3,2)) = {}".format(am.Wigner3J(2, am.HalfInt(3,2), am.HalfInt(5,2), +2, -am.HalfInt(1,2), -am.HalfInt(3,2))))
    # std::cout << "Clebsch-Gordan: Expect 0.676123..." << std::endl;
    # std::cout << am::ClebschGordan(2, +2, HalfInt(3,2), -HalfInt(1,2), HalfInt(5,2), +HalfInt(3,2)) << std::endl;
    print("CG: Expect 0.676123...")
    print("am.ClebschGordan(2, +2, am.HalfInt(3,2), -am.HalfInt(1,2), am.HalfInt(5,2), +am.HalfInt(3,2)) = {}".format(am.ClebschGordan(2, +2, am.HalfInt(3,2), -am.HalfInt(1,2), am.HalfInt(5,2), +am.HalfInt(3,2))))
    # std::cout << "Wigner 6-J: Expect 0.0757095..." << std::endl;
    # std::cout << am::Wigner6J(2, HalfInt(5,2), HalfInt(9,2), 5, HalfInt(5,2), HalfInt(7,2)) << std::endl;
    print("Wigner 6-J: Expect 0.0757095...")
    print("am.Wigner6J(2, am.HalfInt(5,2), am.HalfInt(9,2), 5, am.HalfInt(5,2), am.HalfInt(7,2)) = {}".format(am.Wigner6J(2, am.HalfInt(5,2), am.HalfInt(9,2), 5, am.HalfInt(5,2), am.HalfInt(7,2))))
    # std::cout << "Unitary 6-J: Expect 0.677166..." << std::endl;
    # std::cout << am::Unitary6J(2, HalfInt(5,2), HalfInt(9,2), 5, HalfInt(5,2), HalfInt(7,2)) << std::endl;
    print("Unitary 6-J: Expect 0.677166...")
    print("am.Unitary6J(2, am.HalfInt(5,2), am.HalfInt(9,2), 5, am.HalfInt(5,2), am.HalfInt(7,2)) = {}".format(am.Unitary6J(2, am.HalfInt(5,2), am.HalfInt(9,2), 5, am.HalfInt(5,2), am.HalfInt(7,2))))
    # std::cout << "Wigner 9-J: Expect -0.00197657..." << std::endl;
    # std::cout << am::Wigner9J(6,3,7,4,5,3,9,8,10) << std::endl;
    print("Wigner 9-J: Expect -0.00197657...")
    print("am.Wigner9J(6, 3, 7, 4, 5, 3, 9, 8, 10) = {}".format(am.Wigner9J(6, 3, 7, 4, 5, 3, 9, 8, 10)))
    # std::cout << "Unitary 9-J: Expect -0.364006..." << std::endl;
    # std::cout << am::Unitary9J(6,3,7,4,5,3,9,8,10) << std::endl;
    print("Unitary 9-J: Expect -0.364006...")
    print("am.Unitary9J(6, 3, 7, 4, 5, 3, 9, 8, 10) = {}".format(am.Unitary9J(6, 3, 7, 4, 5, 3, 9, 8, 10)))

    h = am.HalfInt(1, 2)
    print(am.Wigner9J(h, h, 0, h, h, 0, 0, 0, 0))
    print(am.Wigner9J(h, h, 0, h, h, 0, 1, 1, 0))
    print(am.Unitary9J(h, h, 0, h, h, 0, 1, 1, 0))
