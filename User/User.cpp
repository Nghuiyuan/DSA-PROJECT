#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <cctype>
#include <limits>
using namespace std;

class User {
public:
    User() {}
    User(const string& fullName, const string& username, const string& contactNumber, const string& email, const string& password)
        : fullName(fullName), username(username), contactNumber(contactNumber), email(email), password(password) {}

    void editProfile(const string& newFullName, const string& newContactNumber, const string& newEmail) {
        fullName = newFullName;
        contactNumber = newContactNumber;
        email = newEmail;
    }

    void changePassword(const string& newPassword) {
        password = newPassword;
    }

    const string& getUsername() const { return username; }
    const string& getPassword() const { return password; }
    const string& getFullName() const { return fullName; }
    const string& getContactNumber() const { return contactNumber; }
    const string& getEmail() const { return email; }

    void save(ofstream& outFile) const {
        outFile << fullName << "," << username << "," << contactNumber << "," << email << "," << password << "\n";
    }

    static User load(const string& line) {
        size_t pos = 0;
        size_t prev = 0;
        string tokens[5];
        int i = 0;
        while ((pos = line.find(',', prev)) != string::npos && i < 5) {
            tokens[i++] = line.substr(prev, pos - prev);
            prev = pos + 1;
        }
        tokens[i] = line.substr(prev);

        return User(tokens[0], tokens[1], tokens[2], tokens[3], tokens[4]);
    }

private:
    string fullName;
    string username;
    string contactNumber;
    string email;
    string password;
};

class UserManager {
public:
    UserManager() {
        loadUsers();
    }

    ~UserManager() {
        saveUsers();
    }

    void registerUser() {
        string fullName, username, contactNumber, email, password, confirmPassword;
        while (true) {
            cout << "\nPlease enter your information\n";
            cout << "Full name: ";
            getline(cin, fullName);
            cout << "Username: ";
            getline(cin, username);

            cout << "Contact number: ";
            getline(cin, contactNumber);
            if (!isValidPhoneNumber(contactNumber)) {
                cout << "Invalid contact number. Please try again.\n";
                continue;
            }

            cout << "Email: ";
            getline(cin, email);
            if (!isValidEmail(email)) {
                cout << "Invalid email format. Please try again.\n";
                continue;
            }

            cout << "Password: ";
            getline(cin, password);
            cout << "Confirm Password: ";
            getline(cin, confirmPassword);

            if (password != confirmPassword) {
                cout << "Passwords do not match. Please try again.\n";
                continue;
            }

            if (users.find(username) != users.end()) {
                cout << "Username already exists. Please try again.\n";
                continue;
            }

            users[username] = User(fullName, username, contactNumber, email, password);
            cout << "Registration successful. Logging in...\n";
            loggedInUser = &users[username];
            break;
        }
    }

    User* loginUser() {
        string username, password;
        cout << "\nEnter your information to login\n";
        cout << "Username: ";
        getline(cin, username);
        cout << "Password: ";
        getline(cin, password);

        auto it = users.find(username);
        if (it != users.end() && it->second.getPassword() == password) {
            cout << "Login successful.\n";
            return &it->second;
        }
        cout << "Invalid username or password.\n";
        return nullptr;
    }

    void editUserProfile(User* user) {
        if (user) {
            string fullName, contactNumber, email;
            while (true) {
                cout << "\nCurrent profile details:\n";
                cout << "Full name: " << user->getFullName() << "\n";
                cout << "Contact number: " << user->getContactNumber() << "\n";
                cout << "Email: " << user->getEmail() << "\n";

                cout << "\nEnter new details:\n";
                cout << "New full name: ";
                getline(cin, fullName);
                cout << "New contact number: ";
                getline(cin, contactNumber);
                if (!isValidPhoneNumber(contactNumber)) {
                    cout << "Invalid contact number. Please try again.\n";
                    continue;
                }
                cout << "New email: ";
                getline(cin, email);
                if (!isValidEmail(email)) {
                    cout << "Invalid email format. Please try again.\n";
                    continue;
                }

                cout << "\nUpdated profile details:\n";
                cout << "Full name: " << fullName << "\n";
                cout << "Contact number: " << contactNumber << "\n";
                cout << "Email: " << email << "\n";

                char confirm;
                cout << "Do you want to save the changes? (y/n): ";
                cin >> confirm;
                cin.ignore();

                if (confirm == 'y' || confirm == 'Y') {
                    user->editProfile(fullName, contactNumber, email);
                    cout << "Profile updated successfully.\n";
                    break;
                } else {
                    cout << "Profile update canceled.\n";
                    break;
                }
            }
        }
    }

