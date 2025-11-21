#pragma once

/*
 * Author: Darrshini
 * Date: 10/10/2025
 * Description: This is a shared header file for other cpp files
 */

#ifndef RSA_HEADER_H
#define RSA_HEADER_H

#include <gmp.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>
#include <sstream>
#include <iomanip>



using namespace std;

struct RSAKey {
	mpz_t p;
	mpz_t q;
	mpz_t n; // modulus , product of p * q
	mpz_t e; // exponent value
	mpz_t d; // inverse mod of e
	mpz_t phi;
	mpz_t p1; // store value of p - 1
	mpz_t q1; // store value of q - 1
	mpz_t m; // AES key value
	mpz_t cipher; // store ciphertext
};



extern RSAKey rsa;
void displayStatus();
string getFileTimestamp(const string& filename);
void showMenu();
void generatePublicKey();
void generatePrivateKey();
void encryptAESKey();
void decryptAESKey();
void encryptDecryptMenu();
vector<uint8_t> hexToBytes(const string& hex);
string bytesToHex(const vector<uint8_t>& bytes);
bool isValidHex(const string& str);
void initRSAKey(); // remember to initialize ya
void clearRSAKey(); // clear the damn memory!
bool keysMatch();



static inline bool fileExists(const string& filename) {
	ifstream file(filename);
	return file.good();
}

static inline void printSeparator() {
	cout << string(50, '=') << endl;
}

#endif 