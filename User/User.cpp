#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <limits>
using namespace std;
class UserManager;
class User {
public:
	void save(ofstream& outFile) const {
        outFile << fullName << "," << username << "," << contactNumber << "," << email << "," << password << "," << isLocked << "\n";
    }
    void editProfile(const string& newFullName, const string& newContactNumber, const string& newEmail) {
        fullName = newFullName;
        contactNumber = newContactNumber;
        email = newEmail;
    }

    void changePassword(const string& newPassword) {
        password = newPassword;
    }
    User() : isLocked(false) {}
    User(const string& fullName, const string& username, const string& contactNumber, const string& email, const string& password, bool isLocked = false)
        : fullName(fullName), username(username), contactNumber(contactNumber), email(email), password(password), isLocked(isLocked) {}

    

    const string& getUsername() const { return username; }
    const string& getPassword() const { return password; }
    const string& getFullName() const { return fullName; }
    const string& getContactNumber() const { return contactNumber; }
    const string& getEmail() const { return email; }
    bool getIsLocked() const { return isLocked; }
    void setIsLocked(bool locked) { isLocked = locked; }

    

    static User load(const string& line) {
        size_t pos = 0;
        size_t prev = 0;
        string tokens[6];
        int i = 0;
        while ((pos = line.find(',', prev)) != string::npos && i < 6) {
            tokens[i++] = line.substr(prev, pos - prev);
            prev = pos + 1;
        }
        tokens[i] = line.substr(prev);

        return User(tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], tokens[5] == "1");
    }

private:
    string fullName;
    string username;
    string contactNumber;
    string email;
    string password;
    bool isLocked;
    
    
};

class Node {
public:
    User user;
    Node* next;

    Node(const User& user) : user(user), next(nullptr) {}
};

class UserManager {
public:
    UserManager() : head(nullptr) {
        loadUsers();
    }

    ~UserManager() {
        saveUsers();
        clearList();
    }

