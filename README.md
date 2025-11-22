# RSA_Encryption_System
RSA Program that encrypts AES-128 keys

# RSA Algorithm
 - We will be using RSA Algorithm to encrypt the AES key. This AES key is used for encrypting a text file containing confidential data.
 - AES uses the same key for encryption and decryption. So if this key falls into the wrong hands, the confidential data can easily be decrypted.
 - So we decided to encrypt this AES key with RSA an Assymetric encyrption method involving two keys (Public and Private).

> You might be wondering, why not just use RSA to encrypt the text file? The problem is that RSA cannot handle large amounts of data. Even if it could, it requires lots of computational power and time, making it ineffcient to encrypt large data.
>So why not use it to encrypt the vulnerable AES key? This would protect both the key and the data!


# RSA Crypto System - Group 7

A C++ implementation of RSA encryption/decryption for AES key management using GMP library.

## Setup Guide to run our RSA program

### Ensure your setup environment has the following:
- Windows 10/11
- Visual Studio 2022 (with C++ development tools)
- Git (for cloning the repository)


##  GMP Setup 
### Install VCPKG required package manager to install GMP library

### vcpkg in C:\ drive

### Step 1: Install vcpkg in C drive 
```cmd
# Open Command Prompt as Administrator
cd C:\
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
```
### Step 2: Install GMP Library 
#### Still in C:\vcpkg folder
```cmd
.\vcpkg install gmp:x64-windows
```

### Step 3: Navigate to where you want your project (Desktop, Documents, etc.)
`cd C:\Users\YourName\Desktop`

### Step 4: Clone the repository 
``` cmd
git clone https://github.com/lwin12/Encryption-Project.git
cd Encryption Project
```

### Step 5: Navigate to RSA-Encryption branch where our codes for RSA program is
```cmd 
git checkout RSA-Encryption
```


### Step 6: Create Empty Project and Add Files in Visual Studio 2022
1. **File → New → Project**
2. **Choose "Empty C++ Project"**
3. **Name:** `RSA-Crypto-System`
4. **Add files manually:**
   - Right-click "Source Files" → Add → Existing Item
   - Select all `.cpp` files
   - Right-click "Header Files" → Add → Existing Item
   - Select `rsa.h`

Verify Project Structure in Solution Explorer:
If you opened existing code, your structure should look like:

<img width="385" height="289" alt="image" src="https://github.com/user-attachments/assets/6b1e5456-780f-4c8f-8649-849d87d35340" />

##  Crypto++ Libraries setup
Please refer to the AES branch to setup the library.
Alternately you can refer to this youtube video on how to setup the Crypto++ library. 
[![Watch the video](https://img.youtube.com/vi/5XE4zEN-WKg/0.jpg)](https://www.youtube.com/watch?v=5XE4zEN-WKg)



## ENSURE BUILD PLATFORM IN VS2022 IS x64:

1. Look at the top toolbar
2. Find the dropdown that says "x86" or "Win32"
3. Change it to "x64"

#### If x64 is not available:
1. Build → Configuration Manager
2. Platform → dropdown → New → x64 → OK


**THIS BRANCH IS STILL UNDER CONSTRUCTION**

*Stay tuned for more updates on our RSA program!*
