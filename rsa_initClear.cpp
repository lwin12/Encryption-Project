
/*
 * Author: Darrshini
 * Date: 11/10/2025
 * Description: This is for initializing mpt_z variables and clearing memory
 */

#include "rsa.h"


RSAKey rsa;


void initRSAKey() {
    mpz_inits(rsa.p, rsa.q, rsa.n, rsa.e, rsa.d, rsa.phi, rsa.p1, rsa.q1, rsa.m, rsa.cipher, NULL);
}


void clearRSAKey() {
    mpz_clears(rsa.p, rsa.q, rsa.n, rsa.e, rsa.d, rsa.phi, rsa.p1, rsa.q1, rsa.m, rsa.cipher, NULL);
}