    void changeUserPassword(User* user) {
        if (user) {
            string currentPassword, newPassword, confirmPassword;
            while (true) {
                cout << "Current password: ";
                getline(cin, currentPassword);

                if (currentPassword != user->getPassword()) {
                    cout << "Current password is incorrect. Please try again.\n";
                    continue;
                }

                cout << "New password: ";
                getline(cin, newPassword);
                cout << "Confirm new password: ";
                getline(cin, confirmPassword);

                if (newPassword != confirmPassword) {
                    cout << "New passwords do not match. Please try again.\n";
                    continue;
                }

                user->changePassword(newPassword);
                cout << "Password changed successfully.\n";
                break;
            }
        }
    }

    void displayMainMenu() {
        cout << "===================================\n";
        cout << "---Welcome to Reservation System---\n";
        cout << "===================================\n";
        if (loggedInUser) {
            cout << "1. Profile\n2. Logout\n";
        } else {
            cout << "1. Register\n2. Login\n3. Exit\n";
        }
        cout << "Enter your choice: ";
    }

    void displayProfileMenu() {
        cout << "========== Profile Menu ==========\n";
        cout << "1. Edit Profile\n2. Change Password\n3. Back to Main Menu\n";
        cout << "Enter your choice: ";
    }

    void run() {
        int choice;
        do {
            displayMainMenu();
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number.\n";
                continue;
            }
            cin.ignore();

            if (loggedInUser) {
                if (choice == 1) {
                    int profileChoice;
                    do {
                        displayProfileMenu();
                        if (!(cin >> profileChoice)) {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Invalid input. Please enter a number.\n";
                            continue;
                        }
                        cin.ignore();
                        switch (profileChoice) {
                            case 1:
                                editUserProfile(loggedInUser);
                                break;
                            case 2:
                                changeUserPassword(loggedInUser);
                                break;
                            case 3:
                                break;
                            default:
                                cout << "Invalid choice.\n";
                                break;
                        }
                    } while (profileChoice != 3);
                } else if (choice == 2) {
                    loggedInUser = nullptr;
                    cout << "Logged out successfully.\n";
                } else {
                    cout << "Invalid choice.\n";
                }
            } else {
                switch (choice) {
                    case 1:
                        registerUser();
                        break;
                    case 2:
                        loggedInUser = loginUser();
                        break;
                    case 3:
                        cout << "Exiting...\n";
                        return;
                    default:
                        cout << "Invalid choice.\n";
                        break;
                }
            }
        } while (true);
    }

private:
    unordered_map<string, User> users;
    User* loggedInUser = nullptr;
    static const string fileName;

    void loadUsers() {
        ifstream inFile(fileName.c_str());
        string line;
        while (getline(inFile, line)) {
            User user = User::load(line);
            users[user.getUsername()] = user;
        }
    }

    void saveUsers() {
        ofstream outFile(fileName.c_str());
        for (const auto& pair : users) {
            pair.second.save(outFile);
        }
    }

    bool isValidPhoneNumber(const string& number) {
        for (char c : number) {
            if (!isdigit(c)) {
                return false;
            }
        }
        return true;
    }

    bool isValidEmail(const string& email) {
        return email.find('@') != string::npos;
    }
};

const string UserManager::fileName = "users.txt";

int main() {
    UserManager userManager;
    userManager.run();
    return 0;
}