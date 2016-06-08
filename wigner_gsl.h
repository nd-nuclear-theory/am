/****************************************************************
  wigner_gsl.h                       

  Defines Wigner coupling and recoupling symbols as wrappers 
  for GSL angular momentum functions.  

  See, e.g., appendix to de Shalit and Talmi for underlying formulas.

  Includes support for HalfInt arguments.

  Language: C++
  Mark A. Caprio, University of Notre Dame.

  2/16/10 (mac): Created.
  11/13/15 (mac): Add unitary 6J for (12)3-(13)2 recoupling 
    and Racah reduction factor.
  2/27/16 (mac): Update includes for restructured header files.
  3/8/16 (mac): Enclose in namespace.
  6/8/16 (mac): Update compilation guard directive.
 
****************************************************************/

#ifndef WIGNER_GSL_H_
#define WIGNER_GSL_H_

#include <gsl/gsl_sf_coupling.h>

#include "am.h"

// Naming convention: 
//   - Function names *not* ending in '2' accept HalfInt arguments J.
//   - Function names ending in '2' accept integer arguments 2*J.

namespace am {

  // Wigner3J(ja,jb,jc,ma,mb,mc)
  //   returns Wigner 3-J symbol
  //   wrapper for gsl_sf_coupling_3j

  inline 
    double Wigner3J(
		    const HalfInt& ja, const HalfInt& jb, const HalfInt& jc, 
		    const HalfInt& ma, const HalfInt& mb, const HalfInt& mc
		    )
  {
    return gsl_sf_coupling_3j(
			      TwiceValue(ja), TwiceValue(jb), TwiceValue(jc), 
			      TwiceValue(ma), TwiceValue(mb), TwiceValue(mc)
			      );
  }

  // ClebschGordan(ja,ma,jb,mb,jc,mc)
  //   returns Clebsch-Gordan coefficient
  //   wrapper for gsl_sf_coupling_3j

  inline 
    double ClebschGordan(
			 const HalfInt& ja, const HalfInt& ma, 
			 const HalfInt& jb, const HalfInt& mb, 
			 const HalfInt& jc, const HalfInt& mc
			 )
  {
    return Hat(jc)*ParitySign(ja-jb+mc)
      *gsl_sf_coupling_3j(
			  TwiceValue(ja), TwiceValue(jb), TwiceValue(jc), 
			  TwiceValue(ma), TwiceValue(mb), -TwiceValue(mc)
			  );
    ;
  }

  // Wigner6J(ja,jb,jc,jd,je,jf)
  //   returns Wigner 6-J symbol
  //   wrapper for gsl_sf_coupling_6j

  inline 
    double Wigner6J(
		    const HalfInt& ja, const HalfInt& jb, const HalfInt& jc, 
		    const HalfInt& jd, const HalfInt& je, const HalfInt& jf
		    )
  {
    return gsl_sf_coupling_6j(
			      TwiceValue(ja), TwiceValue(jb), TwiceValue(jc), 
			      TwiceValue(jd), TwiceValue(je), TwiceValue(jf)
			      );
  }

  // Unitary6J(ja,jb,jc,jd,je,jf)
  //   wrapper for gsl_sf_coupling_6j
  //   returns unitary recoupling symbol for (12)3-1(23) recoupling
  //
  //   Arguments follow row order in 6J symbol:
  //     Unitary6J(J1,J2,J12,J3,J,J13)
  //   This is equivalent to U(J1,J2,J,J3,J12,J2), though neither is a
  //   particularly memorable ordering.  Why not (J1,J2,J3,J12,J23,J)?!

  inline 
    double Unitary6J(
		     const HalfInt& ja, const HalfInt& jb, const HalfInt& jc, 
		     const HalfInt& jd, const HalfInt& je, const HalfInt& jf
		     )
  {
    return ParitySign(ja+jb+jd+je)*Hat(jc)*Hat(jf)
      *Wigner6J(ja,jb,jc,jd,je,jf);
  }

  // Unitary6JZ(ja,jb,jc,jd,je,jf)
  //   wrapper for gsl_sf_coupling_6j
  //   returns unitary recoupling symbol for (12)3-(13)2 recoupling
  //   the name "Z" follows Millener for this purpose
  //
  //   Arguments follow row order in 6J symbol:
  //     Unitary6JZ(J1,J2,J12,J,J3,J13)
  //   This is equivalent to Z(J2,J1,J,J3,J12,J13), though neither is a
  //   particularly memorable ordering.  Why not (J1,J2,J3,J12,J13,J)?!

  inline 
    double Unitary6JZ(
		      const HalfInt& ja, const HalfInt& jb, const HalfInt& jc, 
		      const HalfInt& jd, const HalfInt& je, const HalfInt& jf
		      )
  {
    return ParitySign(jb+je+jc+jf)*Hat(jc)*Hat(jf)
      *Wigner6J(ja,jb,jc,jd,je,jf);
  }

  // RacahReductionFactorFirstSystem(j1p,j2p,Jp,j1,j2,J,J0)
  //   wrapper for gsl_sf_coupling_6j
  //   returns prefactor for first-system operator in Racah two-system 
  //     reduction formula

  inline 
    double RacahReductionFactorFirstSystem(
					   const HalfInt& j1p, const HalfInt& j2p, const HalfInt& Jp, 
					   const HalfInt& j1, const HalfInt& j2, const HalfInt& J, 
					   const HalfInt& J0
					   )
  {
    return ParitySign(j1p+j2p+J+J0)*Hat(Jp)*Hat(J)
      *Wigner6J(j1p,Jp,j2p,J,j1,J0);
  }

  // Wigner9J(ja,jb,jc,jd,je,jf,jg,jh,ji)
  //   returns Wigner 9-J symbol
  //   wrapper for gsl_sf_coupling_9j

  inline 
    double Wigner9J(
		    const HalfInt& ja, const HalfInt& jb, const HalfInt& jc, 
		    const HalfInt& jd, const HalfInt& je, const HalfInt& jf,
		    const HalfInt& jg, const HalfInt& jh, const HalfInt& ji
		    )
  {
    return gsl_sf_coupling_9j(
			      TwiceValue(ja), TwiceValue(jb), TwiceValue(jc), 
			      TwiceValue(jd), TwiceValue(je), TwiceValue(jf),
			      TwiceValue(jg), TwiceValue(jh), TwiceValue(ji)
			      );
  }

  // Unitary9J(ja,jb,jc,jd,je,jf,jg,jh,ji)
  //   returns unitary 9-J symbol
  //   wrapper for gsl_sf_coupling_9j

  inline 
    double Unitary9J(
		     const HalfInt& ja, const HalfInt& jb, const HalfInt& jc, 
		     const HalfInt& jd, const HalfInt& je, const HalfInt& jf,
		     const HalfInt& jg, const HalfInt& jh, const HalfInt& ji
		     )
  {
    return Hat(jc)*Hat(jf)*Hat(jg)*Hat(jh)
      *Wigner9J(
		ja, jb, jc, 
		jd, je, jf,
		jg, jh, ji
		);
  }

} // namespace

#endif
