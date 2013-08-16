/**
 * 	Use The GNU Multiple Precision Arithmetic Library 5.1.2 to deal with the big numbers
 *
 * 	This head file is used for referring the GMP Library and rename the big number classes
 *
 */

#include "useGMP.h"

bigInteger countGCD(const bigInteger &a, const bigInteger &b)
{
	bigInteger c;
	mpz_gcd (c.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t());
	return c;
}

bigInteger countLCM(const bigInteger &a, const bigInteger &b)
{
	bigInteger c;
	mpz_lcm (c.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t());
	return c;
}

bigInteger realToInt(const bigReal &r)
{
	bigInteger ans;
	mpz_set_f(ans.get_mpz_t(), r.get_mpf_t());
	return ans;
}
