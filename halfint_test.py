"""Provide Python port of halfint_test.cpp unit tests.

    Language: Python 3

    Mark A. Caprio
    University of Notre Dame

    05/17/20 (mac): Created.
    06/26/20 (mac): Finish converting tests.  Add dict key test.

"""

import am

if (__name__=="__main__"):

    # // HalfInt arithmetic tests
    # std::cout << HalfInt(3) << " "  << HalfInt(3,1) << " " << HalfInt(3,2) << std::endl;
    # std::cout << TwiceValue(HalfInt(3,2)) << std::endl;
    # std::cout << std::max(HalfInt(5,2),HalfInt(1,2)) << std::endl;
    # std::cout << std::min(HalfInt(5,2),HalfInt(1,2)) << std::endl;
    # std::cout << HalfInt(-1,2) << " -> " << abs(HalfInt(-1,2)) << std::endl;
    # std::cout << HalfInt(7,2) << " -> " << abs(HalfInt(7,2)) << std::endl;
    # std::cout << -HalfInt(1,2) << std::endl;
    # std::cout << HalfInt(1)+HalfInt(1,2) << std::endl;
    # std::cout << 0+HalfInt(1,2) << std::endl;
    # std::cout << 1+HalfInt(1,2) << std::endl;
    # //std::cout << "double... " << 1.0 + DValue(HalfInt(1,2)) << std::endl;
    # std::cout << "double... " << 1.0 + double(HalfInt(1,2)) << std::endl;
    # std::cout << "****" << std::endl;
    # // should cause compiler failure:
    # // std::cout << "fallacious but lucky... 1.0 + HalfInt(1,2) = " << 1.0 + HalfInt(1,2) << std::endl;
    # // std::cout << "fallacious and not lucky... 0.5 + HalfInt(1,2) = " << 0.5 + HalfInt(1,2) << std::endl;
    # // std::cout << "****" << std::endl;

    print("{} {}".format(am.HalfInt(3),am.HalfInt(3,2)))
    print("{} {}".format(am.TwiceValue(am.HalfInt(3,2)),am.HalfInt(3,2).TwiceValue()))
    print("{}".format(max(am.HalfInt(5,2),am.HalfInt(1,2))))
    print("{}".format(min(am.HalfInt(5,2),am.HalfInt(1,2))))
    print("{} {}".format(am.HalfInt(-1,2),abs(am.HalfInt(-1,2))))
    print("{} {}".format(am.HalfInt(7,2),abs(am.HalfInt(7,2))))
    print("{}".format(-am.HalfInt(1,2)))
    print("{}".format(am.HalfInt(1)+am.HalfInt(1,2)))
    print("{} {}".format(0+am.HalfInt(1,2),1+am.HalfInt(1,2)))
    print("{}".format(1.0+float(am.HalfInt(1,2))))
    try:
        print("{}".format(1.0+am.HalfInt(1,2)))
    except TypeError as e:
        print(e)

    # // invalid denominator
    # // std::cout << HalfInt(7,4) << std::endl; 	// causes throw
    try:
        print("{}".format(am.HalfInt(7,4)))
    except ValueError as e:
        print(e)
    
    # // integer truncation
    # std::cout << int(HalfInt(4,2)) << " " << int(HalfInt(3,2)) << " " << int(HalfInt(-3,2)) << std::endl;

    print("{} {} {}".format(int(am.HalfInt(4,2)),int(am.HalfInt(3,2)),int(am.HalfInt(-3,2))))
    
    # // hat arithmetic
    # std::cout << Hat(HalfInt(1,2)) << " " << Hat(1) << std::endl;

    print("{} {}".format(am.Hat(am.HalfInt(1,2)),am.Hat(1)))
    
    # // parity sign
    # std::cout << ParitySign(-1) << std::endl;
    
    print("{} {}".format(am.ParitySign(-1),am.ParitySign(am.HalfInt(-2,2))))

    # // complex phase
    # std::cout << Phase(HalfInt(1,2)) << std::endl;
    
    print("{}".format(am.Phase(am.HalfInt(1,2))))
    
    # // hashing
    # std::cout << "hash " << HalfInt(1,2).Str() << " " << hash_value(HalfInt(1,2)) << " "
    #           << HalfInt(22,2).Str() << " "  << hash_value(HalfInt(22,2)) << std::endl;
    # std::cout << "****" << std::endl;
    print("{} {}".format(am.HalfInt(1,2).__hash__(),am.HalfInt(22,2).__hash__()))

    # Python: HalfInt as dict key
    
    d = {am.HalfInt(1,2): 999}
    print("{} {}".format(d,d[am.HalfInt(1,2)]))
