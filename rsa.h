#pragma once
/*
 * Author: Darrshini
 * Date: 10/10/2025
 * Description: This is a shared header file for other cpp files
 */
#ifndef RSA_HEADER_H
#define RSA_HEADER_H

 // ===== DISABLE GMP WARNING FOR MSVC =====
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4146)
#endif

#include <gmp.h>

#ifdef _MSC_VER
#pragma warning(pop)
#endif
// ===== END GMP WARNING SUPPRESSION =====

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>
#include <sstream>
#include <iomanip>
#include <random>
#include <conio.h>
#include <Windows.h>
#include <sha.h>
#include <hex.h>
#include <filters.h>
#include <chrono>
#include <thread>


#ifdef _WIN32
#define getch _getch
#endif

using namespace std;

struct RSAKey {
    mpz_t p;
    mpz_t q;
    mpz_t n;
    mpz_t e;
    mpz_t d;
    mpz_t phi;
    mpz_t p1;
    mpz_t q1;
    mpz_t m;
    mpz_t cipher;
};

extern RSAKey rsa;

// Function declarations
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
void initRSAKey();
void clearRSAKey();
bool keysMatch();
string hashPassword(const string& password);
void saveTemporaryPlainPassword(const string& password);
void saveHashedPassword(const string& password);

static inline bool fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

static inline void printSeparator() {
    cout << string(50, '=') << endl;
}

inline string generateRandomPassword(int length = 8) {
    const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, chars.size() - 1);
    string password;
    for (int i = 0; i < length; i++)
        password += chars[dis(gen)];
    return password;
}

inline string getMaskedPassword() {
    string password;
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";
            }
        }
        else {
            password.push_back(ch);
            cout << '*';
        }
    }
    cout << endl;
    return password;
}

inline void smoothClear() {
    cout << "\033[2J\033[H";
}

#endif // RSA_HEADER_H