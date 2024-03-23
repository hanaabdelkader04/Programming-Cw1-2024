#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// encryption function using Caesar

string encrypt(const string &password, int shift) {
    string encrypted_password = password;
    for (char &c : encrypted_password) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            c = ((c - base + shift) % 26) + base;
        }
    }
    return encrypted_password;
}

// decryption function

string decrypt(const string &encrypted_password, int shift) {
    return encrypt(encrypted_password, 26 - shift);
}

// appending encrypted credentials 

void save_user_to_file(const string& username, const string& encrypted_password) {
    ofstream userFile(username + ".txt");
    if (userFile.is_open()) {
        userFile << encrypted_password << "\n";
        userFile.close();
    } else {
        cerr << "Unable to open file for writing." << endl;
    }
}

// User Existence

bool does_user_exist(const string& username) {
    ifstream userFile(username + ".txt");
    return userFile.good();
}

// user registration logic

void register_user() {
    cout << "Enter a new username: ";
    string username;
    cin >> username;

    if (does_user_exist(username)) {
        cout << "Username already exists. Please choose a different username." << endl;
        return;
    }

    cout << "Enter a password: ";
    string password;
    cin >> password;

    string encrypted_password = encrypt(password, 10);
    save_user_to_file(username, encrypted_password);
    cout << "Registration completed! Welcome, " << username << "!" << endl;
}

// login user logic

bool login_user(string& username) {
    cout << "Enter your username: ";
    cin >> username;

    if (!does_user_exist(username)) {
        cerr << "Username not found!" << endl;
        return false;
    }

    cout << "Enter your password: ";
    string password;
    cin >> password;

    string filename = username + ".txt";
    ifstream userFile(filename);
    string stored_encrypted_password;
    getline(userFile, stored_encrypted_password);
    userFile.close();

    string input_encrypted_password = encrypt(password, 10);
    if (input_encrypted_password == stored_encrypted_password) {
        cout << "Welcome, " << username << "!" << endl;
        return true;
    } else {
        cerr << "Invalid password!" << endl;
        return false;
    }
}

// random password generation function

string generate_random_password(int length) {
    string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string password = "";
    for (int i = 0; i < length; ++i) {
        password += charset[rand() % charset.length()];
    }
    return password;
}

// function to add a password either manually or generated

void add_website_password(const string& username) {
    cout << "Enter the website name: ";
    string website;
    cin >> website;

    cout << "Do you want to enter the password manually (m) or generate it randomly (r)? ";
    char choice;
    cin >> choice;

    string password;
    if (choice == 'm') {
        cout << "Enter the password for the website: ";
        cin >> password;
    } else if (choice == 'r') {
        cout << "Enter the length of the password to generate: ";
        int length;
        cin >> length;
        password = generate_random_password(length);
        cout << "Generated Password: " << password << endl;
    } else {
        cerr << "Invalid choice!" << endl;
        return;
    }

    int shift = 10;
    string encrypted_password = encrypt(password, shift);

    string filename = username + ".txt";
    ofstream userFile(filename, ios::app); 
    if (userFile.is_open()) {
        userFile << website << " " << encrypted_password << "\n";
        userFile.close();
        cout << "Website and password added successfully." << endl;
    } else {
        cerr << "Unable to open file to add website and password." << endl;
    }
}

// all passwords retrieval function

void retrieve_all_passwords(const string& username) {
    cout << "Retrieving all passwords for " << username << ":\n";

    int shift = 10;
    string filename = username + ".txt";
    ifstream userFile(filename);
    string line, website, encrypted_password;

    // Skip the first line containing user's encrypted password
    getline(userFile, encrypted_password); 

    // Display headers
    cout << "+----------------------------------+\n";
    cout << "|         Website       |  Password |\n";
    cout << "+----------------------------------+\n";

    while (getline(userFile, line)) {
        size_t sep = line.find(' ');
        if (sep != string::npos) {
            website = line.substr(0, sep);
            encrypted_password = line.substr(sep + 1);

            // Decrypt password
            string password = decrypt(encrypted_password, shift);

            // Display website and password in table format
            cout << "| " << setw(20) << left << website << " | " << setw(9) << left << password << " |\n";
        }
    }

    cout << "+----------------------------------+\n";

    userFile.close();
}

// specific website password retrieval

void retrieve_password_for_website(const string& username, const string& website) {
    cout << "Retrieving password for " << username << " and " << website << ":\n";
    int shift = 10;
    string filename = username + ".txt";

    ifstream userFile(filename);
    if (!userFile.is_open()) {
        cerr << "Error: Could not open user file for reading." << endl;
        return;
    }

    string stored_encrypted_password, stored_website;
    getline(userFile, stored_encrypted_password); 

    bool found = false;
    while (getline(userFile, stored_website, ' ') && getline(userFile, stored_encrypted_password)) {
        if (website != stored_website) {
            continue;
        }
        found = true;
        cout << "Website: " << stored_website << ", Password: " << decrypt(stored_encrypted_password, shift) << endl;
    }

    if (!found) {
        cout << "No matching website found in the user's file." << endl;
    }

    userFile.close();
}

// main function

int main() {
    srand(time(nullptr));

    string username;
    while (true) {
        cout << "Would you like to login (l) or register (r)? ";
        string answer;
        cin >> answer;

        if (answer == "l") {
            if (login_user(username)) {
                while (true) {
                    cout << "Choose an option:\n";
                    cout << "1. Add a new website and password\n";
                    cout << "2. Retrieve all my passwords\n";
                    cout << "3. Retrieve a password for a specific website\n";
                    cout << "4. Logout\n";
                    int option;
                    cin >> option;

                    if (option == 1) {
                        add_website_password(username);
                    } else if (option == 2) {
                        retrieve_all_passwords(username);
                    } else if (option == 3) {
                        string website;
                        cout << "Enter the website name: ";
                        cin >> website;
                        retrieve_password_for_website(username, website);
                    } else if (option == 4) {
                        cout << "Logging out...\n";
                        break;
                    } else {
                        cout << "Invalid option. Please choose a valid option.\n";
                    }
                }
            }
        } else if (answer == "r") {
            register_user();
        } else {
            cout << "Invalid command! Please choose 'l' for login or 'r' for registration.\n";
        }
    }

    return 0;
}
