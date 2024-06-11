#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
#include <cctype>
#include <algorithm>
#include <iomanip>

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
       	cout<<setw(12)<<left<<fullname<<setw(12)<<left<<username<<setw(20)<<left<<contactNumber<<setw(25)<<left<<email<<setw(10)<<left<<(isLocked ? "Yes" : "No")<<endl;
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
        cout << setw(12) << left << adminname
             << setw(10) << left << (isLocked ? "Yes" : "No")
             << setw(15) << left << (isSuperAdmin ? "Super Admin" : "Admin")
             << setw(10) << left << sex
             << setw(5) << left << age
             << setw(15) << left << contactNumber << endl;
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
    
    
    cout<<"------------------------------"<<endl;
    cout << "Admin Details:" << endl;
    cout<<"------------------------------"<<endl;
    cout << "Username: " << loggedInAdmin->adminname << endl;
    cout << "Password: " << loggedInAdmin->password << endl;
    cout << "Sex: " << loggedInAdmin->sex << endl;
    cout << "Age: " << loggedInAdmin->age << endl;
    cout << "Contact Number: " << loggedInAdmin->contactNumber << endl;

    int choice;
    cout << "\nChoose what to edit:" << endl;
    cout << "\n1. Password" << endl;
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

    cout << "\nUpdated Admin Details:" << endl;
    cout<<"------------------------------"<<endl;
    cout << "Username: " << loggedInAdmin->adminname << endl;
    cout << "Password: " << loggedInAdmin->password << endl;
    cout << "Sex: " << loggedInAdmin->sex << endl;
    cout << "Age: " << loggedInAdmin->age << endl;
    cout << "Contact Number: " << loggedInAdmin->contactNumber << endl;

    char confirmSave;
    cout << "Do you want to save the changes to file? (y/n): ";
    cin >> confirmSave;
    if (tolower(confirmSave) == 'y') {
        writeAdminsToFile();
        cout << "Changes saved to file." << endl;
    } else {
        cout << "Changes not saved." << endl;
    }

    char continueEditing;
    cout << "Do you want to continue editing? (y/n): ";
    cin >> continueEditing;
    if (tolower(continueEditing) == 'y') {
        editProfile();
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

    for (int i = 0; i < adminCount; ++i) {
        if (admins[i].adminname == adminname) {
            cout << "Are you sure you want to delete admin '" << adminname << "'? (y/n): ";
            char confirmDelete;
            cin >> confirmDelete;
            if (tolower(confirmDelete) == 'y') {
                for (int j = i; j < adminCount - 1; ++j) {
                    admins[j] = admins[j + 1];
                }
                adminCount--;
                cout << "Admin deleted successfully." << endl;
                writeAdminsToFile();
                return;
            } else {
                cout << "Deletion canceled." << endl;
                return;
            }
        }
    }
    cout << "Admin not found." << endl;
}


//ADMIN
	void viewAdminList() {
    	if (!isSuperAdmin) {
    	    cout << "Only Super Admin can view admin details." << endl;
    	    return;
    	}
    	
    	if(adminCount == 0)
    	{
    		cout<<"No admin found."<<endl;
    		return;
		}
		
		int sortad=0;
		
		while(sortad<1 || sortad>2)
		{
			cout<<"--------------------"<<endl;
			cout<<"Sorting Menu"<<endl;
			cout<<"--------------------"<<endl;
			cout<<"Select sorting order: "<<endl;
			cout<<"\n1. Ascending (Selection Sort) "<<endl;
		    cout<<"\n2. Descending (Quick Sort) "<<endl;
		    
		    cin>>sortad;
		    
		    if(sortad<1 || sortad>2)
		    {
		    	cout<<"Invalid choice. Please select option 1 or option 2."<<endl;
			}
		}
		
		if (sortad == 1)
		 {
		 selectionSortAdmins(admins, adminCount, true); // Ascending
		 } 
		 else if (sortad == 2) 
		 {
		 quickSortAdmins(admins, 0, adminCount - 1); // Descending
         }
		cout<<"--------------------"<<endl;
		cout<<"Admin List:"<<endl;
		cout<<"--------------------"<<endl;
		for(int i=0; i<adminCount; ++i)
		{
			admins[i].displayAdmin();
			cout<<"\n"<<endl;
		}
	}
	void selectionSortAdmins(Admin admins[], int n, bool ascending) 
{
    for (int i = 0; i < n - 1; ++i) 
	{
        int minIndex = i;
        for (int j = i + 1; j < n; ++j) 
		{
            if ((admins[j].adminname < admins[minIndex].adminname) == ascending) {
                minIndex = j;
            }
        }
        if (minIndex != i)
		 {
            swap(admins[i], admins[minIndex]);
        }
    }
}

