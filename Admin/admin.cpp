#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
#include <algorithm>

using namespace std;

const int MAX_ADMINS = 100; // Maximum number of admins
const int MAX_USERS = 100;  // Maximum number of users

class User {
public:
    string fullname;
    string username;
    string contactNumber;
    string email;
    string password;
    bool isLocked;

    User() {}

    User(string fname, string uname, string contact, string e, string passw, bool locked = false)
        : fullname(fname), username(uname), contactNumber(contact), email(e), password(passw), isLocked(locked) {}

    void displayUser() const {
        cout << "\nFullname: " << fullname
             << "\nUsername: " << username
             << "\nContact Number: " << contactNumber
             << "\nEmail: " << email
             << "\nPassword: " << password
             << "\nLocked: " << (isLocked ? "Yes" : "No") << endl;
    }
};

class Admin {
public:
    string adminname;
    string password;
    bool isLocked;
    bool isSuperAdmin;
    string sex;
    int age;
    string contactNumber;

    Admin() {}

    Admin(string admin, string pw, bool locked, bool superAdmin, string s, int a, string contact)
        : adminname(admin), password(pw), isLocked(locked), isSuperAdmin(superAdmin), sex(s), age(a), contactNumber(contact) {}

    void displayAdmin() const {
        cout << "Admin's username: " << adminname
             << "\nLocked: " << (isLocked ? "Yes" : "No")
             << "\nRole: " << (isSuperAdmin ? "Super Admin" : "Admin")
             << "\nSex: " << sex
             << "\nAge: " << age
             << "\nContact Number: " << contactNumber << endl;
    }

    void displayProfile() const {
        cout << "Admin's username: " << adminname
             << ", Sex: " << sex
             << ", Age: " << age
             << ", Contact Number: " << contactNumber << endl;
    }
};

class AdminManager {
private:
    Admin admins[MAX_ADMINS];
    User users[MAX_USERS];
    string adminFile;
    string userFile;
    Admin* loggedInAdmin;
    bool isSuperAdmin;
    int adminCount;
    int userCount;

public:
    AdminManager(const string &afile, const string &ufile) : adminFile(afile), userFile(ufile), loggedInAdmin(NULL), isSuperAdmin(false), adminCount(0), userCount(0) {
        readAdminsFromFile(adminFile);
        readUsersFromFile(userFile);
        if (adminCount == 0) {
            registerSuperAdmin();
        }
    }

    void registerSuperAdmin() {
        string adminname, password;
        cout << "\nNo admin accounts found. Please register a Super Admin account." << endl;
        cout << "Enter Super Admin username: ";
        getline(cin, adminname);
        cout << "Enter Super Admin password: ";
        getline(cin, password);

        addAdmin(adminname, password, true);

        cout << "Super Admin account created successfully!" << endl;
    }

    void readAdminsFromFile(const string &afile) {
        ifstream file(afile.c_str());

        if (file.is_open()) {
            string line;

            while (getline(file, line) && adminCount < MAX_ADMINS) {
                stringstream ss(line);
                string admin, pw, lockedStr, superAdminStr, sex, contact;
                int age;

                getline(ss, admin, ',');
                getline(ss, pw, ',');
                getline(ss, lockedStr, ',');
                getline(ss, superAdminStr, ',');
                getline(ss, sex, ',');
                ss >> age;
                ss.ignore();
                getline(ss, contact, ',');
                bool locked = (lockedStr == "1");
                bool superAdmin = (superAdminStr == "1");
                admins[adminCount++] = Admin(admin, pw, locked, superAdmin, sex, age, contact);
            }

            file.close();
        }
    }

    void readUsersFromFile(const string &ufile) {
        ifstream file(ufile.c_str());

        if (file.is_open()) {
            string line;

            while (getline(file, line) && userCount < MAX_USERS) {
                stringstream ss(line);
                string fname, uname, contact, e, passw, lockedStr;
                bool locked;

                getline(ss, fname, ',');
                getline(ss, uname, ',');
                getline(ss, contact, ',');
                getline(ss, e, ',');
                getline(ss, passw, ',');
                getline(ss, lockedStr, ',');
                locked = (lockedStr == "1");

                users[userCount++] = User(fname, uname, contact, e, passw, locked);
            }

            file.close();
        }
    }

