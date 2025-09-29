# Encryption-Project
Aim of this project is to be able to encrypt a simple text file and send it to another computer on the same network.

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
