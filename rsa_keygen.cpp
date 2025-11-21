
/*
 * Author: Darrshini
 * Date: 10/10/2025
 * Description:This file handles the public and private key generation funtions
 */

#include "rsa.h"
using namespace std;


void generatePublicKey() {

	printSeparator();
	cout << "     PUBLIC KEY GENERATION\n";
	printSeparator();

	if (fileExists("public_key.txt")) {
		char overwrite;
		cout << "\n[WARNING] Public key already exists!\n";
		cout << "Do u want to overwrite? (y/n): ";
		cin >> overwrite;
		cin.ignore();

		if (tolower(overwrite) != 'y') {
			cout << "\n[ABORT] Key generation cancelled!\n\n";
			return;
		}
	}

	cout << "\n[INFO] Generating RSA-1024 key pair...\n";


	gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, static_cast<unsigned long>(time(NULL)));

	//generate random prime numbers p and q
	

	mpz_urandomb(rsa.p, state, 1024); // generates a random integer of 1024 bits and stored into p
	mpz_nextprime(rsa.p, rsa.p);     // takes current p and finds the next prime number greater or equal to it
	mpz_urandomb(rsa.q, state, 1024);
	mpz_nextprime(rsa.q, rsa.q);

	// we will use this loop to check the values generated, incase the modular inverse fails ( when value generated for p == q)

	while (mpz_cmp(rsa.p, rsa.q) == 0) {
		mpz_urandomb(rsa.q, state, 1024);
		mpz_nextprime(rsa.q, rsa.q);
	}

	/*
	* ^ after this both p and q are randomly chosen large primes that are about 1024 bits long.
	* when p and q are 1024 bits long each , when computing its product 'n' we generate a 1024 bit RSA key
	* NOTE : We are doing this for demo purposes but in the actual world, 1024 is considered weak, secure systems use 2048 or 4096 bit RSA keys
	*/

	// save p and q values to a hidden internal file for later private key generation
    // this allows program to compute phi and private key even after restarting
	ofstream pq("pq_values.txt");
	pq << mpz_get_str(NULL, 16, rsa.p) << endl;
	pq << mpz_get_str(NULL, 16, rsa.q) << endl;
	pq.close();



	// calculate 'n' the product of primes = p * q . This product value will be part of public key
	mpz_mul(rsa.n, rsa.p, rsa.q);

	/*
	* lo and behold the Euler totient applicationnn:
	* Calculating Euler Totient function Φ(n) as Φ(n) = Φ(p * q) = Φ(p) * Φ(q) = (p - 1) * (q - 1)  -- got formula from : https://www.geeksforgeeks.org/computer-networks/rsa-algorithm-cryptography/    
	* first compute phi value = (p-1)*(q-1):
	*/


	mpz_sub_ui(rsa.p1, rsa.p, 1); //mpz_sub_ui is a function that subtacts 1 from a GMP number which is 'p' so p1 = p - 1
	mpz_sub_ui(rsa.q1, rsa.q, 1); // doing the same as mentioned above but for GMP prime number 'q' so q1 = q - 1
	mpz_mul(rsa.phi, rsa.p1, rsa.q1); // lastly it will compute the product of (p-1)*(q-1) which is p1 * q1 to get phi value  


	// setting public exponent e to 65537
	// the value 65537 is a common default value used in RSA followed bu 3 and 17 , but 3 and 17 is too weak so we use 65537

	mpz_set_ui(rsa.e, 65537); // mpz_set_ui is a function used for setting GMP integer to an integer that is not negative (unsigned)

	char* n_str = mpz_get_str(NULL, 16, rsa.n); // function is used for converting the GMP integer n into a string of hexadecimal values, NUll is for automatically allocating memory for this string
	char* e_str = mpz_get_str(NULL, 16, rsa.e); // same is done for exponent, this value will be used for encryption so it is necessary to save this value

	// saving public key into public_key.txt file
	ofstream out("public_key.txt");
	out << n_str << endl;
	out << e_str << endl;
	out.close();

	printSeparator();
	cout << "[SUCCESS] Public key generated!\n";
	cout << "Saved to: 'public_key.txt'\n\n";
	printSeparator();


	free(n_str);
	free(e_str);
	gmp_randclear(state); // this is for clearing the random number generator state as it was used for genearting random prime numbers for p and q

}