    void writeUsersToFile() {
        ofstream file(userFile.c_str());
        if (file.is_open()) {
            for (int i = 0; i < userCount; ++i) {
                file << users[i].fullname << ',' << users[i].username << ',' << users[i].contactNumber << ',' << users[i].email << ',' << users[i].password  << ',' << (users[i].isLocked ? "1" : "0") << endl;
            }
            file.close();
        }
    }

    void writeAdminsToFile() {
        ofstream file(adminFile.c_str());
        if (file.is_open()) {
            for (int i = 0; i < adminCount; ++i) {
                file << admins[i].adminname << ',' << admins[i].password << ',' << (admins[i].isLocked ? "1" : "0") << ',' << (admins[i].isSuperAdmin ? "1" : "0") << ',' << admins[i].sex << ',' << admins[i].age << ',' << admins[i].contactNumber << endl;
            }
            file.close();
        }
    }

    Admin* findAdmin(const string &adminname) {
        for (int i = 0; i < adminCount; ++i) {
            if (admins[i].adminname == adminname) {
                return &admins[i];
            }
        }
        return NULL;
    }

    bool login(const string &adminname, const string &password) {
        Admin* admin = findAdmin(adminname);
        if (admin != NULL && admin->password == password && !admin->isLocked) {
            isSuperAdmin = admin->isSuperAdmin;
            loggedInAdmin = admin;
            return true;
        }
        return false;
    }

    void addAdmin(const string &adminname, const string &password, bool isSuperAdmin) {
        string sex, contact;
        int age;
        int attempts = 0;

        while (true) {
            cout << "Please enter sex (male/female): ";
            cin >> sex;
            if (sex != "male" && sex != "female") {
				attempts++;
				if (attempts >= 2) {
					cout << "Invalid input! You have exceeded the maximum number of attempts." << endl;
					return;
				}
				cout << "Invalid sex! Please enter either 'male' or 'female'." << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			} else {
				break;
			}
		}
		
		attempts = 0;

    	while (true) {
        	cout << "Please enter age: ";
        	cin >> age;
        	if (cin.fail()) {
        	    attempts++;
        	    if (attempts >= 2) {
        	        cout << "Invalid input! You have exceeded the maximum number of attempts." << endl;
        	        return;
        	    }
        	    cout << "Invalid age! Please enter a number." << endl;

        	    cin.clear();
        	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
        	} else {
        	    break;
        	}
    	}

    	cin.ignore();

    	attempts = 0;

    	while (true) {
        	cout << "Please enter contact number: ";
        	getline(cin, contact);
        	if (contact.length() < 10 || contact.length() > 11) {
        	    attempts++;
        	    if (attempts >= 2) {
        	        cout << "Invalid input! You have exceeded the maximum number of attempts." << endl;
        	        return;
        	    }
        	    cout << "Invalid contact number! Please enter a number between 10 and 11 characters long." << endl;
        	} else {
        	    break;
        	}
    	}
	
    	if (adminCount < MAX_ADMINS) {
    	    admins[adminCount++] = Admin(adminname, password, false, isSuperAdmin, sex, age, contact);
    	    writeAdminsToFile();
    	    cout << "Admin added successfully!" << endl;
    	} else {
    	    cout << "Maximum number of admins reached! Cannot add new admin." << endl;
    	}
	}

