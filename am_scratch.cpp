inline
double Hat2(int two_j)
{
	return sqrt(static_cast<double>(two_j+1));
}

inline
double Hat(const HalfInt& j)
{
	return Hat2(TwiceValue(j));
}

// phase sign (-)^sum
// DEBUGGING: Note that C++ % operator is not guaranteed positive definite unless
// both arguments are nonnegative.  Although taking abs() does not in general 
// preserve modular equivalency class, it does suffice for the present purpose.
// Without abs(), e.g., ParitySign(-1) can result in exception.
inline 
int ParitySign2(int two_sum)
{

	int remainder = abs(two_sum) % 4;
	if (remainder == 0) 
		return +1;
	else if (remainder == 2)
		return -1;
	else
	{
		std::cerr << "ParitySign2 called for noneven value of twice the angular momentum" << std::endl; 
		throw std::invalid_argument("ParitySign2 called for noneven value of twice the angular momentum");
	}
}

inline 
int ParitySign(const HalfInt& sum)
{
	return ParitySign2(TwiceValue(sum));
}
