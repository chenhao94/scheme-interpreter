/**
 * 	Use The GNU Multiple Precision Arithmetic Library 5.1.2 to deal with the big numbers
 *
 * 	This head file is used for referring the GMP Library and rename the big number classes
 *
 */
#ifndef __USEGMP_H___
#define __USEGMP_H___

#include <gmpxx.h>

typedef mpz_class bigInteger;	// intergers
typedef mpq_class bigRational;	// exact rational numbers 	(fraction)
typedef mpf_class bigReal;		// inexact real numbers		(float)

#endif