//ADMIN
void quickSortAdmins(Admin admins[], int low, int high) 
{
    if (low < high) {
        int pivot = partition(admins, low, high);
        quickSortAdmins(admins, low, pivot - 1);
        quickSortAdmins(admins, pivot + 1, high);
    }
}

int partition(Admin admins[], int low, int high) {
    Admin pivot = admins[high];
    int i = low - 1;
    for (int j = low; j <= high - 1; ++j) {
        if (admins[j].adminname > pivot.adminname) {
            ++i;
            swap(admins[i], admins[j]);
        }
    }
    swap(admins[i + 1], admins[high]);
    return i + 1;
}

//ADMIN
void displayAdminHeader() {
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << setw(12) << left << "Adminname" 
         << setw(10) << left << "Locked" 
         << setw(15) << left << "Role" 
         << setw(10) << left << "Sex" 
         << setw(5) << left << "Age" 
         << setw(15) << left << "Contact Number" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
}

int binarySearchAdmin(Admin admins[], int left, int right, string key) {
    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (admins[mid].adminname == key)
            return mid;

        if (admins[mid].adminname < key)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1;
}

int jumpSearchAdminBySex(Admin admins[], int n, string key) {
    int step = sqrt(n);
    int prev = 0;

    while (admins[min(step, n) - 1].sex < key) {
        prev = step;
        step += sqrt(n);
        if (prev >= n)
            return -1;
    }

    while (admins[prev].sex < key) {
        prev++;
        if (prev == min(step, n))
            return -1;
    }

    if (admins[prev].sex == key)
        return prev;

    return -1;
}

void binarySearchByAdminname() {
    string searchAdminname;
    cout << "Enter the admin name to search: ";
    cin.ignore(); 
    getline(cin, searchAdminname);

    sort(admins, admins + adminCount, [](const Admin &a, const Admin &b) {
        return a.adminname < b.adminname;
    });

    int index = binarySearchAdmin(admins, 0, adminCount - 1, searchAdminname);

    if (index != -1) {
        cout << "Admin found:" << endl;
        displayAdminHeader();
        admins[index].displayAdmin();
    } else {
        cout << "Admin not found." << endl;
    }
}

void jumpSearchByAdminSex() {
    string searchSex;
    cout << "Enter the sex to search: ";
    cin.ignore();
    getline(cin, searchSex);

    sort(admins, admins + adminCount, [](const Admin &a, const Admin &b) {
        return a.sex < b.sex;
    });

    int index = jumpSearchAdminBySex(admins, adminCount, searchSex);

    if (index != -1) {
        cout << "Admins found:" << endl;
        displayAdminHeader();
        // Display all matching admins
        while (index < adminCount && admins[index].sex == searchSex) {
            admins[index].displayAdmin();
            index++;
        }
    } else {
        cout << "No admins found with the specified sex." << endl;
    }
}