	void editProfile() {
	    if (loggedInAdmin == NULL) {
    	    cout << "No admin is logged in." << endl;
    	    return;
    	}
	
    	string newPassword, newSex, newContact;
    	int newAge;
    	int attempts = 0;
	
    	cout << "Admin Details:" << endl;
    	cout << "Username: " << loggedInAdmin->adminname << endl;
    	cout << "Password: " << loggedInAdmin->password << endl;
    	cout << "Sex: " << loggedInAdmin->sex << endl;
    	cout << "Age: " << loggedInAdmin->age << endl;
    	cout << "Contact Number: " << loggedInAdmin->contactNumber << endl;
		
    	int choice;
    	cout << "Choose what to edit:" << endl;
    	cout << "1. Password" << endl;
    	cout << "2. Sex" << endl;
    	cout << "3. Age" << endl;
    	cout << "4. Contact Number" << endl;
    	cout << "5. Return to Main Menu" << endl;
    	cout << "Enter your choice: ";
    	cin >> choice;
    	cin.ignore();
		
    	switch (choice) {
    	    case 1: {
    	        cout << "Enter new password: ";
    	        getline(cin, newPassword);
    	        if (!newPassword.empty()) {
    	            loggedInAdmin->password = newPassword;
    	        }
    	        break;
    	    }

    	    case 2: {
    	        while (true) {
    	            cout << "Please enter sex (male/female): ";
    	            cin >> newSex;
    	            if (newSex != "male" && newSex != "female") {
    	                attempts++;
        	            if (attempts >= 2) {
        	                cout << "Invalid input! You have exceeded the maximum number of attempts." << endl;
        	                return;
        	            }
        	            cout << "Invalid sex! Please enter either 'male' or 'female'." << endl;
        	            cin.clear();
        	            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        	        } else {
        	            loggedInAdmin->sex = newSex;
        	            break;
        	        }
        	    }
        	    break;
        	}

        	case 3: {
            	while (true) {
            	    cout << "Please enter age: ";
            	    cin >> newAge;
            	    if (cin.fail()) {
            	        attempts++;
            	        if (attempts >= 2) {
            	            cout << "Invalid input! You have exceeded the maximum number of attempts." << endl;
            	            return;
            	        }
            	        cout << "Invalid age! Please enter a number." << endl;
	
            	        cin.clear();
            	        cin.ignore(numeric_limits<streamsize>::max(), '\n');
            	    } else {
                	    loggedInAdmin->age = newAge;
                	    break;
                	}
            	}
            	break;
        	}

        	case 4: {
            	while (true) {
            	    cout << "Please enter contact number: ";
            	    getline(cin, newContact);
            	    if (newContact.length() < 10 || newContact.length() > 11) {
            	        attempts++;
            	        if (attempts >= 2) {
            	            cout << "Invalid input! You have exceeded the maximum number of attempts." << endl;
            	            return;
            	        }
            	        cout << "Invalid contact number! Please enter a number between 10 and 11 characters long." << endl;
            	    } else {
            	        loggedInAdmin->contactNumber = newContact;
            	        break;
            	    }
            	}
            	break;
        	}

        	case 5:
            	return;

        	default:
            	cout << "Invalid choice!" << endl;
    	}

    	writeAdminsToFile();

    	char continueEditing;
    	cout << "Do you want to continue editing? (y/n): ";
    	cin >> continueEditing;
    	if (tolower(continueEditing) == 'y') {
    	    editProfile();
    	}
	}

	void viewAdminDetails(const string &adminname) {
    	if (!isSuperAdmin) {
    	    cout << "Only Super Admin can view admin details." << endl;
    	    return;
    	}

    	Admin* admin = findAdmin(adminname);
    	if (admin != NULL) {
    	    admin->displayAdmin();
    	} else {
    	    cout << "Admin not found!" << endl;
    	}
	}

	void lockUnlockAdmin(const string &adminname) {
    	if (!isSuperAdmin) {
    	    cout << "Only Super Admin can lock/unlock an admin." << endl;
    	    return;
    	}

    Admin* admin = findAdmin(adminname);
    	if (admin != NULL) {
    	    if (isSuperAdmin && admin->adminname == loggedInAdmin->adminname) {
    	        cout << "Super Admin cannot lock themselves!" << endl;
    	        return;
    	    }
    	    admin->isLocked = !admin->isLocked;
    	    cout << (admin->isLocked ? "Admin has been locked." : "Admin has been unlocked.") << endl;
    	    writeAdminsToFile();
    	} else {
    	    cout << "Admin not found!" << endl;
    	}
	}

