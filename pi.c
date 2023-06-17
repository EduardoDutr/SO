#include "pi.h"

char* getPi(int digits) {
    mpfr_t pi;
    mpfr_prec_t precision = digits * 3.34;
    mpfr_init2(pi, precision);
    mpfr_const_pi(pi, MPFR_RNDN);

    char* pi_str = (char*) malloc((digits + 3) * sizeof(char));
    mpfr_sprintf(pi_str, "%.*Rf", digits, pi);

    mpfr_clear(pi);
    return pi_str;
}
