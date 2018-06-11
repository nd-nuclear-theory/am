/****************************************************************
  racah_reduction.h

  Defines coefficients in Racah reduction formulas.

  Takes HalfInt angular momentum arguments, i.e., based on wigner_gsl.

  Language: C++

  Mark A. Caprio
  University of Notre Dame

  4/28/18 (mac): Created, based on code from moshinsky_xform.cpp.

****************************************************************/

#ifndef RACAH_REDUCTION_H_
#define RACAH_REDUCTION_H_

#include "am/wigner_gsl.h"

namespace am {

  double RacahReductionFactor1Rose(
      const HalfInt& J1p, const HalfInt& J2p, const HalfInt& Jp,
      const HalfInt& J1, const HalfInt& J2, const HalfInt& J,
      const HalfInt& J0
    )
  // Coefficient in Racah two-system reduction formula, for case of first-system
  // operator.
  //
  // Follows either Rose or Brink-Satchler convention for RME:
  //
  //   <J1',J2;J||A_1^J0||J1,J2;J>
  //     =(-)^(J1'+J2+J+J0)*Hat(J1')*Hat(J)*{J1',J',J2;J,J1,J0} 
  //
  // Note: Under Edmonds or Bohr-Mottelson convention, the Hat(J1') would be a
  // Hat(J').
  //
  // See, e.g., Brink & Satchler, Angular momentum, 2ed. (1968), Appendix VI.
  //
  // Assertion: Arguments J2p and J2 must be equal.
  //
  // Arguments:
  //   J1p, J2p, Jp (input): bra angular momenta
  //   J1, J2, J (input): ket angular momenta
  //   J0 (input): operator angular momentum
  //
  // Returns:
  //   coefficient
  {

    assert(J2p==J2);

    double value = ParitySign(J1p+J2+J+J0)
      *Hat(J1p)*Hat(J)
      *Wigner6J(J1p,Jp,J2,J,J1,J0);
    return value;
  }

  double RacahReductionFactor2Rose(
      const HalfInt& J1p, const HalfInt& J2p, const HalfInt& Jp,
      const HalfInt& J1, const HalfInt& J2, const HalfInt& J,
      const HalfInt& J0
    )
  // Coefficient in Racah two-system reduction formula, for case of second-system
  // operator.
  //
  // Follows either Rose or Brink-Satchler convention for RME:
  //
  //   <J1,J2';J||A_2^J0||J1,J2;J>
  //     =(-)^(J1+J2+J'+J0)*Hat(J2')*Hat(J)*{J',J2',J1;J2,J,J0} 
  //
  // Note: Under Edmonds or Bohr-Mottelson convention, the Hat(J2') would be a
  // Hat(J').
  //
  // See, e.g., Brink & Satchler, Angular momentum, 2ed. (1968), Appendix VI.
  //
  // Assertion: Arguments J1p and J1 must be equal.
  //
  // Arguments:
  //   J1p, J2p, Jp (input): bra angular momenta
  //   J1, J2, J (input): ket angular momenta
  //   J0 (input): operator angular momentum
  //
  // Returns:
  //   coefficient
  {

    assert(J1p==J1);

    double value = ParitySign(J1+J2+Jp+J0)
      *Hat(J2p)*Hat(J)
      *Wigner6J(Jp,J2p,J1,J2,J,J0);
    return value;
  }

  double RacahReductionFactor12DotRose(
      const HalfInt& J1p, const HalfInt& J2p, const HalfInt& Jp,
      const HalfInt& J1, const HalfInt& J2, const HalfInt& J,
      const HalfInt& J0
    )
  // Calculate coefficient in Racah two-system reduction formula, for case of
  // scalar product of two operators, applicable to either Rose or Brink-Satchler
  // Wigner-Eckart convention.
  //
  // See, e.g., Brink & Satchler, Angular momentum, 2ed. (1968), Appendix VI.
  {

    assert(Jp==J);

    double value = ParitySign(J2p+Jp+J1)
      * Hat(J1p) * Hat(J2p)
      * Wigner6J(J1p, J2p, Jp, J2 J1, J0);
    return value;
  }

  double RacahReductionFactor12Rose(
      const HalfInt& J1p, const HalfInt& J2p, const HalfInt& Jp,
      const HalfInt& J1, const HalfInt& J2, const HalfInt& J,
      const HalfInt& J0a, const HalfInt& J0b, const HalfInt& J0
    )
  // Calculate coefficient in Racah two-system reduction formula, for case of
  // scalar product of two operators, applicable to either Rose or Brink-Satchler
  // Wigner-Eckart convention.
  //
  // See, e.g., Brink & Satchler, Angular momentum, 2ed. (1968), Appendix VI.
  {

    assert(AllowedTriangle(Jp, J, J0));

    double value = Hat(J0) * Hat(J)
      * Hat(J1p) * Hat(J2p)
      * Wigner9J(J1p, J2p, Jp, J1, J2, J, J0a, J0b, J0);
    return value;
  }

} // namespace

#endif
