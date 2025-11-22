
#include "rijndael.h"
#include "modes.h"
#include "files.h"
#include "osrng.h"
#include "hex.h"
#include "pwdbased.h"
#include <sha.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include <conio.h>
#include <iomanip>
#include <filesystem>

using namespace CryptoPP;
using CryptoByte = CryptoPP::byte;
using namespace std;
namespace fs = std::filesystem;

// --- Helpers ---
string getMaskedInput(const string& prompt) {
    cout << prompt;
    string input; int ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!input.empty()) { input.pop_back(); cout << "\b \b"; }
        }
        else if (ch == 0 || ch == 224) _getch();
        else { input.push_back((char)ch); cout << '*'; }
    }
    cout << endl;
    return input;
}

string generateRandomPasswordHex() {
    AutoSeededRandomPool prng;
    SecByteBlock pwd(16);
    prng.GenerateBlock(pwd, pwd.size());
    string hexPwd;
    HexEncoder encoder(new StringSink(hexPwd));
    encoder.Put(pwd, pwd.size());
    encoder.MessageEnd();
    return hexPwd;
}

string sha256Hex(const string& s) {
    CryptoByte digest[SHA256::DIGESTSIZE];
    SHA256().CalculateDigest(digest, (const CryptoByte*)s.data(), s.size());
    string hexDigest;
    HexEncoder encoder(new StringSink(hexDigest));
    encoder.Put(digest, sizeof(digest));
    encoder.MessageEnd();
    return hexDigest;
}

void secureWipe(SecByteBlock& b) {
    volatile CryptoByte* p = b.BytePtr();
    for (size_t i = 0; i < b.size(); ++i) p[i] = 0;
}

void secureWipe(string& s) {
    if (!s.empty()) {
        volatile char* p = &s[0];
        for (size_t i = 0; i < s.size(); ++i) p[i] = 0;
        s.clear();
    }
}

void countdownOneLine(int seconds, const string& prefix = "Time left: ") {
    for (int i = seconds; i > 0; --i) {
        cout << "\r" << prefix << i << " seconds   " << flush;
        this_thread::sleep_for(chrono::seconds(1));
    }
    cout << "\r" << string(prefix.size() + 20, ' ') << "\r" << flush;
}

// --- PBKDF2 derive ---
void deriveAESKey(const string& password, const CryptoByte* salt, SecByteBlock& key, SecByteBlock& iv) {
    PKCS5_PBKDF2_HMAC<SHA256> pbkdf;
    pbkdf.DeriveKey(key, key.size(), 0, (CryptoByte*)password.data(), password.size(), salt, 16, 50000);
    pbkdf.DeriveKey(iv, iv.size(), 0, (CryptoByte*)password.data(), password.size(), salt, 16, 60000);
}

// --- Ensure hidden folder exists ---
void ensureSecurityFolder() {
    fs::path dir(".security");
    if (!fs::exists(dir)) fs::create_directory(dir);
}

// --- AES Encryption ---
void aesEncrypt() {
    ensureSecurityFolder();

    string plaintext;
    try { FileSource fs("input.txt", true, new StringSink(plaintext)); }
    catch (...) { cout << "[ERROR] Could not read input.txt\n"; return; }

    string password = generateRandomPasswordHex();
    AutoSeededRandomPool prng;
    CryptoByte salt[16]; prng.GenerateBlock(salt, sizeof(salt));

    SecByteBlock key(AES::DEFAULT_KEYLENGTH), iv(AES::BLOCKSIZE);
    deriveAESKey(password, salt, key, iv);

    string ciphertext;
    try {
        CBC_Mode<AES>::Encryption enc; enc.SetKeyWithIV(key, key.size(), iv);
        StringSource ss(plaintext, true, new StreamTransformationFilter(enc, new StringSink(ciphertext)));
    }
    catch (...) {
        cout << "[ERROR] Encryption failed\n";
        secureWipe(key); secureWipe(iv); secureWipe(password);
        return;
    }

    string encodedSalt;
    StringSource ss1(salt, sizeof(salt), true, new HexEncoder(new StringSink(encodedSalt)));
    string finalOutput = encodedSalt + ciphertext;

    try {
        FileSink out("input.txt");
        out.Put((const CryptoByte*)finalOutput.data(), finalOutput.size());
    }
    catch (...) {
        cout << "[ERROR] Could not write encrypted input.txt\n";
        secureWipe(key); secureWipe(iv); secureWipe(password);
        return;
    }

    // --- Save persistent hashed password ---
    string passwordHashHex = sha256Hex(password);
    ofstream h(".security/password_hash_AES.txt", ios::out | ios::trunc);
    if (h) h << passwordHashHex << endl;

    // --- Save temporary plaintext password for 15s demo ---
    const string tempFile = "temp_pass_AES.txt";
    ofstream p(tempFile, ios::out | ios::trunc);
    if (p) p << password << endl;

    cout << "\n[INFO] Encryption complete!\nCiphertext replaced input.txt\n";
    cout << "[INFO] Temporary plaintext password file: " << tempFile << " (visible for 30 seconds)\n";
    cout << "[INFO] Persistent hashed password stored in: .security/password_hash_AES.txt\n";
    cout << "[INFO] Derived AES key (for RSA encryption): ";
    string aesKeyHex;
    StringSource ss2(key, key.size(), true, new HexEncoder(new StringSink(aesKeyHex)));
    cout << aesKeyHex << "\n\n";

    // --- Live 30-sec countdown and automatic deletion ---
    for (int i = 30; i > 0; --i) {
        cout << "\r[SECURITY] Move file within " << i << " seconds   " << flush;
        this_thread::sleep_for(chrono::seconds(1));
    }

    // Ensure file stream is closed before deletion
    p.close();

    if (fs::exists(tempFile)) {
        try {
            fs::remove(tempFile);
            cout << "\n[INFO] Temporary plaintext password file deleted.\n";
        }
        catch (const fs::filesystem_error& e) {
            cout << "\n[WARNING] Could not delete temporary password file: " << e.what() << "\n";
        }
    }

    secureWipe(key); secureWipe(iv); secureWipe(password); secureWipe(aesKeyHex);
}

