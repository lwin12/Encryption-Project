
/*
 * Author: Darrshini
 * Date: 10/10/2025
 * Description: This program has functions for encrypting and decrypting AES key
 */

#include "rsa.h"


using namespace std;

// convert hex string to byte vector
 vector<uint8_t> hexToBytes(const string& hex) {
    vector<uint8_t> bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        string byteString = hex.substr(i, 2);
        bytes.push_back((uint8_t)stoi(byteString, nullptr, 16));
    }
    return bytes;
}

// convert byte vector to hex string
 string bytesToHex(const vector<uint8_t>& bytes) {
    stringstream ss;
    ss << hex << setfill('0');
    for (auto b : bytes)
        ss << setw(2) << (int)b;
    return ss.str();
}

 bool isValidHex(const string& str) {
    for (char c : str) {
        if (!isxdigit(c))
            return false;
    }
    return true;
}


void encryptAESKey() {
   
    if (!fileExists("public_key.txt")) {
        cout << "\n[ERROR] Public key not found!\n";
        cout << "Pls generate public key first (Option 1).\n";
        return;
    }

    ifstream in("public_key.txt");

    string n_value, e_value;
    getline(in, n_value);
    getline(in, e_value);
    in.close();

    mpz_set_str(rsa.n, n_value.c_str(), 16);
    mpz_set_str(rsa.e, e_value.c_str(), 16);

    string aesKey;
    bool validInput = false;

    while (!validInput) {
        cout << "\nEnter AES-128 Key (32 hex): ";
        cin >> aesKey;

        // Remove spaces if any
        aesKey.erase(remove(aesKey.begin(), aesKey.end(), ' '), aesKey.end());

        // Validate hex input
        if (!isValidHex(aesKey)) {
            cout << "[ERROR] Invalid input! Pls enter only hexadecimal characters (0-9, A-F).\n";
            continue;
        }

        // Warn if key is too long
        if (aesKey.length() > 32) {
            cout << "[WARNING] Key is longer than 128 bits. It will be reduced to 32 characters.\n";
            aesKey = aesKey.substr(0, 32);
        }

        validInput = true;
    }


    transform(aesKey.begin(), aesKey.end(), aesKey.begin(), ::toupper);
    if (aesKey.length() % 2 != 0)
        aesKey = "0" + aesKey; // ensure even length

    cout << "\n[INFO] Original AES Key: " << aesKey << endl;
    cout << "\n[INFO] Encrypting using RSA...\n";


    // Convert to byte vector
    vector<uint8_t> aesBytes = hexToBytes(aesKey);

    // add a pad of 16 bytes for AES-128 
    while (aesBytes.size() < 16)
        aesBytes.insert(aesBytes.begin(), 0x00);

    // Add simple padding byte 0x01 at the start
    aesBytes.insert(aesBytes.begin(), 0x01);
    
    // Import padded bytes into mpz_t
    mpz_import(rsa.m, aesBytes.size(), 1, 1, 1, 0, aesBytes.data());

    // ensure m < n -- m value bineg larger than n happens in rare cases but good to check 
    if (mpz_cmp(rsa.m, rsa.n) >= 0) {
        cout << "\n[ERROR] AES key value excceds RSA modulus!\n";
        cout << "\n[ERROR] Generate larger RSA keys (change the keygen values to larger one in code).\n";
        return;
    }
    // Encrypt
    mpz_powm(rsa.cipher, rsa.m, rsa.e, rsa.n);

    char* cipher_str = mpz_get_str(NULL, 16, rsa.cipher);
    ofstream out("encrypted_aes.txt");
    out << cipher_str << endl;
    out.close();
    

    printSeparator();
    cout << "[SUCCESS] AES key encrypted successfully!\n";
    cout << "\nEncrypted Ciphertext:\n" << cipher_str << endl;
    cout << "\nSaved to: 'encrypted_aes.txt'\n";
    printSeparator();
    free(cipher_str);
}

void decryptAESKey() {

    printSeparator();
    cout << "       AES KEY DECRYPTION\n";
    printSeparator();

    // Check if private key exists
    if (!fileExists("private_key.txt")) {
        cout << "\n[ERROR] Private key not found!\n";
        cout << "Please generate private key first (Option 2 from main menu).\n";
        return;
    }

    ifstream in("private_key.txt");
   
    string n_value, d_value;
    getline(in, n_value);
    getline(in, d_value);
    in.close();

    mpz_set_str(rsa.n, n_value.c_str(), 16);
    mpz_set_str(rsa.d, d_value.c_str(), 16);

    // check if encrypted aes key text file exists
    if (!fileExists("encrypted_aes.txt")) {
        cout << "\n[ERROR] Encrypted AES key not found!\n";
        cout << "Please encrypt an AES key first.\n";
        return;
    }

    ifstream enc("encrypted_aes.txt");
    string ciphertext;
    getline(enc, ciphertext);
    enc.close();

    mpz_set_str(rsa.cipher, ciphertext.c_str(), 16);


    cout << "\n[INFO] Encrypted Ciphertext:\n" << ciphertext << endl;
    cout << "\n[INFO] Decrypting using RSA...\n\n";

    // Decrypt to rsa.m
    mpz_powm(rsa.m, rsa.cipher, rsa.d, rsa.n);

    // Export decrypted number to bytes
    size_t count = 0;
    //set correct buffer size, it if is too small it can cause data loss in the exported bytes which can cause mistmatch in the output
    vector<uint8_t> outBytes((mpz_sizeinbase(rsa.m, 2) + 7) / 8);
    mpz_export(outBytes.data(), &count, 1, 1, 1, 0, rsa.m);
    outBytes.resize(count);


    // Remove padding byte (0x01)
    if (!outBytes.empty() && outBytes[0] == 0x01)
        outBytes.erase(outBytes.begin());
    

    // Trim or pad to 16 bytes
    while (outBytes.size() < 16)
        outBytes.insert(outBytes.begin(), 0x00);
    if (outBytes.size() > 16)
        outBytes = vector<uint8_t>(outBytes.end() - 16, outBytes.end());

    // Convert bytes to hex
    string finalAES = bytesToHex(outBytes);
    transform(finalAES.begin(), finalAES.end(), finalAES.begin(), ::toupper);

    printSeparator();
    cout << "[SUCCESS] Decryption completed!\n";
    cout << "\nDecrypted AES Key: " << finalAES << endl;
    printSeparator();
}

void encryptDecryptMenu() {
    int choice;
    do {
        cout << "\n";
        printSeparator();
        cout << "           RSA ENCRYPT & DECRYPT MENU\n";
        printSeparator();
        cout << "SUB-MENU OPTIONS: \n";
        cout << "1. Encrypt AES Key\n";
        cout << "2. Decrypt AES Key\n";
        cout << "3. Back to Main menu\n";
        printSeparator();
        cout << "\nEnter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: 
            encryptAESKey(); 
            break;
        case 2: 
            decryptAESKey(); 
            break;
        case 3: 
            return;
        default:
            cout << "[ERROR] Invalid choice! Pls enter 1-3.\n";
            cin.clear();
            cin.ignore(100, '\n');
        }
    } while (choice != 3);
}