    void registerUser() {
        string fullName, username, contactNumber, email, password, confirmPassword;
        bool valid;
        while (true) {
            cout << "\nPlease enter your information\n";
            cout << "Full name: ";
            getline(cin, fullName);
            if (!isAlpha(fullName)) {
                cout << "Full name can only contain letters. Please try again.\n";
                continue;
            }
			do{
				fflush(stdin); 
				cout << "Username: ";
	            getline(cin, username);
	            if (findUserByUsername(username) != nullptr) {
	                cout << "Username already exists. Please try again.\n"<<endl;
	            }}while(findUserByUsername(username) != nullptr);
            
			do{fflush(stdin); 
				cout << "Contact number: ";
	            getline(cin, contactNumber);
	            valid=true;
	            if (!isValidPhoneNumber(contactNumber)) {
	                cout << "Invalid contact number. Please try again.\n"<<endl;
	                valid=false;
	            }else if(isPhoneNumberExists(contactNumber,username ))
				{cout << "Contact number already exists. Please try again.\n"<<endl;
				valid=false;}
			}while(!valid);
            

			do{
			fflush(stdin); 
			cout << "Email: ";
            getline(cin, email);
            valid=true;
            if (!isValidEmail(email)) {
                cout << "Invalid email format. Please try again.\n"<<endl;
                valid=false;
            }else if(isEmailExists(email,username ))
			{cout << "Email already exists. Please try again.\n"<<endl;
			valid=false;
			}	
			}while(!valid);
           

			
            cout << "Password: ";
            getline(cin, password);
            cout << "Confirm Password: ";
            getline(cin, confirmPassword);

            if (password != confirmPassword) {
                cout << "Passwords do not match. Please try again.\n";
                continue;
            }

            addUser(User(fullName, username, contactNumber, email, password));
            saveUsers();
            system("cls");
            cout << "Registration successful. Logging in...\n";
            loggedInUser = findUserByUsername(username);
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

        User* user = findUserByUsername(username);
        if (user) {
            if (user->getIsLocked()) {
                cout << "Your account is locked." << endl;
                return nullptr;
            }
            if (user->getPassword() == password) {
            	system("cls");
                cout << "Login successful.\n";
                return user;
            }
        }
        cout << "Invalid username or password.\n"<<endl;
        return nullptr;
    }

    void editUserProfile(User* user) {
    	bool valid=true;
    	string uname;
		uname=user->getUsername();
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
                if (!isAlpha(fullName)) {
                    cout << "Full name can only contain letters. Please try again.\n";
                    continue;
                }

                do{fflush(stdin); 
				cout << "New Contact Number: ";
	            getline(cin, contactNumber);
	            valid=true;
	            if (!isValidPhoneNumber(contactNumber)) {
	                cout << "Invalid contact number. Please try again.\n"<<endl;
	                valid=false;
	            }else if(isPhoneNumberExists(contactNumber,uname))
				{cout << "Contact number already exists. Please try again.\n"<<endl;
				valid=false;}
			}while(!valid);

			do{
			fflush(stdin); 
			cout << "New Email: ";
            getline(cin, email);
            valid=true;
            if (!isValidEmail(email)) {
                cout << "Invalid email format. Please try again.\n"<<endl;
                valid=false;
            }else if(isEmailExists(email,uname))
			{cout << "Email already exists. Please try again.\n"<<endl;
			valid=false;
			}	
			}while(!valid);
                

                cout << "\nNew profile details:\n";
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
                    saveUsers();
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
                saveUsers();
                break;
            }
        }
    }

    void lockUnlockUser(const string& username) {
        User* user = findUserByUsername(username);
        if (user) {
            user->setIsLocked(!user->getIsLocked());
            cout << (user->getIsLocked() ? "User has been locked." : "User has been unlocked.") << endl;
            saveUsers();
        } else {
            cout << "User not found!" << endl;
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
        cout << "\n========== Profile Menu ==========\n";
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
        }while (true);
    }

private:
    Node* head;
    User* loggedInUser = nullptr;
   string fileName = "users.txt";
	friend void saveUsers(UserManager& userManager);
    friend void editUserProfile(UserManager& userManager, User* user);
    friend void changeUserPassword(UserManager& userManager, User* user);
    void addUser(const User& user) {
        Node* newNode = new Node(user);
        if (!head) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    void loadUsers() {
        ifstream inFile(fileName.c_str());
        string line;
        while (getline(inFile, line)) {
            addUser(User::load(line));
        }
    }

    void saveUsers() {
        ofstream outFile(fileName.c_str());
        Node* current = head;
        while (current) {
            current->user.save(outFile);
            current = current->next;
        }
    }

    void clearList() {
        Node* current = head;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
    }

    User* findUserByUsername(const string& username) {
        Node* current = head;
        while (current) {
            if (current->user.getUsername() == username) {
                return &current->user;
            }
            current = current->next;
        }
        return nullptr;
    }

    bool isAlpha(const string& str) {
        for (char c : str) {
            if (!isalpha(c) && c != ' ') {
                return false;
            }
        }
        return true;
    }

    bool isValidPhoneNumber(const string& number) {
        return (number.length() == 10 || number.length() == 11) && number.substr(0, 2) == "01";
    }

    bool isValidEmail(const string& email) {
        return email.find('@') != string::npos;
    }
    bool isPhoneNumberExists(const string& phoneNumber,const string& uname) {
        Node* current = head;
        while (current) {
            if (current->user.getContactNumber() == phoneNumber&&current->user.getUsername()!=uname) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    bool isEmailExists(const string& email,const string& uname) {
        Node* current = head;
        while (current) {
            if (current->user.getEmail() == email&&current->user.getUsername()!=uname) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
};

int main() {
    UserManager userManager;
    userManager.run();
    return 0;
}
