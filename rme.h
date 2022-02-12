/****************************************************************
  rme.h

  Defines standard reduced matrix elements from angular momentum theory.

  All RMEs are in Rose convention.  Since the operators are of integer rank,
  these are equivalently in Brink and Satchler convention.

  Functions take HalfInt angular momentum arguments, i.e., based on wigner_gsl.

  Language: C++

  Patrick J. Fasano
  University of Notre Dame

  + 08/11/18 (pjf): Created.
  + 12/03/18 (pjf): Add AngularMomentumOperatorType enum.
  + 07/01/19 (pjf): Fix phase on jjJCoupledAngularMomentumJ2RME, clean up to
    use more consistent phases in general (i.e. use two-system formula).
  + 07/07/19 (sp/pjf): Fix Wigner6J->Wigner3J typo in SphericalHarmonicCRME.
  + 04/10/20 (pjf): Replace assertions with exceptions.

****************************************************************/

#ifndef RME_H_
#define RME_H_

#include <cmath>
#include <stdexcept>

#include "wigner_gsl.h"
#include "racah_reduction.h"

namespace am {

  constexpr double kPi = 3.141592653589793238462643383279502884197169399375;
  constexpr double kSqrt4Pi = 3.544907701811032054596334966682290365595098912244;
  constexpr double kInvSqrt4Pi = 0.282094791773878143474039725780386292922025314664;

  enum class AngularMomentumOperatorType : char {kOrbital='l', kSpin='s', kTotal='j'};

  inline
  double SphericalHarmonicCRME(const int& lp, const int& l, const int& k)
  // Calculate reduced matrix element of the "C" spherical harmonic in l basis.
  //
  // This is the spherical harmonic in Racah's normalization [Brink & Satchler
  // (1993), app. IV, p. 145]:
  //
  //    Y_k = [(2*k+1)/(4*pi)]^(1/2) * C_k
  //
  // Arguments:
  //   lp (int): bra orbital angular momentum
  //   l (int): ket orbital angular momentum
  //   k (int): spherical harmonic rank
  //
  // Returns:
  //   reduced matrix element (double), Rose convention
  {
    if (!AllowedTriangle(lp, k, l)) throw std::domain_error("triangle disallowed");
    // Brink & Satchler (1993), app. VI, p.153
    double value = Hat(l) * ParitySign(lp) * Wigner3J(lp, k, l, 0, 0, 0);
    return value;
  }

  inline
  double LJCoupledSphericalHarmonicCRME(
    const int& lp, const HalfInt& jp, const int& l, const HalfInt& j,
    const int& k)
  // Calculate reduced matrix element of the "C" spherical harmonic in lj-coupled basis.
  //
  // This is the spherical harmonic in Racah's normalization [Brink & Satchler
  // (1993), app. IV, p. 145]:
  //
  //    Y_k = [(2*k+1)/(4*pi)]^(1/2) * C_k
  //
  // Arguments:
  //   lp (int): bra orbital angular momentum
  //   jp (HalfInt): bra total angular momentum
  //   l (int): ket orbital angular momentum
  //   j (HalfInt): ket total angular momentum
  //   k (int): spherical harmonic rank
  //
  // Returns:
  //   reduced matrix element (double), Rose convention
  {
    if (!AllowedTriangle(lp, HalfInt(1, 2), jp)) throw std::domain_error("triangle disallowed");
    if (!AllowedTriangle(l, HalfInt(1, 2), j)) throw std::domain_error("triangle disallowed");

    // parity constraint
    if ((lp+l+k)%2 != 0) {
      return 0;
    }

    // Brink & Satchler (1993), app. VI, p.153
    double value = Hat(j) * ParitySign(j + k - HalfInt(1, 2))
      * Wigner3J(jp, j, k, HalfInt(1, 2), -HalfInt(1, 2), 0);
    return value;
  }

  inline
  double SphericalHarmonicYRME(const int& lp, const int& l, const int& k)
  // Calculate reduced matrix element of spherical harmonic in l basis.
  //
  // Arguments:
  //   lp (int): bra orbital angular momentum
  //   l (int): ket orbital angular momentum
  //   k (int): spherical harmonic rank
  //
  // Returns:
  //   reduced matrix element (double), Rose convention
  {
    // by converting normalization from RME for "C" spherical harmonic
    //
    // Brink & Satchler (1993), app. IV, p. 145
    double value = Hat(k) * kInvSqrt4Pi
      * SphericalHarmonicCRME(lp, l, k);
    return value;
  }

