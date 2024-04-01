#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

struct Node {
    // website name
    string key;
    // encrypted password
    string value; 
    Node* next;

    Node(string k, string v) : key(k), value(v), next(nullptr) {}
};

class HashTable {
public:
    Node** table;
    int capacity;

    HashTable(int size) : capacity(size) {
        table = new Node*[capacity];
        for (int i = 0; i < capacity; i++) {
            table[i] = nullptr;
        }
    }

    ~HashTable() {
        for (int i = 0; i < capacity; i++) {
            Node* entry = table[i];
            while (entry != nullptr) {
                Node* toDelete = entry;
                entry = entry->next;
                delete toDelete;
            }
        }
        delete[] table;
    }

    int hash(const string& key) {
        unsigned long hash = 5381;
        for (char c : key) {
            hash = ((hash << 5) + hash) + c;
        }
        return hash % capacity;
    }

    void insert(const string& key, const string& value) {
        int hashValue = hash(key);
        Node* prev = nullptr;
        Node* entry = table[hashValue];

        while (entry != nullptr && entry->key != key) {
            prev = entry;
            entry = entry->next;
        }

        if (entry == nullptr) {
            entry = new Node(key, value);
            if (prev == nullptr) {
                table[hashValue] = entry;
            } else {
                prev->next = entry;
            }
        } else {
            entry->value = value;
        }
    }

    string get(const string& key) {
        int hashValue = hash(key);
        Node* entry = table[hashValue];

        while (entry != nullptr) {
            if (entry->key == key) {
                return entry->value;
            }
            entry = entry->next;
        }
        return "";
    }

    bool contains(const string& key) {
        int hashValue = hash(key);
        Node* entry = table[hashValue];

        while (entry != nullptr) {
            if (entry->key == key) {
                return true;
            }
            entry = entry->next;
        }
        return false;
    }

    void remove(const string& key) {
    int index = hash(key);
    Node* current = table[index];
    Node* prev = nullptr;

    while (current != nullptr) {
        if (current->key == key) {
            if (prev == nullptr) {
                table[index] = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            return;
        }
        prev = current;
        current = current->next;
    }
}

};

HashTable userPasswords(1000);

// encryption function using Caesar Cipher
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

// function to safely read an integer input
int readInteger(const string& prompt) {
    int input;
    while (true) {
        cout << prompt;
        cin >> input;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number." << endl;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return input;
        }
    }
}

// user existance
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

// user login logic
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


// file operations
bool fileContainsWebsite(const string& filename, const string& website) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        if (line.find(website + " ") == 0) { // Check if the line starts with the website name
            return true;
        }
    }
    return false;
}

void appendLineToFile(const string& filename, const string& line) {
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << line << endl;
    } else {
        cerr << "Unable to open file " << filename << endl;
    }
}


void add_website_password(const string& username) {
    cout << "Enter the website name: ";
    string website;
    cin >> website;

    string compositeKey = username + "|" + website;
    string filename = username + ".txt";

    if (userPasswords.contains(compositeKey)) {
        cout << "Error: You already have a password stored for " << website << ".\n";
        return;
    }

    if (fileContainsWebsite(filename, website)) {
        cout << "Error: You already have a password stored for " << website << " (verified by file check).\n";
        return;
    }

    cout << "Do you want to enter the password manually (m) or generate it randomly (r)? ";
    char choice;
    cin >> choice;

    string password;
    if (choice == 'm') {
        cout << "Enter the password for the website: ";
        cin >> password;
    } else if (choice == 'r') {
        int length = readInteger("Enter the length of the password to generate: ");
        password = generate_random_password(length);
        cout << "Generated Password: " << password << endl;
    } else {
        cerr << "Invalid choice!" << endl;
        return;
    }

    int shift = 10;
    string encrypted_password = encrypt(password, shift);

    appendLineToFile(filename, website + " " + encrypted_password);
    userPasswords.insert(compositeKey, encrypted_password);

    cout << "Website and password added successfully." << endl;
}


