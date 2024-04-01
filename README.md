
# Password Manager README

## Overview
This Password Manager is a simple yet effective command-line application developed in C++ for managing your passwords securely. It allows users to register, login, store, and retrieve passwords for different websites. The application encrypts your passwords using a Caesar Cipher encryption method, providing basic security for your sensitive information.

## Features
- User Registration and Login: Securely register as a new user with a username and password, or login if you're an existing user.
- Password Encryption: Utilizes Caesar Cipher for basic encryption and decryption of passwords.
- Add Website and Password: Allows users to store passwords for different websites. Users can either enter a password manually or generate a random one.
- Retrieve Passwords: Users can retrieve all their stored passwords or fetch a password for a specific website.
- Secure Password Storage: Passwords are stored in a file, encrypted for added security.
Getting Started
-Password Deletion: Users are able to delete any passwords

## Prerequisites
- A C++ compiler (GCC recommended)
- Basic knowledge of using terminal or command prompt

## Installation

#### Clone the repository or download the source code.
git clone https://github.com/hanaabdelkader04/Programming-Cw1-2024/tree/main/prog2

#### Navigate to the project directory.
cd password-manager

#### Compile the program using your C++ compiler. For example, with GCC:
g++ -o passwordManager main.cpp

#### Run the compiled application.
./passwordManager


## Usage Instructions

- Register/Login: Upon starting the application, you will be prompted to either login (l) or register (r). New users should register first.
- Adding a Website Password: After logging in, select the option to add a new website and password. You can choose to input the password manually or generate one automatically.
- Retrieving Passwords: To view all your stored passwords, select the option to retrieve all passwords. To view a password for a specific site, choose the respective option and provide the website's name.
- Password Deletion: Users can delete any password they wsnt by just enter the website's name it belongs to.
- Logout: You can logout at any time by selecting the logout option.
