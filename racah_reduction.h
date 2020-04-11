/****************************************************************
  racah_reduction.h

  Defines coefficients in Racah reduction formulas.

  Takes HalfInt angular momentum arguments, i.e., based on wigner_gsl.

  Language: C++

  Mark A. Caprio
  University of Notre Dame

  + 04/28/18 (mac): Created, based on code from moshinsky_xform.cpp.
  + 06/13/18 (pjf): Add remaining two-system reduction formulae.
  + 08/10/18 (pjf): Add single-system reduction formula.
  + 01/24/19 (pjf): Add 2-1 two-system reduction formula.
  + 04/10/20 (pjf): Replace assertions with exceptions.

****************************************************************/

#ifndef RACAH_REDUCTION_H_
#define RACAH_REDUCTION_H_

#include <stdexcept>
#include <string>
#include "wigner_gsl.h"

namespace am {

  inline
  double RacahReductionFactorRose(
    const HalfInt& Jp, const HalfInt& J, const HalfInt& Jpp,
    const HalfInt& J0a, const HalfInt& J0b, const HalfInt& J0
  )
  // Calculate coefficient in Racah single-system reduction formula, for case of
  // tensor product of two operators, applicable to Rose Wigner-Eckart convention.
  //
  // See, e.g., Brink & Satchler, Angular momentum, 2ed. (1968), chapter V.
  //
  // Arguments:
  //   Jp (input): bra angular momentum
  //   J (input): ket angular momentum
  //   Jpp (input): intermediate state angular momentum
  //   J0a, J0b, J0 (input): operator angular momenta
  //
  // Returns:
  //   coefficient
  {
    if (!AllowedTriangle(J0a, J0b, J0)) throw std::domain_error("triangle disallowed");
    if (!AllowedTriangle(Jp, J0, J)) throw std::domain_error("triangle disallowed");

    double value = ParitySign(J0-Jp-J)
      * Hat(Jpp) * Hat(J0)
      * Wigner6J(Jp, J, J0, J0b, J0a, Jpp);
    return value;
  }

  inline
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
    if (!(J2p==J2)) throw std::domain_error("triangle disallowed");

    double value = ParitySign(J1p+J2+J+J0)
      *Hat(J1p)*Hat(J)
      *Wigner6J(J1p,Jp,J2,J,J1,J0);
    return value;
  }

  inline
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
    if (!(J1p==J1)) throw std::domain_error("triangle disallowed");

    double value = ParitySign(J1+J2+Jp+J0)
      *Hat(J2p)*Hat(J)
      *Wigner6J(Jp,J2p,J1,J2,J,J0);
    return value;
  }

  inline
  double RacahReductionFactor12DotRose(
      const HalfInt& J1p, const HalfInt& J2p, const HalfInt& Jp,
      const HalfInt& J1, const HalfInt& J2, const HalfInt& J,
      const HalfInt& J0
    )
  // Calculate coefficient in Racah two-system reduction formula, for case of
  // dot product of two operators, applicable to either Rose or Brink-Satchler
  // Wigner-Eckart convention.
  //
  // See, e.g., Brink & Satchler, Angular momentum, 2ed. (1968), Appendix VI.
  //
  // Arguments:
  //   J1p, J2p, Jp (input): bra angular momenta
  //   J1, J2, J (input): ket angular momenta
  //   J0 (input): operator angular momentum
  //
  // Returns:
  //   coefficient
  {

    if (Jp!=J) throw std::domain_error("triangle disallowed");

    double value = ParitySign(J2p+Jp+J1)
      * Hat(J1p) * Hat(J2p)
      * Wigner6J(J1p, J2p, Jp, J2, J1, J0);
    return value;
  }

  inline
  double RacahReductionFactor12Rose(
      const HalfInt& J1p, const HalfInt& J2p, const HalfInt& Jp,
      const HalfInt& J1, const HalfInt& J2, const HalfInt& J,
      const HalfInt& J0a, const HalfInt& J0b, const HalfInt& J0
    )
  // Calculate coefficient in Racah two-system reduction formula, for case of
  // tensor product of two operators, applicable to either Rose or Brink-Satchler
  // Wigner-Eckart convention.
  //
  // Tensor product of operator a acting on system 1, operator b acting on
  // system 2, coupled to J0:
  //
  //   <J1',J2';J||[A_1*B_2]^J0||J1,J2;J>
  //     =Hat(J')*Hat(J0)*{J',J,J0;J1',J1,J0a;J2',J2,J0b}*Hat(J1p)*Hat(J2p)
  //
  // See, e.g., Brink & Satchler, Angular momentum, 2ed. (1968), Appendix VI.
  //
  // Arguments:
  //   J1p, J2p, Jp (input): bra angular momenta
  //   J1, J2, J (input): ket angular momenta
  //   J0a, J0b, J0 (input): operator angular momenta
  //
  // Returns:
  //   coefficient
  {
    if (!AllowedTriangle(Jp, J, J0)) throw std::domain_error("triangle disallowed");

    double value = Hat(J0) * Hat(J)
      * Hat(J1p) * Hat(J2p)
      * Wigner9J(Jp, J, J0, J1p, J1, J0a, J2p, J2, J0b);
    return value;
  }

  inline
  double RacahReductionFactor21Rose(
      const HalfInt& J1p, const HalfInt& J2p, const HalfInt& Jp,
      const HalfInt& J1, const HalfInt& J2, const HalfInt& J,
      const HalfInt& J0a, const HalfInt& J0b, const HalfInt& J0
    )
  // Calculate coefficient in Racah two-system reduction formula, for case of
  // tensor product of two operators, applicable to either Rose or Brink-Satchler
  // Wigner-Eckart convention.
  //
  // Tensor product of operator a acting on system 2, operator b acting on
  // system 1, coupled to J0:
  //
  //   <J1',J2';J||[A_2*B_1]^J0||J1,J2;J>
  //     =(-)^(J0a+J0b-J0)*Hat(J')*Hat(J0)*{J',J,J0;J1',J1,J0b;J2',J2,J0a}*Hat(J1p)*Hat(J2p)
  //
  // See, e.g., Brink & Satchler, Angular momentum, 2ed. (1968), Appendix VI.
  //
  // Arguments:
  //   J1p, J2p, Jp (input): bra angular momenta
  //   J1, J2, J (input): ket angular momenta
  //   J0a, J0b, J0 (input): operator angular momenta
  //
  // Returns:
  //   coefficient
  {
    if (!AllowedTriangle(Jp, J, J0)) throw std::domain_error("triangle disallowed");

    double value = ParitySign(J0a + J0b - J0)
      * Hat(J0) * Hat(J)
      * Hat(J1p) * Hat(J2p)
      * Wigner9J(Jp, J, J0, J1p, J1, J0b, J2p, J2, J0a);
    return value;
  }

}  // namespace am

#endif  // RACAH_REDUCTION_H_
