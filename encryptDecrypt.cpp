#pragma once
/*
 * Author: Darrshini
 * Date: 10/10/2025
 * Description: This is a shared header file for other cpp files
 */

#include "rsa.h"
#include <gmp.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <random>
#include <Windows.h>   // For Sleep()
#include <conio.h>     // For _getch()
#include <cryptlib.h>
#include <sha.h>
#include <hex.h>
#include <filters.h>

using namespace CryptoPP;
using CryptoByte = CryptoPP::byte;
using namespace std;
namespace fs = std::filesystem;

// --- Global variable to store temporary password in memory ---
string g_tempPassword;

// --- Hidden folder and file paths ---
const string hiddenFolder = ".security";
const string rsaHashFile = hiddenFolder + "/password_hash_RSA.txt";
const string tempHashFile = "temp_pass_RSA.txt";

// --- Hex/byte utilities ---
vector<uint8_t> hexToBytes(const string& hex) {
    vector<uint8_t> bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        string byteString = hex.substr(i, 2);
        bytes.push_back((uint8_t)stoi(byteString, nullptr, 16));
    }
    return bytes;
}

string bytesToHex(const vector<uint8_t>& bytes) {
    stringstream ss;
    ss << hex << setfill('0');
    for (auto b : bytes)
        ss << setw(2) << (int)b;
    return ss.str();
}

bool isValidHex(const string& str) {
    for (char c : str) if (!isxdigit(c)) return false;
    return true;
}

// --- SHA-256 hashing ---
string hashPassword(const string& password) {
    SHA256 hash;
    CryptoByte digest[SHA256::DIGESTSIZE];
    hash.Update((const CryptoByte*)password.data(), password.size());
    hash.Final(digest);

    string hexDigest;
    HexEncoder encoder(new StringSink(hexDigest));
    encoder.Put(digest, sizeof(digest));
    encoder.MessageEnd();
    return hexDigest;
}

// --- Save hashed password temporarily ---
void saveTemporaryPlainPassword(const string& password) {
    const string tempPlainFile = "temp_pass_RSA_plain.txt";
    ofstream tempOut(tempPlainFile, ios::out | ios::trunc);
    if (tempOut) tempOut << password << endl;
    tempOut.close();

    cout << "\n[INFO] Temporary plain password saved to '" << tempPlainFile
        << "' for 30 seconds. Please save it elsewhere!\n";

    for (int i = 30; i >= 1; i--) {
        cout << "\rTime left: " << i << " seconds " << flush;
        Sleep(1000);
    }

    if (fs::exists(tempPlainFile))
        fs::remove(tempPlainFile);

    smoothClear();
    cout << "[INFO] Temporary plain password file removed for security.\n\n";
}

// --- Save hashed password persistently ---
void saveHashedPassword(const string& password) {
    if (!fs::exists(hiddenFolder)) fs::create_directory(hiddenFolder);
    string hash = hashPassword(password);
    ofstream out(rsaHashFile, ios::out | ios::trunc);
    if (out) { out << hash << endl; out.close(); }
}

// --- Verify password ---
bool verifyPassword(const string& input) {
    if (!fs::exists(rsaHashFile)) {
        cout << "\n[ERROR] Hidden password hash not found!\n";
        return false;
    }
    ifstream hashIn(rsaHashFile);
    string storedHash; getline(hashIn, storedHash); hashIn.close();
    string inputHash = hashPassword(input);
    return (inputHash == storedHash);
}

// --- Encrypt AES key ---
void encryptAESKey() {
    if (!fileExists("public_key.txt")) {
        cout << "\n[ERROR] Public key not found!\nPlease generate public key first (Option 1).\n";
        return;
    }

    ifstream in("public_key.txt");
    string n_value, e_value;
    getline(in, n_value); getline(in, e_value); in.close();
    mpz_set_str(rsa.n, n_value.c_str(), 16);
    mpz_set_str(rsa.e, e_value.c_str(), 16);

    string aesKey; bool validInput = false;
    while (!validInput) {
        cout << "\nEnter AES-128 Key (32 hex): "; cin >> aesKey;
        aesKey.erase(remove(aesKey.begin(), aesKey.end(), ' '), aesKey.end());

        if (!isValidHex(aesKey)) { cout << "[ERROR] Invalid input! Only hexadecimal (0-9, A-F).\n"; continue; }
        if (aesKey.length() > 32) { cout << "[WARNING] Key longer than 128 bits. Reduced to 32 characters.\n"; aesKey = aesKey.substr(0, 32); }
        validInput = true;
    }

    transform(aesKey.begin(), aesKey.end(), aesKey.begin(), ::toupper);
    if (aesKey.length() % 2 != 0) aesKey = "0" + aesKey;

    cout << "\n[INFO] Original AES Key: " << aesKey << endl;
    cout << "[INFO] Encrypting using RSA...\n";

    vector<uint8_t> aesBytes = hexToBytes(aesKey);
    while (aesBytes.size() < 16) aesBytes.insert(aesBytes.begin(), 0x00);
    aesBytes.insert(aesBytes.begin(), 0x01); // padding

    mpz_import(rsa.m, aesBytes.size(), 1, 1, 1, 0, aesBytes.data());
    if (mpz_cmp(rsa.m, rsa.n) >= 0) { cout << "[ERROR] AES key exceeds RSA modulus!\n"; return; }

    mpz_powm(rsa.cipher, rsa.m, rsa.e, rsa.n);
    char* cipher_str = mpz_get_str(NULL, 16, rsa.cipher);

    ofstream out("encrypted_aes.txt"); out << cipher_str << endl; out.close();

    g_tempPassword = generateRandomPassword(10);
    saveHashedPassword(g_tempPassword);
    saveTemporaryPlainPassword(g_tempPassword);

    cout << "\n[INFO] AES key encrypted successfully!\n";
    cout << "Encrypted Ciphertext:\n" << cipher_str << endl;

    printSeparator();
    cout << "Encrypted AES Key Saved to: 'encrypted_aes.txt'\n";
    printSeparator();

    free(cipher_str);
}

