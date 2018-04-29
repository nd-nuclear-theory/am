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
  // Calculate coefficient in Racah two-system reduction formula, for case of
  // first-system operator, applicable to either Rose or Brink-Satchler
  // Wigner-Eckart convention.
  //
  // See, e.g., Brink & Satchler, Angular momentum, 2ed. (1968), Appendix VI.
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
  // Calculate coefficient in Racah two-system reduction formula, for case of
  // second-system operator, applicable to either Rose or Brink-Satchler
  // Wigner-Eckart convention.
  //
  // See, e.g., Brink & Satchler, Angular momentum, 2ed. (1968), Appendix VI.
  {

    assert(J1p==J1);

    double value = ParitySign(J1+J2+Jp+J0)
      *Hat(J2p)*Hat(J)
      *Wigner6J(Jp,J2p,J1,J2,J,J0);
    return value;
  }

} // namespace

#endif
