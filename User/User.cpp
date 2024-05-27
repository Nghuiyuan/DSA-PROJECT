#include <iostream>
#include <string>
#include <map>
#include <fstream>
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
        cout << "\nPlease enter your information\n";
        cout << "Full name: ";
        getline(cin, fullName);
        cout << "Username: ";
        getline(cin, username);
        cout << "Contact number: ";
        getline(cin, contactNumber);
        cout << "Email: ";
        getline(cin, email);
        cout << "Password: ";
        getline(cin, password);
        cout << "Confirm Password: ";
        getline(cin, confirmPassword);

        if (password != confirmPassword) {
            cout << "Passwords do not match. Please register again.\n";
            return;
        }

        if (users.find(username) != users.end()) {
            cout << "Username already exists. Please register again.\n";
            return;
        }
        users[username] = User(fullName, username, contactNumber, email, password);
        cout << "Registration successful. Please log in now.\n";
    }

    User* loginUser() {
        string username, password;
        cout << "\nEnter your information to login\n";
        cout << "Username: ";
        getline(cin, username);
        cout << "Password: ";
        getline(cin, password);

        map<string, User>::iterator it = users.find(username);
        if (it != users.end() && it->second.getPassword() == password) {
            cout << "Login successful.\n";
            return &it->second;
        }
        cout << "Invalid username or password.\n";
        return NULL;
    }

    void editUserProfile(User* user) {
        if (user) {
            string fullName, contactNumber, email;
            cout << "\nNew full name: ";
            getline(cin, fullName);
            cout << "New contact number: ";
            getline(cin, contactNumber);
            cout << "New email: ";
            getline(cin, email);
            user->editProfile(fullName, contactNumber, email);
            cout << "Profile updated successfully.\n";
        }
    }

    void changeUserPassword(User* user) {
        if (user) {
            string currentPassword, newPassword, confirmPassword;
            cout << "Current password: ";
            getline(cin, currentPassword);

            if (currentPassword != user->getPassword()) {
                cout << "Current password is incorrect.\n";
                return;
            }

            cout << "New password: ";
            getline(cin, newPassword);
            cout << "Confirm new password: ";
            getline(cin, confirmPassword);

            if (newPassword != confirmPassword) {
                cout << "New passwords do not match.\n";
                return;
            }

            user->changePassword(newPassword);
            cout << "Password changed successfully.\n";
        }
    }

    void displayMainMenu() {
        cout << "===================================\n";
        cout << "---Welcome to Reservation System---\n";
        cout << "===================================\n";
        cout << "1. Register\n2. Login\n3. Edit Profile\n4. Change Password\n5. Exit\nEnter your choice: ";
    }

    void run() {
        User* loggedInUser = NULL;
        int choice;

        do {
            displayMainMenu();
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1:
                    registerUser();
                    break;
                case 2:
                    loggedInUser = loginUser();
                    break;
                case 3:
                case 4:
                    if (!loggedInUser) {
                        cout << "Please login first.\n";
                        loggedInUser = loginUser();
                        if (!loggedInUser) {
                            break;
                        }
                    }
                    if (choice == 3) {
                        editUserProfile(loggedInUser);
                    } else if (choice == 4) {
                        changeUserPassword(loggedInUser);
                    }
                    break;
                case 5:
                    cout << "Exiting...\n";
                    break;
                default:
                    cout << "Invalid choice.\n";
                    break;
            }
        } while (choice != 5);
    }

private:
    map<string, User> users;
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
        for (map<string, User>::const_iterator it = users.begin(); it != users.end(); ++it) {
            it->second.save(outFile);
        }
    }
};

const string UserManager::fileName = "user.txt";

int main() {
    UserManager userManager;
    userManager.run();
    return 0;
}