// --- Decrypt AES key ---
void decryptAESKey() {
    printSeparator(); cout << "       AES KEY DECRYPTION\n"; printSeparator();

    if (!fileExists("private_key.txt")) { cout << "\n[ERROR] Private key not found!\n"; return; }

    ifstream in("private_key.txt");
    string n_value, d_value;
    getline(in, n_value); getline(in, d_value); in.close();
    mpz_set_str(rsa.n, n_value.c_str(), 16);
    mpz_set_str(rsa.d, d_value.c_str(), 16);

    if (!fileExists("encrypted_aes.txt")) { cout << "\n[ERROR] Encrypted AES key not found!\n"; return; }

    string enteredPassword; cout << "\nEnter the saved decryption password: ";
    enteredPassword = getMaskedPassword(); cout << endl;

    if (!verifyPassword(enteredPassword)) {
        cout << "\n[ERROR] Incorrect password! Cannot decrypt AES key.\n";
        return;
    }

    ifstream enc("encrypted_aes.txt");
    string ciphertext; getline(enc, ciphertext); enc.close();

    mpz_set_str(rsa.cipher, ciphertext.c_str(), 16);
    cout << "\n[INFO] Encrypted Ciphertext:\n" << ciphertext << endl;
    cout << "[INFO] Decrypting using RSA...\n\n";

    mpz_powm(rsa.m, rsa.cipher, rsa.d, rsa.n);

    size_t count = 0;
    vector<uint8_t> outBytes((mpz_sizeinbase(rsa.m, 2) + 7) / 8);
    mpz_export(outBytes.data(), &count, 1, 1, 1, 0, rsa.m);
    outBytes.resize(count);

    if (!outBytes.empty() && outBytes[0] == 0x01) outBytes.erase(outBytes.begin());
    while (outBytes.size() < 16) outBytes.insert(outBytes.begin(), 0x00);

    if (outBytes.size() > 16) {
        ptrdiff_t startIndex = static_cast<ptrdiff_t>(outBytes.size()) - 16;
        outBytes = vector<uint8_t>(outBytes.begin() + startIndex, outBytes.end());
    }

    string finalAES = bytesToHex(outBytes);
    transform(finalAES.begin(), finalAES.end(), finalAES.begin(), ::toupper);

    printSeparator();
    cout << "[SUCCESS] Decryption completed!\n";
    cout << "Decrypted AES Key: " << finalAES << endl;
    printSeparator();
}

void encryptDecryptMenu()
{
    int choice;

    while (true)
    {
        smoothClear();   // *** New smooth clear ***

        printSeparator();
        cout << " RSA ENCRYPT & DECRYPT MENU\n";
        printSeparator();
        cout << "SUB-MENU OPTIONS:\n";
        cout << "1. Encrypt AES Key\n";
        cout << "2. Decrypt AES Key\n";
        cout << "3. Back to Main menu\n";
        printSeparator();
        cout << "\nEnter your choice: ";

        cin >> choice;

        // Prevent menu breaking from invalid input
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\n[ERROR] Invalid input! Pls enter a number.\n";
            Sleep(1200);
            continue;
        }

        cin.ignore(1000, '\n'); // Clear any leftover input

        smoothClear();   // *** Clear before showing results ***

        switch (choice)
        {
        case 1:
            encryptAESKey();
            cout << "\nPress ENTER to return to menu...";
            cin.get();
            break;

        case 2:
            decryptAESKey();
            cout << "\nPress ENTER to return to menu...";
            cin.get();
            break;

        case 3:
            return;

        default:
            cout << "[ERROR] Invalid choice! Pls enter 1–3.\n";
            Sleep(1200);
            break;
        }
    }
}
