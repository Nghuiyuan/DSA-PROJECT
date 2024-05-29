#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>

using namespace std;

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
    vector<Admin> admins;
    string adminFile;
    Admin* loggedInAdmin;
    bool isSuperAdmin;

public:
    AdminManager(const string &afile) : adminFile(afile), loggedInAdmin(NULL), isSuperAdmin(false) {
        admins = readAdminsFromFile(adminFile);
        if (admins.empty()) {
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
        writeAdminsToFile();

        cout << "Super Admin account created successfully!" << endl;
    }

    vector<Admin> readAdminsFromFile(const string &afile) {
        vector<Admin> admins;
        ifstream file(afile.c_str());

        if (file.is_open()) {
            string line;

            while (getline(file, line)) {
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
                admins.push_back(Admin(admin, pw, locked, superAdmin, sex, age, contact));
            }

            file.close();
        }
        return admins;
    }

    void writeAdminsToFile() {
        ofstream file(adminFile.c_str());
        if (file.is_open()) {
            for (size_t i = 0; i < admins.size(); ++i) {
                file << admins[i].adminname << ',' << admins[i].password << ',' << (admins[i].isLocked ? "1" : "0") << ',' << (admins[i].isSuperAdmin ? "1" : "0") << ',' << admins[i].sex << ',' << admins[i].age << ',' << admins[i].contactNumber << endl;
            }
            file.close();
        }
    }

    Admin* findAdmin(const string &adminname) {
        for (size_t i = 0; i < admins.size(); ++i) {
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

        admins.push_back(Admin(adminname, password, false, isSuperAdmin, sex, age, contact));
        writeAdminsToFile();
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

    void deleteAdmin(const string &adminname) {
        if (!isSuperAdmin) {
            cout << "Only Super Admin can delete an admin." << endl;
            return;
        }

        if (adminname == loggedInAdmin->adminname) {
            cout << "Super Admin cannot delete themselves!" << endl;
            return;
        }

        for (size_t i = 0; i < admins.size(); ++i) {
            if (admins[i].adminname == adminname) {
                admins.erase(admins.begin() + i);
                cout << "Admin deleted successfully." << endl;
                writeAdminsToFile();
                return;
            }
        }
        cout << "Admin not found." << endl;
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
            cout << "6. Exit" << endl;
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
    AdminManager adminManager(afile);
    adminManager.run();
    return 0;
}
