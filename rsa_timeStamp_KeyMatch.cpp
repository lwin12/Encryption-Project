

/*
 * Author: Darrshini
 * Date: 02/11/2025
 * Description: file to store timpstamp tracking and key match tracking fucntions
 */

#include "rsa.h"
#include <sys/stat.h>
#include <ctime>

 // get file creation or modification time
	string getFileTimestamp(const string& filename) {
	struct stat fileInfo;
	if (stat(filename.c_str(), &fileInfo) != 0) {
		return "N/A";
	}

	time_t fileTime = fileInfo.st_mtime;  // Last modification time
	char buffer[80];
	struct tm timeinfo;

	if (localtime_s(&timeinfo, &fileTime) != 0) {
		return "N/A";
	}

	strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", &timeinfo);
	return string(buffer);
}

// Check if public and private keys match (have same ID)
bool keysMatch() {
	if (!fileExists("public_key.txt") || !fileExists("private_key.txt")) {
		return false;
	}

	// Read n value from public key (first line)
	ifstream pub("public_key.txt");
	string pub_n;
	getline(pub, pub_n);
	pub.close();

	// Read n value from private key (first line)  
	ifstream priv("private_key.txt");
	string priv_n;
	getline(priv, priv_n);
	priv.close();

	return (pub_n == priv_n);
}

// display system status with timestamps
void displayStatus() {
	cout << "File Status:\n";
	printSeparator();

	cout << "Public Key:    ";
	if (fileExists("public_key.txt")) {
		cout << "Created: " << getFileTimestamp("public_key.txt") << endl;
	}
	else {
		cout << "[X] Not found\n";
	}

	cout << "Private Key:   ";
	if (fileExists("private_key.txt")) {
		cout << "Created: " << getFileTimestamp("private_key.txt") << endl;
	}
	else {
		cout << "[X] Not found\n";
	}

	cout << "Encrypted AES: ";
	if (fileExists("encrypted_aes.txt")) {
		cout << "Created: " << getFileTimestamp("encrypted_aes.txt") << endl;
	}
	else {
		cout << "[X] Not found\n";
	}

	printSeparator();

	// Check if keys match
	if (fileExists("public_key.txt") && fileExists("private_key.txt")) {
		if (keysMatch()) {
			cout << "Key Status:    [OK] Keys are paired & ready to use\n";
		}
		else {
			cout << "Key Status:    [WARNING] Keys don't match!\n";
			cout << "               Generate new private key (Option 2)\n";
		}
	}
	else if (fileExists("public_key.txt") && !fileExists("private_key.txt")) {
		cout << "Key Status:    [INCOMPLETE] Generate private key (Option 2)\n";
	}
	else if (!fileExists("public_key.txt") && fileExists("private_key.txt")) {
		cout << "Key Status:    [INCOMPLETE] Generate public key (Option 1)\n";
	}
	else {
		cout << "Key Status:    [MISSING] Generate keys (Options 1 & 2)\n";
	}
}