void searchAdmin() {
    if (!isSuperAdmin) {
        cout << "Only Super Admin can search for an admin." << endl;
        return;
    }

    int choice;

    cout << "--------------------------------------------------------------------------------" << endl;
    cout << setw(12) << left << "Adminname" 
         << setw(10) << left << "Locked" 
         << setw(15) << left << "Role" 
         << setw(10) << left << "Sex" 
         << setw(5) << left << "Age" 
         << setw(15) << left << "Contact Number" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    for (int i = 0; i < adminCount; ++i) {
        admins[i].displayAdmin();
    }

    cout << "Select search option:" << endl;
    cout << "1. Binary Search by Admin Name" << endl;
    cout << "2. Jump Search by Admin Sex" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            binarySearchByAdminname();
            break;
        case 2:
            jumpSearchByAdminSex();
            break;
        default:
            cout << "Invalid choice." << endl;
            break;
    }
}
	

	void lockUnlockUser(const string &username) {
    	if (!isSuperAdmin) {
    	    cout << "Only Super Admin can lock/unlock a user." << endl;
    	    return;
    	}

    	for (int i = 0; i < userCount; ++i) {
    	    if (users[i].username == username) {
    	        // Prompt for confirmation
    	        char confirm;
    	        cout << "Are you sure you want to lock/unlock user '" << username << "'? (y/n): ";
    	        cin >> confirm;

    	        if (tolower(confirm) == 'y') {
    	            users[i].isLocked = !users[i].isLocked;
    	            cout << (users[i].isLocked ? "User has been locked." : "User has been unlocked.") << endl;
    	            writeUsersToFile();
    	        } else {
    	            cout << "Operation canceled." << endl;
    	        }
    	        return;
    	    }
    	}
    	cout << "User not found!" << endl;
	}

    
