# Encryption Project
Aim of this project is to be able to encrypt a simple text file and send it to another computer on the same network.  
Due to time constraints, we will be using a thumb drive to share the files.  

*Let's assume Bob wants to send Jane a text file.*  

1. *Jane must generate a public and private key.*  
   *Jane will then keep the private key to herself and tell Bob her public key.*  

2. *Bob receives Jane's public key.*  
   *Bob proceeds to encrypt text file using Jane's Public key.*  
   *Bob sends the encrypted text file to Jane.*  
   
3. *Jane receives Bob's encryped text file.*  
   *Jane decrypts the encryped text file using her private key.*

# Generating Public & Private Keys
1. Pick two large prime numbers *p, q*.  
   *p* and *q* must be random, large and secret.
     
2. Compute their product  
   *n = p · q*  
   n is now part of both public and private key.

3. Compute Euler's totient  
   *φ(n) = (p−1)(q−1)*
   This is used to determine valid exponents.

4. Choose the public exponent *e*  
   Integer *e* must fullfil this condition: 1 < *e* < φ(n)  

5. Compute the private exponent *d*  
   *d* is the modular multiplicative inverse of *e (modφ(n))*:  
   *d · e ≡ 1 ( modφ ( n ) )*  
   This can be calculated with the Extended Euclidean Algorithm. (Topic 2 of math 1)

6. Public Key: *(e, n)* --> Used for encryption.
7. Private Key: *(d, n)* --> Used for decryption.

# Tasks
- Code to generate 2 Random Prime Numbers, *string gen_2_Prime();*
- Code to generate Public & Private keys, *void gen_Pri_Pub(int n);*  
  For now use these for the 2nd Tasks:  
  *p = 1299827*  
  *q = 1300033*  
  *n = p · q = 1299827 · 1300033 = 1689744480831*