	void lockUnlockUser(const string &username) {
    	if (!isSuperAdmin) {
    	    cout << "Only Super Admin can lock/unlock a user." << endl;
    	    return;
    	}

    	for (int i = 0; i < userCount; ++i) {
    	    if (users[i].username == username) {
        	    users[i].isLocked = !users[i].isLocked;
        	    cout << (users[i].isLocked ? "User has been locked." : "User has been unlocked.") << endl;
        	        writeUsersToFile();
        	        return;
        	    }
        	}
        cout << "User not found!" << endl;
    }

    void deleteAdmin(const string &adminname) {
        if (!isSuperAdmin) {
            cout << "Only Super Admin can delete an admin." << endl;
            return;
        }

        if (adminname == loggedInAdmin->adminname) {
            cout << "Super Admin cannot delete themselves!" << endl;
            return;
        }

        for (int i = 0; i < adminCount; ++i) {
            if (admins[i].adminname == adminname) {
                for (int j = i; j < adminCount - 1; ++j) {
                    admins[j] = admins[j + 1];
                }
                adminCount--;
                cout << "Admin deleted successfully." << endl;
                writeAdminsToFile();
                return;
            }
        }
        cout << "Admin not found." << endl;
    }

    void viewUserList() {
        if (!isSuperAdmin) {
            cout << "Only Super Admins can view the user list." << endl;
            return;
        }

        if (userCount == 0) {
            cout << "No users found." << endl;
            return;
        }

        int sortOption = 0;
        while (sortOption < 1 || sortOption > 3) {
            cout << "-----------------------------" << endl;
            cout << "Sorting Menu" << endl;
            cout << "-----------------------------" << endl;
            cout << "Select sorting order: \n1. Ascending (Counting Sort)\n2. Descending (Counting Sort)\n3. Ascending (Pancake Sort)\nEnter your choice: ";
            cin >> sortOption;
            if (sortOption < 1 || sortOption > 3) {
                cout << "Invalid choice. Please select 1, 2, or 3." << endl;
            }
        }

        if (sortOption == 1 || sortOption == 2) {
            countingSortUsers(users, userCount, sortOption == 1);
        } else if (sortOption == 3) {
            pancakeSortUsers(users, userCount);
        }

        cout << "\nList of users:\n";
        for (int i = 0; i < userCount; ++i) {
            users[i].displayUser();
        }
    }

    void countingSortUsers(User users[], int n, bool ascending) {
        const int MAX_CHAR = 256;

        User output[MAX_CHAR];
        int count[MAX_CHAR] = {0};

        for (int i = 0; i < n; i++) {
            count[tolower(users[i].username[0])]++;
        }

        if (ascending) {
            for (int i = 1; i < MAX_CHAR; i++) {
                count[i] += count[i - 1];
            }
        } else {
            for (int i = MAX_CHAR - 2; i >= 0; i--) {
                count[i] += count[i + 1];
            }
        }

        for (int i = n - 1; i >= 0; i--) {
            output[count[tolower(users[i].username[0])] - 1] = users[i];
            count[tolower(users[i].username[0])]--;
        }

        for (int i = 0; i < n; i++) {
            users[i] = output[i];
        }
    }

	void flip(User users[], int i) {
    	int start = 0;
    	while (start < i) {
    	    swap(users[start], users[i]);
    	    start++;
    	    i--;
    	}
	}

	int findMaxIndex(User users[], int n) {
    	int mi = 0;
    	for (int i = 1; i < n; ++i) {
    	    string username1 = users[i].username;
    	    string username2 = users[mi].username;

    	    // Convert usernames to lowercase
    	    transform(username1.begin(), username1.end(), username1.begin(), ::tolower);
    	    transform(username2.begin(), username2.end(), username2.begin(), ::tolower);

    	    if (username1 > username2) {
    	        mi = i;
    	    }
    	}
    	return mi;
	}

    void pancakeSortUsers(User users[], int n) {
        for (int curr_size = n; curr_size > 1; --curr_size) {
            int mi = findMaxIndex(users, curr_size);
            if (mi != curr_size - 1) {
                flip(users, mi);
                flip(users, curr_size - 1);
            }
        }
    }