//ADMIN
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

        char confirm;
        cout << "Are you sure you want to lock/unlock admin '" << adminname << "'? (y/n): ";
        cin >> confirm;

        if (tolower(confirm) == 'y') {
            admin->isLocked = !admin->isLocked;
            cout << (admin->isLocked ? "Admin has been locked." : "Admin has been unlocked.") << endl;
            writeAdminsToFile();
        } else {
            cout << "Operation canceled." << endl;
        }
    } else {
        cout << "Admin not found!" << endl;
    }
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
        
        cout << "--------------------------------------------------------------------------------------------------------" << endl;
    	cout << setw(12) << left << "Fullname" << setw(12) << left << "Username" << setw(20) << left << "Contact Number" << setw(25) << left << "Email" << setw(10) << left << "Locked" << endl;
    	cout << "--------------------------------------------------------------------------------------------------------" << endl;
    	for (int i = 0; i < userCount; ++i) {
    	    users[i].displayUser();
    	}
    
        int sortOption = 0;
        while (sortOption < 1 || sortOption > 3) {
            cout << "-----------------------------" << endl;
            cout << "Sorting Menu" << endl;
            cout << "-----------------------------" << endl;
            cout << "Select sorting order: \n1. Ascending  (Sort Username using Counting Sort)\n2. Descending (Sort Username using Counting Sort)\n3. Ascending  (Sort Fullname using Pancake Sort)\nEnter your choice: ";
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

        cout<<"--------------------------------------------------------------------------------------------------------"<<endl;
		cout<<setw(12)<<left<<"Fullname"<<setw(12)<<left<<"Username"<<setw(20)<<left<<"Contact Number"<<setw(25)<<left<<"Email"<<setw(10)<<left<<"Locked"<<endl;
       	cout<<"--------------------------------------------------------------------------------------------------------"<<endl;
        for (int i = 0; i < userCount; ++i) {
            users[i].displayUser();
        }
    }

    void countingSortUsers(User users[], int n, bool ascending) {
    	const int MAX_CHAR = 256;
    
    	// Determine the maximum length of usernames for sorting
    	int maxLength = 0;
    	for (int i = 0; i < n; i++) {
    	    if (users[i].username.length() > maxLength) {
    	        maxLength = users[i].username.length();
    	    }
    	}

    	// Perform counting sort for each character position, starting from the end
    	for (int pos = maxLength - 1; pos >= 0; pos--) {
        	User *output = new User[n];
        	int count[MAX_CHAR] = {0};

        	// Count occurrence of each character at the current position
        	for (int i = 0; i < n; i++) {
        	    char charAtPos = pos < users[i].username.length() ? tolower(users[i].username[pos]) : 0;
        	    count[charAtPos]++;
        	}

        	// Adjust count array for ascending or descending order
        	if (ascending) {
        	    for (int i = 1; i < MAX_CHAR; i++) {
        	        count[i] += count[i - 1];
        	    }
        	} else {
        	    for (int i = MAX_CHAR - 2; i >= 0; i--) {
        	        count[i] += count[i + 1];
        	    }
        	}

        	// Build the output array based on the current character position
        	for (int i = n - 1; i >= 0; i--) {
        	    char charAtPos = pos < users[i].username.length() ? tolower(users[i].username[pos]) : 0;
        	    output[count[charAtPos] - 1] = users[i];
        	    count[charAtPos]--;
        	}

        	// Copy the sorted output back to the users array
        	for (int i = 0; i < n; i++) {
        	    users[i] = output[i];
        	}
        	delete[] output;
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
        	string fullname1 = users[i].fullname;
        	string fullname2 = users[mi].fullname;

        	// Convert fullnames to lowercase
        	transform(fullname1.begin(), fullname1.end(), fullname1.begin(), ::tolower);
        	transform(fullname2.begin(), fullname2.end(), fullname2.begin(), ::tolower);

        	if (fullname1 > fullname2) {
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

	void displayUserHeader() {
    	cout<<"--------------------------------------------------------------------------------------------------------"<<endl;
		cout<<setw(12)<<left<<"Fullname"<<setw(12)<<left<<"Username"<<setw(20)<<left<<"Contact Number"<<setw(25)<<left<<"Email"<<setw(10)<<left<<"Locked"<<endl;
       	cout<<"--------------------------------------------------------------------------------------------------------"<<endl;
	}

   void searchByContactNumber() {
    	string searchContactNumber;
    	cout << "Enter the contact number to search: ";
    	cin.ignore(); // To clear the newline character from the input buffer
    	getline(cin, searchContactNumber);

    	bool found = false;
    	for (int i = 0; i < userCount; ++i) {
    	    if (users[i].contactNumber == searchContactNumber) {
    	        if (!found) {
    	            cout << "User(s) found:" << endl;
    	            displayUserHeader();
        	        found = true;
        	    }
        	    users[i].displayUser();
        	}
    	}
    	if (!found) {
        	cout << "User not found." << endl;
    	}
	}

	
	void searchByFullname() {
    	string searchFullname;
    	cout << "Enter the full name to search: ";
    	cin.ignore(); // To clear the newline character from the input buffer
    	getline(cin, searchFullname);

    	// Convert searchFullname to lowercase
    	transform(searchFullname.begin(), searchFullname.end(), searchFullname.begin(), ::tolower);

    	bool found = false;
    	for (int i = 0; i < userCount; ++i) {
    	    string userFullname = users[i].fullname;
    	    // Convert userFullname to lowercase
    	    transform(userFullname.begin(), userFullname.end(), userFullname.begin(), ::tolower);

    	    if (userFullname == searchFullname) {
    	        cout << "User found:" << endl;
    	        displayUserHeader();
    	        users[i].displayUser();
    	        found = true;
    	        break;
    	    }
    	}
    	if (!found) {
    	    cout << "User not found." << endl;
    	}
	}

	void binarySearchByUsername() {
    	string searchUsername;
    	cout << "Enter the username to search: ";
    	cin.ignore(); // To clear the newline character from the input buffer
    	getline(cin, searchUsername);

    	// Convert searchUsername to lowercase
    	transform(searchUsername.begin(), searchUsername.end(), searchUsername.begin(), ::tolower);

    	// Sort users array by username before binary search
    	sort(users, users + userCount, [](const User &a, const User &b) {
    	    string usernameA = a.username;
    	    string usernameB = b.username;
    	    // Convert usernames to lowercase for comparison
    	    transform(usernameA.begin(), usernameA.end(), usernameA.begin(), ::tolower);
    	    transform(usernameB.begin(), usernameB.end(), usernameB.begin(), ::tolower);
    	    return usernameA < usernameB;
    	});

    	int left = 0;
    	int right = userCount - 1;
    	bool found = false;

    	while (left <= right) {
    	    int mid = left + (right - left) / 2;
    	    string midUsername = users[mid].username;
    	    // Convert midUsername to lowercase
    	    transform(midUsername.begin(), midUsername.end(), midUsername.begin(), ::tolower);

    	    if (midUsername == searchUsername) {
    	        cout << "User found:" << endl;
    	        displayUserHeader();
    	        users[mid].displayUser();
    	        found = true;
    	        break;
    	    }
    	    if (midUsername < searchUsername) {
    	        left = mid + 1;
    	    } else {
    	        right = mid - 1;
    	    }
    	}

    	if (!found) {
    	    cout << "User not found." << endl;
    	}
}

	void searchUser() {
    	int choice;
    	
    	cout << "--------------------------------------------------------------------------------------------------------" << endl;
    	cout << setw(12) << left << "Fullname" << setw(12) << left << "Username" << setw(20) << left << "Contact Number" << setw(25) << left << "Email" << setw(10) << left << "Locked" << endl;
    	cout << "--------------------------------------------------------------------------------------------------------" << endl;
    	for (int i = 0; i < userCount; ++i) {
    	    users[i].displayUser();
    	}
    	cout << "Select search option:" << endl;
    	cout << "1. String Search by Full Name" << endl;
    	cout << "2. String Search by Contact Number" << endl;
    	cout << "3. Binary Search by Username" << endl;
    	cout << "Enter your choice: ";
    	cin >> choice;

    	switch (choice) {
    	    case 1:
    	        searchByFullname();
    	        break;
    	    case 2:
    	        searchByContactNumber();
    	        break;
    	    case 3:
    	        binarySearchByUsername();
    	        break;
    	    default:
    	        cout << "Invalid choice." << endl;
    	        break;
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
            cout << "4. Search Admin(Super Admin Only)" << endl; 
            cout << "5. Lock and Unlock Admin (Super Admin only)" << endl;
            cout << "6. Delete Admin (Super Admin only)" << endl;
            cout << "7. View and Sort User List" << endl;
            cout << "8. View and Search User" << endl;
            cout << "9. Lock and Unlock User (Super Admin only)" << endl;
            cout << "10. Exit" << endl;
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
                	viewAdminList();
                	break;
				}
				
				case 4:
						searchAdmin();
						break;

                case 5: {
                    cout << "-----------------------------" << endl;
                    cout << "Lock/Unlock Admin" << endl;
                    cout << "-----------------------------" << endl;
                    cout << "Enter username to lock/unlock: ";
                    getline(cin, adminname);
                    lockUnlockAdmin(adminname);
                    break;
                }

                case 6: {
                    cout << "-----------------------------" << endl;
                    cout << "Delete Admin" << endl;
                    cout << "-----------------------------" << endl;
                    cout << "Enter username to delete: ";
                    getline(cin, adminname);
                    deleteAdmin(adminname);
                    break;
                }

                case 7:
                    viewUserList();
                    break;

                case 8:
                    searchUser();
                    break;

                case 9: {
                    cout << "-----------------------------" << endl;
                    cout << "Lock/Unlock User" << endl;
                    cout << "-----------------------------" << endl;
                    string username;
                    cout << "Enter username to lock/unlock: ";
                    getline(cin, username);
                    lockUnlockUser(username);
                    break;
                }

                case 10:
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