  inline
  double LJCoupledSphericalHarmonicYRME(
    const int& lp, const HalfInt& jp, const int& l, const HalfInt& j,
    const int& k)
  // Calculate reduced matrix element of spherical harmonic in lj-coupled scheme.
  //
  // Arguments:
  //   lp (int): bra orbital angular momentum
  //   jp (HalfInt): bra total angular momentum
  //   l (int): ket orbital angular momentum
  //   j (HalfInt): ket total angular momentum
  //   k (int): spherical harmonic rank
  //
  // Returns:
  //   reduced matrix element (double), Rose convention
  {
    // by converting normalization from RME for "C" spherical harmonic
    //
    // Brink & Satchler (1993), app. IV, p. 145
    double value = Hat(k) * kInvSqrt4Pi
      * LJCoupledSphericalHarmonicCRME(lp, jp, l, j, k);
    return value;
  }

  inline
  double AngularMomentumJRME(const HalfInt& Jp, const HalfInt& J)
  // Calculate reduced matrix element of angular momentum operator.
  //
  // Arguments:
  //   Jp (HalfInt): bra angular momentum
  //   J  (HalfInt): ket total angular momentum
  //
  // Returns:
  //   reduced matrix element (double), Rose convention
  {
    if (J != Jp) return 0;
    // Brink & Satchler (1993), app. VI, p.153
    double value = std::sqrt(double(Jp)*double(Jp+1));
    return value;
  }

  inline
  double jjJCoupledAngularMomentumJ1RME(
    const HalfInt& J1p, const HalfInt& J2p, const HalfInt& Jp,
    const HalfInt& J1, const HalfInt& J2, const HalfInt& J
  )
  // Calculate reduced matrix element of angular momentum operator.
  //
  // Arguments:
  //   J1p, J2p, Jp (input): bra angular momenta
  //   J1, J2, J (input): ket angular momenta
  //
  // Returns:
  //   reduced matrix element (double), Rose convention
  {
    if (!AllowedTriangle(J1p, J2p, Jp)) throw std::domain_error("triangle disallowed");
    if (!AllowedTriangle(J1, J2, J)) throw std::domain_error("triangle disallowed");
    if (!AllowedTriangle(Jp, 1, J)) throw std::domain_error("triangle disallowed");
    // Kronecker deltas on (J1p,J1) and (J2p,J2)
    if ((J1p != J1) || (J2p != J2)) {
      return 0;
    }
    // Brink & Satchler (1993), app. VI, p.152
    double value = ParitySign(1+J2p+J+J1p)
      * std::sqrt(double(J1p)*double(J1p+1)*double(2*J1p+1)*double(2*J+1))
      * Wigner6J(Jp, J, 1, J1, J1p, J2p);
    return value;
  }

  inline
  double jjJCoupledAngularMomentumJ2RME(
    const HalfInt& J1p, const HalfInt& J2p, const HalfInt& Jp,
    const HalfInt& J1, const HalfInt& J2, const HalfInt& J
  )
  // Calculate reduced matrix element of angular momentum operator.
  //
  // Arguments:
  //   J1p, J2p, Jp (input): bra angular momenta
  //   J1, J2, J (input): ket angular momenta
  //
  // Returns:
  //   reduced matrix element (double), Rose convention
  {
    if (!AllowedTriangle(J1p, J2p, Jp)) throw std::domain_error("triangle disallowed");
    if (!AllowedTriangle(J1, J2, J)) throw std::domain_error("triangle disallowed");
    if (!AllowedTriangle(Jp, 1, J)) throw std::domain_error("triangle disallowed");
    // Kronecker deltas on (J1p,J1) and (J2p,J2)
    if ((J1p != J1) || (J2p != J2)) {
      return 0;
    }
    // Brink & Satchler (1993), app. VI, p.152
    double value = ParitySign(1+J1p+Jp+J2)
      * std::sqrt(double(J2p)*double(J2p+1)*double(2*J2p+1)*double(2*J+1))
      * Wigner6J(Jp, J, 1, J2, J2p, J1p);
    return value;
  }

  inline
  double jjJCoupledAngularMomentumJRME(
    const HalfInt& J1p, const HalfInt& J2p, const HalfInt& Jp,
    const HalfInt& J1, const HalfInt& J2, const HalfInt& J
  )
  // Calculate reduced matrix element of angular momentum operator.
  //
  // Arguments:
  //   J1p, J2p, Jp (input): bra angular momenta
  //   J1, J2, J (input): ket angular momenta
  //
  // Returns:
  //   reduced matrix element (double), Rose convention
  {
    if (!AllowedTriangle(J1p, J2p, Jp)) throw std::domain_error("triangle disallowed");
    if (!AllowedTriangle(J1, J2, J)) throw std::domain_error("triangle disallowed");
    if (!AllowedTriangle(Jp, 1, J)) throw std::domain_error("triangle disallowed");
    // Kronecker deltas on (J1p,J1) and (J2p,J2)
    if ((J1p != J1) || (J2p != J2)) {
      return 0;
    }
    // Brink & Satchler (1993), app. VI, p.153
    double value = AngularMomentumJRME(Jp, J);
    return value;
  }

}  // namespace am
#endif  // RME_H_
