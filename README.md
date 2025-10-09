# Encryption Project (AES Encryption branch)
Current goal is to be able to encrypt a text file and generate an text file that is ciphered. 

10/10/2025: Successfully encryped a string of letters and produced a ciphered text and key.

# Setting up
Step 1: Clone the crypto++ Library to your desktop  
<img width="380" height="90" alt="image" src="https://github.com/user-attachments/assets/761093e9-8b42-4795-ad56-a8bfc5cf5b20" />  
Step 2: Clone the cryptopp-pem Library to your desktop  
<img width="403" height="86" alt="image" src="https://github.com/user-attachments/assets/54f1536d-66ff-4ff4-bfe2-f794f2762f65" />  

After those 2 steps you should have 2 folders on your desktop  
<img width="150" height="88" alt="image" src="https://github.com/user-attachments/assets/42bde250-c6ce-49e2-bb9a-c9a8f888c230" />  

Step 3: Transfer everything from cryptopp-pem folder to cryptopp  

During the transfer there will be a pop up.  
<img width="669" height="430" alt="image" src="https://github.com/user-attachments/assets/719b6dc7-81e6-4e64-83e6-0f1a251abb3b" />  
Click on "Skip this file".

Step 4: Look for file "cryptlib.vcxproj" in the cryptopp folder and double click on it.  
<img width="974" height="378" alt="image" src="https://github.com/user-attachments/assets/b4df35c7-eb13-48dd-aef5-75b4a0b233f6" />

Your visual studio should be able to see this in your solution explorer.  
<img width="417" height="241" alt="image" src="https://github.com/user-attachments/assets/3162d8bc-79c9-4a58-940c-8c2ce7b3c7c5" />  

Step 5: Inside the solution explorer, left click on the arrow besides cryptlib then right click on header and then mouse over Add and click on "Existing items..."  
<img width="758" height="353" alt="image" src="https://github.com/user-attachments/assets/3c8f4e25-5910-43a1-953d-81fb36d304e7" />  

Step 6: Copy paste < "pem_common.h" "pem.h" > in "File name"  
<img width="610" height="364" alt="image" src="https://github.com/user-attachments/assets/1af45020-7c1c-4c28-8dbc-39a3e4b74b83" />  

Step 7: Refer to step 5, but instead of header, it will now be "Source File" then copy paste this   
< "pem_read.cpp" "pem_write.cpp" "pem_common.cpp" >  
<img width="865" height="509" alt="image" src="https://github.com/user-attachments/assets/5cf694b1-eeee-40fd-b4ef-074c0f663fc7" />  

Step 8: Go back to your Visual Studio and change to the screenshot below  
<img width="382" height="28" alt="image" src="https://github.com/user-attachments/assets/c0b03bc9-7229-4d23-b4e9-2bedf407fda3" />

Step 9: Click right click on cryptlib  in solution explorer and click Build   
<img width="474" height="105" alt="image" src="https://github.com/user-attachments/assets/01694d26-8127-4b7f-b622-a6652fc91d5a" />

After awhile, you should be able to see something like below.  
<img width="871" height="170" alt="image" src="https://github.com/user-attachments/assets/50bffcb9-09cb-44eb-a6c2-ff9fff0bd902" />  
Just make sure you see "Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped"

Step 10: Create a new project in VS and copy the contents from main.cpp into your .cpp file  
<img width="1920" height="1032" alt="image" src="https://github.com/user-attachments/assets/1e2fc951-3d8f-458c-ba39-95206a3bd0fa" />  
You will notice that there are alot of errors which is normal.  

Step 11: Right click on your solution then click on properties  
<img width="512" height="854" alt="image" src="https://github.com/user-attachments/assets/345e0491-d0bf-4d27-90c9-586454148c14" />  








Credits to crypto++ library and RazviOverflow