    void searchUser() {
        string searchUsername;
        cout << "Enter the username to search: ";
        getline(cin, searchUsername);
        
        bool found = false;
        for (int i = 0; i < userCount; ++i) {
            if (users[i].username == searchUsername) {
                cout << "User found:" << endl;
                users[i].displayUser();
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "User not found." << endl;
        }
    }

    void run() {
        int choice;
        string adminname, password;

        while (true) {
            clearScreen();
            cout << "-----------------------------" << endl;
            cout << "Login" << endl;
            cout << "-----------------------------" << endl;
            cout << "Please enter your username: ";
            getline(cin, adminname);
            cout << "Please enter your password: ";
            getline(cin, password);

            if (login(adminname, password)) {
                cout << "Login successfully!" << endl;
                clearScreen();
                break;
            } else {
                cout << "Invalid credentials or account locked! Please try again." << endl;
            }
        }

        while (true) {
            cout << "\n-----------------------------" << endl;
            cout << "Main Menu" << endl;
            cout << "-----------------------------" << endl;
            cout << "1. Add Admin (Super Admin only)" << endl;
            cout << "2. Edit Profile" << endl;
            cout << "3. View Admin Details (Super Admin only)" << endl;
            cout << "4. Lock and Unlock Admin (Super Admin only)" << endl;
            cout << "5. Delete Admin (Super Admin only)" << endl;
            cout << "6. View User List" << endl;
            cout << "7. Search User" << endl;
            cout << "8. Lock and Unlock User (Super Admin only)" << endl;
            cout << "9. Exit" << endl;
            cout << "-----------------------------" << endl;
            cout << "\nPlease enter your choice: ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1: {
                    if (!isSuperAdmin) {
                        cout << "Only Super Admin can add a new admin." << endl;
                        break;
                    }

                    cout << "-----------------------------" << endl;
                    cout << "Add Admin" << endl;
                    cout << "-----------------------------" << endl;
                    cout << "Please enter new admin username: ";
                    getline(cin, adminname);
                    cout << "Please enter new admin password: ";
                    getline(cin, password);
                    addAdmin(adminname, password, false);
                    cout << "\nAdmin added successfully!" << endl;
                    break;
                }

                case 2: {
                    cout << "-----------------------------" << endl;
                    cout << "Edit Profile" << endl;
                    cout << "-----------------------------" << endl;
                    editProfile();
                    cout << "\nProfile updated successfully!" << endl;
                    break;
                }

                case 3: {
                    cout << "-----------------------------" << endl;
                    cout << "View Admin Details" << endl;
                    cout << "-----------------------------" << endl;
                    cout << "Enter username to view details: ";
                    getline(cin, adminname);
                    viewAdminDetails(adminname);
                    break;
				                }

                case 4: {
                    cout << "-----------------------------" << endl;
                    cout << "Lock/Unlock Admin" << endl;
                    cout << "-----------------------------" << endl;
                    cout << "Enter username to lock/unlock: ";
                    getline(cin, adminname);
                    lockUnlockAdmin(adminname);
                    break;
                }

                case 5: {
                    cout << "-----------------------------" << endl;
                    cout << "Delete Admin" << endl;
                    cout << "-----------------------------" << endl;
                    cout << "Enter username to delete: ";
                    getline(cin, adminname);
                    deleteAdmin(adminname);
                    break;
                }

                case 6:
                    viewUserList();
                    break;

                case 7:
                    searchUser();
                    break;

                case 8: {
                    cout << "-----------------------------" << endl;
                    cout << "Lock/Unlock User" << endl;
                    cout << "-----------------------------" << endl;
                    string username;
                    cout << "Enter username to lock/unlock: ";
                    getline(cin, username);
                    lockUnlockUser(username);
                    break;
                }

                case 9:
                    return;

                default:
                    cout << "Invalid choice!" << endl;
            }
        }
    }

    void clearScreen() {
        // Clear the screen
        #if defined(_WIN32)
            system("cls");
        #else
            system("clear");
        #endif
    }
};

int main() {
    const string afile = "admin.txt";
    const string ufile = "users.txt";
    AdminManager adminManager(afile, ufile);
    adminManager.run();
    return 0;
}