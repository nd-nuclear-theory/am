/****************************************************************
  wigner_gsl.h / wigner_gsl_twice.h

  Defines Wigner coupling and recoupling symbols as wrappers for GSL
  angular momentum functions:

    - wigner_gsl.h -- takes HalfInt angular momentum arguments (RECOMMENDED)
    - wigner_gsl_twice.h -- takes integer "twice value" angular momentum arguments

  Naming convention:
    - Function names *not* ending in '2' accept HalfInt arguments J.
    - Function names ending in '2' accept integer arguments 2*J.

  See, e.g., appendix to de Shalit and Talmi for underlying formulas.

  DOCUMENTATION: See wigner_gsl.h (rather than wigner_gsl_twice.h) for
  more complete function comments.

  Language: C++

  Mark A. Caprio
  University of Notre Dame

  + 02/16/10 (mac): Created.
  + 11/13/15 (mac): Add unitary 6J for (12)3-(13)2 recoupling
    and Racah reduction factor.
  + 02/27/16 (mac): Update includes for restructured header files.
  + 03/8/16 (mac): Enclose in namespace.
  + 06/8/16 (mac): Update #define guard directive.
  + 06/21/16 (mac): Remove Racah reduction factor. Update comments.
  + 10/18/16 (mac): Update Unitary6J comment. Rename wigner2_gsl.h to
    wigner_gsl_twice.h.
  + 04/28/18 (mac): Restore missing Hat2 and ParitySign2 to
    wigner_gsl_twice.h.
  + 04/10/20 (pjf): Replace assertions with exceptions.

****************************************************************/

#ifndef WIGNER2_GSL_H_
#define WIGNER2_GSL_H_

#include <stdexcept>

#include <gsl/gsl_sf_coupling.h>

#include "am.h"

namespace am {

  // angular momentum hat symbol

  inline
    double Hat2(int two_j)
  {
    return sqrt(static_cast<double>(two_j+1));
  }


  // phase sign (-)^sum

  inline
    int ParitySign2(int two_sum)
  {
    if ((two_sum%2)!=0) throw std::domain_error("two_sum not even");
    int remainder = abs(two_sum/2) % 2;
    int sign = 1 - 2*remainder;
    return sign;
  }

  // Wigner3J(ja,jb,jc,ma,mb,mc)
  //   returns Wigner 3-J symbol
  //   wrapper for gsl_sf_coupling_3j

  inline
    double Wigner3J2(
		     int two_ja, int two_jb, int two_jc,
		     int two_ma, int two_mb, int two_mc
		     )
  {
    return gsl_sf_coupling_3j(
			      two_ja, two_jb, two_jc,
			      two_ma, two_mb, two_mc
			      );
  }

  // ClebschGordan(ja,ma,jb,mb,jc,mc)
  //   returns Clebsch-Gordan coefficient
  //   wrapper for gsl_sf_coupling_3j

  inline
    double ClebschGordan2(
			  int two_ja, int two_ma,
			  int two_jb, int two_mb,
			  int two_jc, int two_mc
			  )
  {
    return Hat2(two_jc)*ParitySign2(two_ja-two_jb+two_mc)
      *gsl_sf_coupling_3j(
			  two_ja, two_jb, two_jc,
			  two_ma, two_mb, -two_mc
			  );
  }

  // Wigner6J(ja,jb,jc,jd,je,jf)
  //   returns Wigner 6-J symbol
  //   wrapper for gsl_sf_coupling_6j

  inline
    double Wigner6J2(
		     int two_ja, int two_jb, int two_jc,
		     int two_jd, int two_je, int two_jf
		     )
  {
    return gsl_sf_coupling_6j(
			      two_ja, two_jb, two_jc,
			      two_jd, two_je, two_jf
			      );
  }

  // Unitary6J(ja,jb,jc,jd,je,jf)
  //   wrapper for gsl_sf_coupling_6j
  //   returns unitary recoupling symbol for (12)3-1(23) recoupling

  inline
    double Unitary6J2(
		      int two_ja, int two_jb, int two_jc,
		      int two_jd, int two_je, int two_jf
		      )
  {
    return ParitySign2(two_ja+two_jb+two_jd+two_je)*Hat2(two_jc)*Hat2(two_jf)*gsl_sf_coupling_6j(
												 two_ja, two_jb, two_jc,
												 two_jd, two_je, two_jf
												 );
  }

  // Wigner9J(ja,jb,jc,jd,je,jf,jg,jh,ji)
  //   returns Wigner 9-J symbol
  //   wrapper for gsl_sf_coupling_9j

  inline
    double Wigner9J2(
		     int two_ja, int two_jb, int two_jc,
		     int two_jd, int two_je, int two_jf,
		     int two_jg, int two_jh, int two_ji
		     )
  {
    return gsl_sf_coupling_9j(
			      two_ja, two_jb, two_jc,
			      two_jd, two_je, two_jf,
			      two_jg, two_jh, two_ji
			      );
  }

  // Unitary9J(ja,jb,jc,jd,je,jf,jg,jh,ji)
  //   returns unitary 9-J symbol
  //   wrapper for gsl_sf_coupling_9j

  inline
    double Unitary9J2(
		      int two_ja, int two_jb, int two_jc,
		      int two_jd, int two_je, int two_jf,
		      int two_jg, int two_jh, int two_ji
		      )
  {
    return Hat2(two_jc)*Hat2(two_jf)*Hat2(two_jg)*Hat2(two_jh)
      *gsl_sf_coupling_9j(
			  two_ja, two_jb, two_jc,
			  two_jd, two_je, two_jf,
			  two_jg, two_jh, two_ji
			  );
  }

} // namespace
#endif