// --- AES Decryption ---
void aesDecrypt() {
    // Check temp file first (demo)
    string pw;
    const string tempFile = "temp_pass_AES.txt";
    if (fs::exists(tempFile)) {
        ifstream tmp(tempFile); getline(tmp, pw); tmp.close();
        cout << "[INFO] Using temporary password from file.\n";
    }
    else {
        pw = getMaskedInput("Enter password to decrypt (masked): ");
    }

    string storedHash;
    ifstream h(".security/password_hash_AES.txt");
    if (!h) { cout << "[ERROR] Persistent hash file missing.\n"; return; }
    getline(h, storedHash); h.close();

    if (sha256Hex(pw) != storedHash) {
        cout << "[ERROR] Password does not match stored hash. Aborting.\n";
        secureWipe(pw); return;
    }

    countdownOneLine(3, "[SECURITY] Verifying ");

    string fullData;
    try { FileSource fs("input.txt", true, new StringSink(fullData)); }
    catch (...) { cout << "[ERROR] Could not read input.txt\n"; secureWipe(pw); return; }

    if (fullData.size() < 32) { cout << "[ERROR] Encrypted file too short.\n"; secureWipe(pw); return; }

    string encodedSalt = fullData.substr(0, 32);
    string ciphertext = fullData.substr(32);
    CryptoByte salt[16]; HexDecoder decoder(new ArraySink(salt, sizeof(salt)));
    decoder.Put((const CryptoByte*)encodedSalt.data(), encodedSalt.size());
    decoder.MessageEnd();

    SecByteBlock key(AES::DEFAULT_KEYLENGTH), iv(AES::BLOCKSIZE);
    deriveAESKey(pw, salt, key, iv);

    string aesKeyInput = getMaskedInput("Enter AES key (from RSA decryption) [masked]: ");
    string derivedHex; StringSource ss3(key, key.size(), true, new HexEncoder(new StringSink(derivedHex)));
    if (aesKeyInput != derivedHex) {
        cout << "\n[ERROR] AES key mismatch.\n"; secureWipe(key); secureWipe(iv); secureWipe(pw); return;
    }

    try {
        string recovered; CBC_Mode<AES>::Decryption dec; dec.SetKeyWithIV(key, key.size(), iv);
        StringSource ss4(ciphertext, true, new StreamTransformationFilter(dec, new StringSink(recovered)));
        FileSink out("decrypted_output.txt"); out.Put((const CryptoByte*)recovered.data(), recovered.size());
        cout << "\n[INFO] Decryption successful! Plaintext stored in 'decrypted_output.txt'\n";
    }
    catch (...) { cout << "\n[ERROR] Decryption failed.\n"; secureWipe(key); secureWipe(iv); secureWipe(pw); return; }

    secureWipe(key); secureWipe(iv); secureWipe(pw);
}

// --- Main ---
int main() {
    cout << "=============================\n";
    cout << " GROUP 7 AES Encryption Tool \n";
    cout << "=============================\n";
    cout << "1. Encrypt\n2. Decrypt\nChoose option: ";

    int choice; if (!(cin >> choice)) { cout << "Invalid input.\n"; return 1; }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 1) aesEncrypt();
    else if (choice == 2) aesDecrypt();
    else cout << "Invalid option.\n";

    return 0;
}