void generatePrivateKey() {


	/*
	* this is an extra feature i thought of , asking user for password to create a private key that will be used for decryption.
	* only authorised person who knows password can generate private key and use it for decryption.
	* user is given 3 tries, if they cannot provide correct password they cannot generate private key, so they dont have key to decrypt.
	*/

	printSeparator();
	cout << "     PRIVATE KEY GENERATION\n";
	printSeparator();

	if (!fileExists("public_key.txt")) {
		cout << "\n[ERROR] Public key not found!\n";
		cout << "Pls generate public key first (Option 1).\n\n";
		return;
	}

	cout << "\n[SECURITY] Password required to generate private key\n\n";

	string password;
	int tries = 0;
	bool access = false;


	while (tries < 3) {
		cout << "Enter password: ";
		cin >> password;
		if (password == "Group7") {
			access = true;
			break;
		}
		tries++;
		cout << "\nIncorrect password. " << (3 - tries) << " tries left.\n";
	}

	if (!access) {
		cout << "\n[SECURITY ALERT] Access Denied!\n";
		cout << "\n[SECURITY ALERT] Maximum attempts exceeded!\n\n";
		return;
	}

	cout << "\n[ACCESS GRANTED]\n";

	if (fileExists("private_key.txt")) {
		char overwrite;
		cout << "\n[WARNING] Private key already exists!\n";
		cout << "Do u want to overwrite? (y/n): ";
		cin >> overwrite;
		cin.ignore();

		if (tolower(overwrite) != 'y') {
			cout << "[ABORT] Key generation cancelled!\n\n";
			return;
		}
	}
	cout << "\n[INFO] Generating private key...\n";

	// the process is almost the same as generating public key except the exponent value which we will inverse it
	// pls refer to above comments to understand how the GMP functions are used here :)

	ifstream in("public_key.txt");
	if (!in) {
		cout << "\nPublic key not found! Generate it first.\n";
		return;
	}

	string n_value, e_value;
	getline(in, n_value);
	getline(in, e_value);
	in.close();


	mpz_set_str(rsa.n, n_value.c_str(), 16);
	mpz_set_str(rsa.e, e_value.c_str(), 16);

	// load p and q from file since they may not be in memory anymore
	ifstream pq("pq_values.txt");
	if (!pq) {
		cout << "\n[ERROR]Missing p and q values!\n";
		cout << "\nPls generate public key first.\n\n";
		return;
	}
	string p_str, q_str;
	getline(pq, p_str);
	getline(pq, q_str);
	pq.close();

	mpz_set_str(rsa.p, p_str.c_str(), 16);
	mpz_set_str(rsa.q, q_str.c_str(), 16);

	// now recalculate phi using the loaded p and q
	mpz_sub_ui(rsa.p1, rsa.p, 1);
	mpz_sub_ui(rsa.q1, rsa.q, 1);
	mpz_mul(rsa.phi, rsa.p1, rsa.q1);


	/*
	* calculating the private exponent d = e^-1 mod phi
	* d is the modular inverse of e mod phi
	* mpz_invert is a function which calculates the modular inverse of e mod phi - > (d * e) mod phi = 1
	* this inversion is what undoes the encryption done using the e value (exponent we set as 65537)
	*/


	if (mpz_invert(rsa.d, rsa.e, rsa.phi) == 0) { // checks if value is 1 which is what u should get, if its 0 then no moular inverse exists , so RSA key generation fails
		cout << "[ERROR] Could not compute modular inverse!\n";
		return;
	}

	char* n_str = mpz_get_str(NULL, 16, rsa.n);
	char* d_str = mpz_get_str(NULL, 16, rsa.d);

	ofstream out("private_key.txt");
	out << n_str << endl;
	out << d_str << endl;
	out.close();

	free(n_str);
	free(d_str);

	cout << "\n[SUCCESS]Private key generated!\n";
	cout << "\nSaved to: 'private_key.txt'.\n\n";
	char choice;
	do {
		cout << "\nEncrypt an AES key now? (y/n): ";
		cin >> choice;
		choice = tolower(choice);
	} while (choice != 'y' && choice != 'n');

	if (choice == 'y')
		encryptAESKey();
	else
		return;

}
