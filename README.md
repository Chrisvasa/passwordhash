# Password Recovery
This project is a GUI-based application designed for managing user accounts, cracking passwords, and managing files. It utilizes ImGui for the graphical interface and offers various functionalities, including user authentication, password hashing, and file manipulation.

## Features
- User authentication and account creation with additional security options.
- Password cracking utility that attempts to match given hashes with passwords.
- File management capabilities, including file sorting and hash generation.
- GUI built with ImGui for an intuitive user interface.

## Screenshot
![image](https://github.com/Chrisvasa/passwordhash/assets/29359169/5b7c3b5f-430f-449c-8976-f0e38c8954da)

## Usage
Examples
- Logging In: Enter your username and password in the 'Account Manager' window and click 'Login'.
- Cracking Passwords: Navigate to the 'Password Cracker' window, enter the hash you wish to crack, and click 'Crack Hash'.
- Managing Files: In the 'File Manager' window, you can select files for various operations like sorting by hash or generating hashes for passwords.

## Prerequisites
Before you begin, ensure you have met the following requirements:
- C++ compiler (GCC or Clang)
- Make (for building the project)
- OpenSSL (for cryptographic functions)
- Linux (Change main to DX11 example from ImGui repo)
