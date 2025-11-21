
/*
 * Author: Darrshini
 * Date: 10/10/2025
 * Description: Program for main menu
 */
#include "rsa.h"



void showMenu() {
	int choice;

	do {
		printSeparator();
		cout << "         GROUP 7 RSA CRYPTO SYSTEM :)\n";
		cout << "    Welcome to our AES Key Encryption System!\n";
		printSeparator();
		displayStatus();
		printSeparator();
		cout << "MENU OPTIONS: \n";
		cout << "1. Generate Public Key\n";
		cout << "2. Generate Private Key\n";
		cout << "3. Encrypt/Decrypt AES Key\n";
		cout << "4. Exit program\n";
		printSeparator();
		cout << "Enter your choice : ";

		cin >> choice;
		cin.ignore();

		switch (choice) {
		case 1:
			generatePublicKey();
			break;

		case 2:
			generatePrivateKey();
			break;

		case 3:
			if (!fileExists("public_key.txt") && !fileExists("private_key.txt")) {
				cout << "\n [ERROR] No Keys Found! Pls Generate Keys first.\n";
			}
			encryptDecryptMenu();
			break;

		case 4:
			cout << "\nExiting program ...\n";
			break;

		default:
			cout << "\n [ERROR] Invalid choice! Pls enter options 1-4\n";
			cin.clear();
			cin.ignore(100, '\n');

		}


	} while (choice != 4);


}

int main() {

	initRSAKey(); // initialize GMP integers
	showMenu();
	clearRSAKey(); // clear memory after GMP integers are used and program exits
	return 0;
}

