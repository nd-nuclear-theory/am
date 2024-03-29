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
  + 02/11/22 (mac): Clarify conventions in docstrings.
  + 03/04/22 (pjf):
    - Use macro AM_EXCEPTIONS to toggle between throwing
      exceptions and simply returning zero.
    - Use C++20 math constants if available.
  + 04/19/22 (mac): Further expand docstrings.
  + 04/19/22 (pjf): Add additional references to docstrings.

****************************************************************/

#ifndef RME_H_
#define RME_H_

#include <cmath>
#include <stdexcept>
#if __has_include(<numbers>)
#  include <numbers>
#endif

#include "wigner_gsl.h"
#include "racah_reduction.h"

namespace am {

#ifdef __cpp_lib_math_constants
  constexpr double kPi = std::numbers::pi;
  constexpr double kSqrt4Pi = 2./std::numbers::inv_sqrtpi;
  constexpr double kInvSqrt4Pi = 0.5*std::numbers::inv_sqrtpi;
#else
  constexpr double kPi = 3.141592653589793238462643383279502884197169399375;
  constexpr double kSqrt4Pi = 3.544907701811032054596334966682290365595098912244;
  constexpr double kInvSqrt4Pi = 0.282094791773878143474039725780386292922025314664;
#endif

  enum class AngularMomentumOperatorType : char {kOrbital='l', kSpin='s', kTotal='j'};

  inline
  double SphericalHarmonicCRME(const int& lp, const int& l, const int& k)
  // Calculate reduced matrix element of spherical harmonic C between spatial
  // wave functions.
  //
  // Spherical harmonic is in Racah's normalization [G. Racah, Phys. Rev. 62,
  // 438 (1942), eqn. (46); Brink & Satchler (1993), eqn. (2.9)].
  //
  //   C_k = ((4*pi)/(2*k+1))^(1/2) * Y_k
  //
  // RME is in Rose convention.
  //
  // Arguments:
  //   lp (int): bra orbital angular momentum
  //   l (int): ket orbital angular momentum
  //   k (int): spherical harmonic rank
  //
  // Returns:
  //   reduced matrix element (double), Rose convention
  {
    #ifdef AM_EXCEPTIONS
    if (!AllowedTriangle(lp, k, l)) throw std::domain_error("triangle disallowed");
    #else
    if (!AllowedTriangle(lp, k, l)) return 0;
    #endif

    // Brink & Satchler (1993), app. VI, p.153
    double value = Hat(l) * ParitySign(lp) * Wigner3J(lp, k, l, 0, 0, 0);
    return value;
  }

  inline
  double LJCoupledSphericalHarmonicCRME(
    const int& lp, const HalfInt& jp, const int& l, const HalfInt& j,
    const int& k)
  // Calculate reduced matrix element of spherical harmonic C between
  // spatial-spin (lj-coupled) wave functions.
  //
  // Spherical harmonic is in Racah's normalization [G. Racah, Phys. Rev. 62,
  // 438 (1942), eqn. (46); Brink & Satchler (1993), eqn. (2.9)].
  //
  //   C_k = ((4*pi)/(2*k+1))^(1/2) * Y_k
  //
  // RME is in Rose convention.
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
    #ifdef AM_EXCEPTIONS
    if (!AllowedTriangle(lp, HalfInt(1, 2), jp)) throw std::domain_error("triangle disallowed");
    if (!AllowedTriangle(l, HalfInt(1, 2), j)) throw std::domain_error("triangle disallowed");
    #else
    if (!AllowedTriangle(lp, HalfInt(1, 2), jp)) return 0;
    if (!AllowedTriangle(l, HalfInt(1, 2), j)) return 0;
    #endif

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
  // Calculate reduced matrix element of spherical harmonic Y between spatial
  // wave functions.
  //
  // RME is in Rose convention.
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
  // Calculate reduced matrix element of spherical harmonic Y between
  // spatial-spin (lj-coupled) wave functions.
  //
  // RME is in Rose convention.
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
  // Calculate reduced matrix element of angular momentum operator J in standard
  // angular momentum basis.
  //
  // RME is in Rose convention.
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
  // Calculate reduced matrix element of angular momentum operator j1 (i.e., for
  // first subsystem) in jjJ coupled basis.
  //
  // RME is in Rose convention.
  //
  // Arguments:
  //   J1p, J2p, Jp (input): bra angular momenta
  //   J1, J2, J (input): ket angular momenta
  //
  // Returns:
  //   reduced matrix element (double), Rose convention
  {
    #ifdef AM_EXCEPTIONS
    if (!AllowedTriangle(J1p, J2p, Jp)) throw std::domain_error("triangle disallowed");
    if (!AllowedTriangle(J1, J2, J)) throw std::domain_error("triangle disallowed");
    if (!AllowedTriangle(Jp, 1, J)) throw std::domain_error("triangle disallowed");
    #else
    if (!AllowedTriangle(J1p, J2p, Jp)) return 0;
    if (!AllowedTriangle(J1, J2, J)) return 0;
    if (!AllowedTriangle(Jp, 1, J)) return 0;
    #endif

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
  // Calculate reduced matrix element of angular momentum operator j2 (i.e., for
  // second subsystem) in jjJ coupled basis.
  //
  // RME is in Rose convention.
  //
  // Arguments:
  //   J1p, J2p, Jp (input): bra angular momenta
  //   J1, J2, J (input): ket angular momenta
  //
  // Returns:
  //   reduced matrix element (double), Rose convention
  {
    #ifdef AM_EXCEPTIONS
    if (!AllowedTriangle(J1p, J2p, Jp)) throw std::domain_error("triangle disallowed");
    if (!AllowedTriangle(J1, J2, J)) throw std::domain_error("triangle disallowed");
    if (!AllowedTriangle(Jp, 1, J)) throw std::domain_error("triangle disallowed");
    #else
    if (!AllowedTriangle(J1p, J2p, Jp)) return 0;
    if (!AllowedTriangle(J1, J2, J)) return 0;
    if (!AllowedTriangle(Jp, 1, J)) return 0;
    #endif

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
  // Calculate reduced matrix element of angular momentum operator J (i.e.,
  // total angular momentum) in jjJ coupled basis.
  //
  // This is identical to calculation of RME of J in standard angular momentum
  // basis, except that checks are applied on the subsystem angular momenta.
  //
  // RME is in Rose convention.
  //
  // Arguments:
  //   J1p, J2p, Jp (input): bra angular momenta
  //   J1, J2, J (input): ket angular momenta
  //
  // Returns:
  //   reduced matrix element (double), Rose convention
  {
    #ifdef AM_EXCEPTIONS
    if (!AllowedTriangle(J1p, J2p, Jp)) throw std::domain_error("triangle disallowed");
    if (!AllowedTriangle(J1, J2, J)) throw std::domain_error("triangle disallowed");
    if (!AllowedTriangle(Jp, 1, J)) throw std::domain_error("triangle disallowed");
    #else
    if (!AllowedTriangle(J1p, J2p, Jp)) return 0;
    if (!AllowedTriangle(J1, J2, J)) return 0;
    if (!AllowedTriangle(Jp, 1, J)) return 0;
    #endif

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
