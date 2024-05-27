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

void writeAdminsToFile(const string &afile, const vector<Admin> &admins) {
    ofstream file(afile.c_str());
    if (file.is_open()) {
        for (size_t i = 0; i < admins.size(); ++i) {
            file << admins[i].adminname << ','
                 << admins[i].password << ','
                 << (admins[i].isLocked ? "1" : "0") << ','
                 << (admins[i].isSuperAdmin ? "1" : "0") << ','
                 << admins[i].sex << ','
                 << admins[i].age << ','
                 << admins[i].contactNumber << endl;
        }
        file.close();
    }
}


Admin* findAdmin(vector<Admin> &admins, const string &adminname) {
    for (size_t i = 0; i < admins.size(); ++i) {
        if (admins[i].adminname == adminname) {
            return &admins[i];
        }
    }
    return NULL;
}

bool login(vector<Admin> &admins, const string &adminname, const string &password, bool &isSuperAdmin) {
    Admin* admin = findAdmin(admins, adminname);
    if (admin != NULL && admin->password == password && !admin->isLocked) {
        isSuperAdmin = admin->isSuperAdmin;
        return true;
    }
    return false;
}

void addAdmin(vector<Admin> &admins, const string &adminname, const string &password, bool isSuperAdmin) {
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
}

void editProfile(Admin &admin) {
    string newPassword, newSex, newContact;
    int newAge;
    int attempts = 0;

    cout << "Admin Details:" << endl;
    cout << "Username: " << admin.adminname << endl;
    cout << "Password: " << admin.password << endl;
    cout << "Sex: " << admin.sex << endl;
    cout << "Age: " << admin.age << endl;
    cout << "Contact Number: " << admin.contactNumber << endl;

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
                admin.password = newPassword;
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
                    admin.sex = newSex;
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
                    admin.age = newAge;
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
                    admin.contactNumber = newContact;
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

    char continueEditing;
    cout << "Do you want to continue editing? (y/n): ";
    cin >> continueEditing;
    if (tolower(continueEditing) == 'y') {
        editProfile(admin);
    }
}

void viewAdminDetails(const Admin &admin) {
    admin.displayAdmin();
}

void lockUnlockAdmin(Admin &admin) {
    admin.isLocked = !admin.isLocked;
}

void deleteAdmin(vector<Admin> &admins, const string &adminname) {
    for (size_t i = 0; i < admins.size(); ++i) {
        if (admins[i].adminname == adminname) {
            admins.erase(admins.begin() + i);
            break;
        }
    }
}


int main() {
    const string afile = "admin.txt";
    vector<Admin> admins = readAdminsFromFile(afile);

    int choice;
    string adminname, password;
    bool isSuperAdmin;

    cout << "Welcome to Admin Management System for Restaurant Reservation!" << endl;
    cout << "Number of admin accounts read from file: " << admins.size() << endl;

    if (admins.empty()) {
        cout << "\nNo admin accounts found. Please register a Super Admin account." << endl;
        cout << "Enter Super Admin username: ";
        getline(cin, adminname);
        cout << "Enter Super Admin password: ";
        getline(cin, password);

        addAdmin(admins, adminname, password, true);
        writeAdminsToFile(afile, admins);

        cout << "Super Admin account created successfully!" << endl;
    } else {
        cout << "Admin accounts found." << endl;
    }

    while (true) {
        cout << "\n-----------------------------" << endl;
        cout << "Main Menu" << endl;
        cout << "-----------------------------" << endl;
        cout << "1. Login" << endl;
        cout << "2. Add Admin (Super Admin only)" << endl;
        cout << "3. Edit Profile" << endl;
        cout << "4. View Admin Details" << endl;
        cout << "5. Lock and Unlock Admin (Super Admin only)" << endl;
        cout << "6. Delete Admin (Super Admin only)" << endl;
        cout << "7. Exit" << endl;
        cout << "-----------------------------" << endl;
        cout << "\nPlease enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                cout << "-----------------------------" << endl;
                cout << "Login" << endl;
                cout << "-----------------------------" << endl;
                cout << "Please enter your username: ";
                getline(cin, adminname);
                cout << "Please enter your password: ";
                getline(cin, password);

                if (login(admins, adminname, password, isSuperAdmin)) {
                    cout << "Login successfully!" << endl;
                } else {
                    cout << "Invalid credentials or account locked!" << endl;
                }
                break;
            }

            case 2: {
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
                addAdmin(admins, adminname, password, false);
                writeAdminsToFile(afile, admins);
                cout << "\nAdmin added successfully!" << endl;
                break;
            }

            case 3: {
                cout << "-----------------------------" << endl;
                cout << "Edit Profile" << endl;
                cout << "-----------------------------" << endl;
                cout << "Enter your username: ";
                getline(cin, adminname);
                Admin* admin = findAdmin(admins, adminname);

                if (admin != NULL) {
                    editProfile(*admin);
                    writeAdminsToFile(afile, admins);
                    cout << "\nProfile updated successfully!" << endl;
                } else {
                    cout << "Oops, admin not found!" << endl;
                }
                break;
            }

            case 4: {
                if (!isSuperAdmin) {
                    cout << "Only Super Admin can view admin details." << endl;
                    break;
                }

                cout << "-----------------------------" << endl;
                cout << "View Admin Details" << endl;
                cout << "-----------------------------" << endl;
                cout << "Enter username to view details: ";
                getline(cin, adminname);
                Admin* viewAdmin = findAdmin(admins, adminname);
                if (viewAdmin != NULL) {
                    viewAdminDetails(*viewAdmin);
                } else {
                    cout << "Admin not found!" << endl;
                }
                break;
            }

            case 5: {
                if (!isSuperAdmin) {
                    cout << "Only Super Admin can lock/unlock an admin." << endl;
                    break;
                }

                cout << "-----------------------------" << endl;
                cout << "Lock/Unlock Admin" << endl;
                cout << "-----------------------------" << endl;
                cout << "Enter username to lock/unlock: ";
                getline(cin, adminname);
                Admin* lockAdmin = findAdmin(admins, adminname);
                if (lockAdmin != NULL) {
                    lockUnlockAdmin(*lockAdmin);
                    writeAdminsToFile(afile, admins);
                    cout << "\nAdmin lock status changed successfully!" << endl;
                } else {
                    cout << "Admin not found!" << endl;
                }
                break;
            }

            case 6: {
                if (!isSuperAdmin) {
                    cout << "Only Super Admin can delete an admin." << endl;
                    break;
                }

                cout << "-----------------------------" << endl;
                cout << "Delete Admin" << endl;
                cout << "-----------------------------" << endl;
                cout << "Enter username to delete: ";
                getline(cin, adminname);
                deleteAdmin(admins, adminname);
                writeAdminsToFile(afile, admins);
                cout << "\nAdmin deleted successfully!" << endl;
                break;
            }

            case 7:
                return 0;

            default:
                cout << "Invalid choice!" << endl;
        }
    }

    return 0;
}