// all passwords retrieval function
void retrieve_all_passwords(const string& username) {
    cout << "Retrieving all passwords for " << username << ":\n";

    ifstream userFile(username + ".txt");
    if (!userFile.is_open()) {
        cerr << "Unable to open file for " << username << ".\n";
        return;
    }

    string line;
    bool skipFirstLine = true;

    cout << left << setw(20) << "Website" << setw(20) << "Password" << "\n";
    cout << string(40, '-') << "\n";

    while (getline(userFile, line)) {
        if (skipFirstLine) {
            skipFirstLine = false;
            continue;
        }

        size_t sepPos = line.find(' ');
        if (sepPos != string::npos) {
            string website = line.substr(0, sepPos);
            string encryptedPassword = line.substr(sepPos + 1);

            string password = decrypt(encryptedPassword, 10);

            cout << left << setw(20) << website << setw(20) << password << "\n";
        }
    }

    userFile.close();
}

// specific website password retrieval
void retrieve_password_for_website(const string& username, const string& website) {
    string compositeKey = username + "|" + website;
    string encrypted_password = userPasswords.get(compositeKey);

    
    if (!encrypted_password.empty()) {
        cout << "Website: " << website << ", Password: " << decrypt(encrypted_password, 10) << endl;
    } else {
        
        ifstream userFile(username + ".txt");
        if (!userFile.is_open()) {
            cerr << "Error: Could not open user file for reading." << endl;
            return;
        }

        bool found = false;
        string line;
        while (getline(userFile, line)) {
            size_t sep = line.find(' ');
            if (sep != string::npos) {
                string fileWebsite = line.substr(0, sep);
                string fileEncryptedPassword = line.substr(sep + 1);
                if (fileWebsite == website) {
                    cout << "Website: " << website << ", Password: " << decrypt(fileEncryptedPassword, 10) << endl;
                    found = true;
                    break;
                }
            }
        }

        if (!found) {
            cout << "No matching website found for " << username << "." << endl;
        }
        userFile.close();
    }
}

// delete a website function
void delete_website_password(const string& username) {
    cout << "Enter the website name you wish to delete: ";
    string website;
    cin >> website;

    
    string compositeKey = username + "|" + website;

    
    userPasswords.remove(compositeKey);

    // deleting from the file
    string filename = username + ".txt";
    ifstream userFile(filename);
    ofstream tempFile("temp_" + filename);
    bool found = false;
    string line;

    while (getline(userFile, line)) {
        if (line.find(website + " ") == string::npos) {
            tempFile << line << endl;
        } else {
            found = true; 
        }
    }

    userFile.close();
    tempFile.close();

    if (found) {
        // delete website and password
        remove(filename.c_str());
        rename(("temp_" + filename).c_str(), filename.c_str());
        cout << "Website and password deleted successfully." << endl;
    } else {
        remove(("temp_" + filename).c_str());
        cout << "Website not found. No changes made." << endl;
    }
}


// Main function
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
                    cout << "Choose an option:\n"
                         << "1. Add a new website and password\n"
                         << "2. Retrieve all my passwords\n"
                         << "3. Retrieve a password for a specific website\n"
                         << "4. Delete a website and password\n"
                         << "5. Logout\n" 
                         << "Enter option number: ";
                    int option = readInteger(""); 

                    switch (option) {
                        case 1:
                            add_website_password(username);
                            break;
                        case 2:
                            retrieve_all_passwords(username);
                            break;
                        case 3: {
                            string website;
                            cout << "Enter the website name: ";
                            cin >> website;
                            retrieve_password_for_website(username, website);
                            break;
                        }
                        case 4:
                            delete_website_password(username);
                            break;
                        case 5:
                            cout << "Logging out...\n";
                            return 0;
                        default:
                            cout << "Invalid option. Please choose a valid option.\n";
                            break;
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

