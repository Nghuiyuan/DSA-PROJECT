#include<iostream>
#include <limits>
#include<cstring>
#include<iomanip>
#include <fstream>
#include <cstdlib>
#include <sstream> 
#include <cctype> 
#include <algorithm> 
#include <stdexcept> 

using namespace std;
const int MAX_ADMINS = 100; // Maximum number of admins
const int MAX_USERS = 100;  // Maximum number of users

void mainmenu();
struct Food {
    string name;
    string category;
    float price;
    Food* next;
};
struct FoodItem {
    string name;
    string category;
    double price;
    int quantity;
    FoodItem* next;
};

struct CategoryNode {
    string category;
    FoodItem* foodHead;
    CategoryNode* next;
};

struct ReservationNode {
    string customerName;
    string reservationDateTime;
    int people;
    string phone;
    FoodItem* foodOrders;
    ReservationNode* next;

    ReservationNode() : customerName(""), reservationDateTime(""), people(0), phone(""), foodOrders(nullptr), next(nullptr) {}
    ReservationNode(const string& name, const string& dateTime, int numPeople, const string& phoneNumber)
        : customerName(name), reservationDateTime(dateTime), people(numPeople), phone(phoneNumber), foodOrders(nullptr), next(nullptr) {}
};
bool checkinput(string input) {
    if(input=="m"||input=="M") {
    	system("cls");
        mainmenu();
        return true;
    } else if(input=="e"||input=="E") {
         cout << "\nLogging out..." << endl;
        exit(0);
    }
    return false;    
}

bool isValidPrice(double price) {
    return price > 0; 
}
bool isValidPhoneNumber(const string& number) {
       if (number.length() != 10 && number.length() != 11) {
        return false;
    }
    if (number.substr(0, 2) != "01") {
        return false;
    }
    for (char c : number) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
    }
    
bool validatePasswordLength(const string& password) {
    return password.length() >= 8;
}

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


class AdminManager :public Admin {
private:
    Admin admins[MAX_ADMINS];
    string adminFile;
    string userFile;
    Admin* loggedInAdmin;
    bool isSuperAdmin;
    int adminCount;
    int userCount;

public:
    AdminManager(const string &afile) : adminFile(afile), loggedInAdmin(NULL), isSuperAdmin(false), adminCount(0){
        readAdminsFromFile(adminFile);
    }
    bool getisSuperAdmin()
    {
    	return isSuperAdmin;
	}
    void adminmenu()
    {	
		string ans,adminname; 
		bool valid=false;
    	cout<<"===================================================================="<<endl;
		cout<<"\t\tAdmin Management"<<endl;
		cout<<"===================================================================="<<endl;
		viewAdminList();
		cout<<"\nOperation : "<<endl;
		cout << "1. Admin Name Sort By Ascending Order (Selection Sort)" << endl;
        cout << "2. Admin Name Sort By Descending Order (Quick Sort)" << endl;
        cout<<"3. Binary Search by Admin Name"<<endl;
        cout<<"4. Jump Search by Admin Sex"<<endl;
        cout<<"5. Add Admin "<<endl;
        cout<<"6. Lock And Unlock Admin "<<endl;
        cout << "\nIf you want to go back to the main page, please enter 'm'." << endl;
		cout << "If you want to exit, please enter 'e'." << endl;
		cout<<"\nPlease select an action by entering its number: ";cin.clear();
		cin>>ans;
		do{	
		cin.clear();
		if(checkinput(ans))
		{	valid=true;	
		}
		else if(ans=="1")
		{	valid=true;
		system("cls");
			cout << "Admin Name sorted by Ascending Order:\n";
			selectionSortAdmins(admins, adminCount, true);
			 adminmenu();
		}else if(ans=="2")
		{	valid=true;system("cls");
			quickSortAdmins(admins, 0, adminCount - 1); 
			adminmenu(); 
		}else if(ans=="3")
		{	valid=true;
		binarySearchByAdminname();
		cout<<"\n"<<endl;
		adminmenu();
		}else if (ans == "4") {
		  valid=true;
		  jumpSearchByAdminSex();
		  cout<<"\n"<<endl;
		  adminmenu();
		}else if (ans == "5") {
		   valid=true;
		   addAdmin();
		}else if (ans == "6") {
		   valid=true;cin.ignore();
		   		   cout << "Enter username to lock/unlock: ";
                    getline(cin, adminname);
                    lockUnlockAdmin(adminname);cin.ignore();
                    adminmenu(); 
		}else{
		cout<<"Invalid selection.Please reenter your selection : ";
			cin>>ans;
		}
		}while(!valid);
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
     bool isContactUnique(const string &contact) 
	{for (int i = 0; i < adminCount; ++i) {
        if (admins[i].contactNumber == contact) {
            return false;
        }
    }
    return true;
}

bool isAdminnameUnique(const string &admin) {
    for (int i = 0; i < adminCount; ++i) {
        if (admins[i].adminname == admin) {
            return false;
        }
    }
    return true;
}
    void addAdmin() {
        string sex, contact, admin;
        int age;
        int attempts = 0;
        cin.ignore();
        string adminname;
        string password;
        cout<<"\n----------------------------"<<endl;
	cout<<"   Add New Admin"<<endl;
	cout<<"----------------------------"<<endl;
while (true) {
    
    cout << "Please enter new admin username: ";
    getline(cin, adminname);
    
    
    
    if (!isAdminnameUnique(adminname)) {
        cout << "Admin username already exists! Please enter a unique username." << endl;
        attempts++;
    } else {
        
        
        do
		{
		cout << "Please enter new admin password: ";
		fflush(stdin);
		getline(cin, password);	
		if(!validatePasswordLength(password))
		{
			cout<<"Password must be at least 8 characters long"<<endl;
		}
		 }while(!validatePasswordLength(password));
        
        break;
    }
    if (attempts > 2) {
        cout << "\nInvalid input! You have exceeded the maximum number of attempts.\n" << endl;
        adminmenu();
        return; // Return to main menu
    }
}


attempts=0;
        while (true) {
            cout << "Please enter sex (male/female): ";
            cin >> sex;
            if (sex != "male" && sex != "female") {
				attempts++;
				cout << "Invalid sex! Please enter either 'male' or 'female'." << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			} else {
				break;
			}
			if (attempts > 2) {
					cout << "\nInvalid input! You have exceeded the maximum number of attempts." << endl;adminmenu();
					return;
				}
		}
		
		attempts = 0;

    	while (true) {
        cout << "Please enter age: ";
        string input;
        cin >> input;

        try {
            size_t pos;
            age = stoi(input, &pos);

            if (pos != input.length()) {
                throw invalid_argument("Invalid input");
            }

            break;
        } catch (invalid_argument&) {
            attempts++;
            cout << "Invalid age! Please enter a valid number." << endl;

            if (attempts > 2) {
                cout << "\nInvalid input! You have exceeded the maximum number of attempts." << endl;
                adminmenu();
                return;
            }
        }
    }

    	cin.ignore();

    	attempts = 0;

    	 while (true) {
        cout << "Please enter contact number: ";
        getline(cin, contact);

        bool isValid = true;

        // Check if all characters are digits
        for (char c : contact) {
            if (!isdigit(c)) {
                isValid = false;
                break;
            }
        }

        if (contact.length() < 10 || contact.length() > 11 || !isValid) {
            attempts++;
            cout << "Invalid contact number! Please enter a number between 10 and 11 characters long, and make sure it contains only digits." << endl;
        } else if (!isContactUnique(contact)) {
            attempts++;
            cout << "Contact number already exists! Please enter a unique contact number." << endl;
        } else {
            break;
        }

        if (attempts > 2) {
            cout << "\nInvalid input! You have exceeded the maximum number of attempts." << endl;
            adminmenu();
            return;
        }
    }
    	if (adminCount < MAX_ADMINS) {
    	    admins[adminCount++] = Admin(adminname, password, false, false, sex, age, contact);
    	    writeAdminsToFile();
    	    cout << "Admin added successfully!" << endl;
    	} else {
    	    cout << "Maximum number of admins reached! Cannot add new admin." << endl;
    	}
    	adminmenu();
	}

void editProfile() {
    if (loggedInAdmin == NULL) {
        cout << "No admin is logged in." << endl;
        return;
    }

    string newPassword, newSex, newContact;
    int newAge;
    int attempts = 0;
    bool valid=false;
    
    cout<<"------------------------------"<<endl;
    cout << "Profile:" << endl;
    cout<<"------------------------------"<<endl;
    cout << "Username: " << loggedInAdmin->adminname << endl;
    cout << "Sex: " << loggedInAdmin->sex << endl;
    cout << "Age: " << loggedInAdmin->age << endl;
    cout << "Contact Number: " << loggedInAdmin->contactNumber << endl;

    string ans;
    cout << "\nOperation:" << endl;
    cout << "1. Password" << endl;
    cout << "2. Sex" << endl;
    cout << "3. Age" << endl;
    cout << "4. Contact Number" << endl;
    cout << "\nIf you want to go back to the main page, please enter 'm'." << endl;
		cout << "If you want to exit, please enter 'e'." << endl;
		cout<<"\nPlease select an action by entering its number: ";cin.clear();
		cin>>ans;
		do{	
		cin.clear();
		if(checkinput(ans))
		{	valid=true;	
		}
		else if(ans=="1")
		{	 valid = true;
			    string oldPassword;
                fflush(stdin);        
                cout << "\nEnter old password: ";
                getline(cin, oldPassword);

                if (oldPassword == loggedInAdmin->password) {
                    

do
		{
		cout << "Enter new password: ";
                    getline(cin, newPassword);
		if(!validatePasswordLength(newPassword))
		{
			cout<<"Password must be at least 8 characters long"<<endl;
		}
		else if (!newPassword.empty()) {
                        loggedInAdmin->password = newPassword;
                        cout << "Password successfully changed." << endl;
                       break;
                    } else {
                        cout << "New password cannot be empty." << endl;
                    }
		 }while(!validatePasswordLength(newPassword)&&!newPassword.empty());
                    
                } else {
                    cout << "Incorrect old password. Please try again.\n" << endl;
                }
                editProfile();
                		}
		else if(ans=="2"){	
		 valid = false;
            while (true) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter sex (male/female): "<<endl;
                cin >> newSex;
                if (newSex != "male" && newSex != "female") {
                    cout << "Invalid sex! Please enter either 'male' or 'female'.\n" << endl;
                    cin.clear();
                } else {
                    loggedInAdmin->sex = newSex;
                    cout << "Sex successfully changed." << endl;
                    valid = true;
                    break;
                }
            }
		}else if(ans=="3")
		{	valid=true;
	            while (true) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter age: ";
                cin >> newAge;
                
                if (cin.fail()) {
                     cout << "Invalid age! Please enter a number.\n" << endl;
                    cin.clear();
                } else {
                     loggedInAdmin->age = newAge;
                    break;
                }
            }
		}else if (ans == "4") {
		  valid=true;
		  while(true){
		fflush(stdin);
        cout << "Please enter new contact number: ";
        getline(cin, newContact);
        if (!isValidPhoneNumber(newContact)) {
            cout << "Invalid contact number! Please enter a phone number.\n" << endl;
        } else if (!isContactUnique(newContact)) {
            cout << "Contact number already exists! Please enter a unique contact number.\n" << endl;
        } else {
            loggedInAdmin->contactNumber = newContact;
            break;
        }
		  }
		}else{
		cout<<"Invalid selection.Please reenter your selection : ";
			cin>>ans;
		}
		}while(!valid);

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
    system("cls");
    cout << "Changes saved." << endl;
    cout << "\nProfile updated successfully!" << endl; // Added line
    editProfile();
} else {
	system("cls");
    cout << "Changes not saved.Back To Profile." << endl;
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
    
    if (adminCount == 0) {
        cout << "No admin found." << endl;
        return;
    }
    cout << "-------------------------------------------------------------------" << endl;
    cout << setw(12) << left << "Adminname" 
         << setw(10) << left << "Locked" 
         << setw(15) << left << "Role" 
         << setw(10) << left << "Sex" 
         << setw(5) << left << "Age" 
         << setw(15) << left << "Contact Number" << endl;
    cout << "-------------------------------------------------------------------" << endl;

    for (int i = 0; i < adminCount; ++i) {
        admins[i].displayAdmin();
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
    transform(key.begin(), key.end(), key.begin(), ::tolower); // Convert key to lowercase
    while (left <= right) {
        int mid = left + (right - left) / 2;

        // Convert adminname to lowercase before comparison
        string adminnameLower = admins[mid].adminname;
        transform(adminnameLower.begin(), adminnameLower.end(), adminnameLower.begin(), ::tolower);

        if (adminnameLower == key)
            return mid;

        if (adminnameLower < key)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1;
}
void squickSortAdmins(Admin admins[], int low, int high) {
    if (low < high) {
        int pivot = spartition(admins, low, high);
        squickSortAdmins(admins, low, pivot - 1);
        squickSortAdmins(admins, pivot + 1, high);
    }
}

int spartition(Admin admins[], int low, int high) {
    Admin pivot = admins[high];
    int i = low - 1;
    for (int j = low; j <= high - 1; ++j) {
        if (admins[j].adminname < pivot.adminname) {
            ++i;
            swap(admins[i], admins[j]);
        }
    }
    swap(admins[i + 1], admins[high]);
    return i + 1;
}

void selectionSortBySex(Admin admins[], int n, bool ascending) {
    for (int i = 0; i < n - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < n; ++j) {
            if ((admins[j].sex < admins[minIndex].sex) == ascending) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            swap(admins[i], admins[minIndex]);
        }
    }
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

    for (int i = prev; i < min(step, n); i++) {
        if (admins[i].sex == key)
            return i;
    }

    return -1;
}


void binarySearchByAdminname() {
    string searchAdminname;
    cout<<"\n----------------------------"<<endl;
	cout<<"   Search By Admin Name"<<endl;
	cout<<"----------------------------"<<endl;
    cout << "Enter the admin name to search: ";
    cin.ignore(); 
    getline(cin, searchAdminname);

    squickSortAdmins(admins,0, adminCount-1);

    int index = binarySearchAdmin(admins, 0, adminCount - 1, searchAdminname);
cout<<"\n"<<endl;
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
    cout<<"\n----------------------------"<<endl;
	cout<<"   Search By Admin Sex"<<endl;
	cout<<"----------------------------"<<endl;
    cout << "Enter the sex to search: ";
    cin.ignore();
    getline(cin, searchSex);

    selectionSortBySex(admins, adminCount, true);

    int index = jumpSearchAdminBySex(admins, adminCount, searchSex);
cout<<"\n"<<endl;
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

void lockUnlockAdmin(const string &adminname) {
   

    Admin* admin = findAdmin(adminname);
    if (admin != NULL) {
        if (isSuperAdmin && admin->adminname == loggedInAdmin->adminname) {
            cout << "Super Admin cannot lock themselves!\n" << endl;
            adminmenu();
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
	adminmenu();
}


}; 
AdminManager a("admin.txt");
class BaseUser {
protected:
    string fullname;
    string username;
    string contactNumber;
    string email;
    string password;
    bool isLocked;

public:
	friend class usermanager;
    BaseUser() {}

    BaseUser(string fname, string uname, string contact, string e, string passw, bool locked = false)
        : fullname(fname), username(uname), contactNumber(contact), email(e), password(passw), isLocked(locked) {}

    
};
class User : public BaseUser {
public:
    User() : BaseUser() {}

    User(string fname, string uname, string contact, string e, string passw, bool locked = false)
        : BaseUser(fname, uname, contact, e, passw, locked) {}
void displayUser() const {
        cout << setw(30) << left << fullname << setw(30) << left << username << setw(20) << left << contactNumber << setw(25) << left << email << setw(10) << left << (isLocked ? "Yes" : "No") << endl;
    }
};

class usermanager{
	    int userCount;
	    User users[MAX_USERS];
    string userFile;
public:
	usermanager(const string &ufile) : userFile(ufile),userCount(0) {
        readUsersFromFile(userFile);

    }
	void usermenu()
{	string ans;
string username;
	bool valid=false;
    	cout<<"=================================================================================="<<endl;
		cout<<"\t\t\tUser Management"<<endl;
		cout<<"=================================================================================="<<endl;
		viewUserList();
		cout<<"\nOperation : "<<endl;
		cout << "1. Ascending  (Sort Username using Counting Sort)" << endl;
        cout << "2. Descending (Sort Username using Counting Sort)" << endl;
        cout<<"3. Ascending  (Sort Fullname using Pancake Sort)"<<endl;
        cout<<"4. String Search by Full Name"<<endl;
        cout<<"5. String Search by Contact Number"<<endl;
        cout<<"6. Binary Search by Username"<<endl;
        cout<<"7. Lock and Unlock User "<<endl;
        cout << "\nIf you want to go back to the main page, please enter 'm'." << endl;
		cout << "If you want to exit, please enter 'e'." << endl;
		cout<<"\nPlease select an action by entering its number: ";cin.clear();
		cin>>ans;
		do{	
		cin.clear();
		if(checkinput(ans))
		{	valid=true;	
		}
		else if(ans=="1")
		{	valid=true;
		system("cls");
			cout << "User Name sorted by Ascending Order:\n";
			countingSortUsers(users, userCount, true);
			 usermenu();
		}else if(ans=="2")
		{	valid=true;
		system("cls");
			cout << "User Name sorted by Descending Order:\n";
			countingSortUsers(users, userCount, false);
			 usermenu();
		}else if(ans=="3")
		{	valid=true;
		system("cls");
			cout << "Fullname sorted by Ascending Order:\n";
			pancakeSortUsers(users, userCount);
			 usermenu();
		}else if(ans=="4")
		{	valid=true;
		cout << "-----------------------------" << endl;
        cout << "Search By Full Name" << endl;
        cout << "-----------------------------" << endl;
			searchByFullname();
			 usermenu();
		}else if(ans=="5")
		{	valid=true;
		cout << "-----------------------------" << endl;
        cout << "Search By Contact Number" << endl;
        cout << "-----------------------------" << endl;
		searchByContactNumber();
		usermenu();
		}else if(ans=="6")
		{	valid=true;
		cout << "-----------------------------" << endl;
        cout << "Search By Username" << endl;
        cout << "-----------------------------" << endl;
			 binarySearchByUsername();
			 usermenu();
		}else if(ans=="7")
		{	valid=true;
		cin.ignore();
		cout << "-----------------------------" << endl;
        cout << "Lock/Unlock User" << endl;
        cout << "-----------------------------" << endl;
        cout << "Enter username to lock/unlock: ";
        getline(cin, username);
        lockUnlockUser(username);
        cout<<"\n"<<endl;
		usermenu();
		}else{
		cout<<"Invalid selection.Please reenter your selection : ";
			cin>>ans;
		}
		}while(!valid);
}
	void lockUnlockUser(const string &username) {

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

    
    void viewUserList() {
        if (userCount == 0) {
            cout << "No users found." << endl;
            return;
        }
        
        cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
    	cout << setw(30) << left << "Fullname" << setw(30) << left << "Username" << setw(20) << left << "Contact Number" << setw(25) << left << "Email" << setw(10) << left << "Locked" << endl;
    	cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
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
	void displayUserHeader() {
        cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
		cout<<setw(30)<<left<<"Fullname"<<setw(30)<<left<<"Username"<<setw(20)<<left<<"Contact Number"<<setw(25)<<left<<"Email"<<setw(10)<<left<<"Locked"<<endl;
        cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
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
    	cout<<"\n"<<endl;
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
    	cout<<"\n"<<endl;
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
    	cout<<"\n"<<endl;
}

	void searchUser() {
    	int choice;
    	
    	cout << "--------------------------------------------------------------------------------------------------------" << endl;
    	cout << setw(30) << left << "Fullname" << setw(30) << left << "Username" << setw(20) << left << "Contact Number" << setw(25) << left << "Email" << setw(10) << left << "Locked" << endl;
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

};


class FoodList {
private:
    Food* head;// Pointer to the head of the linked list
    
        // Merge two sorted lists based on ascending order
 	 Food* merge(Food* left, Food* right, bool ascending) {
 	  // If right list is empty, return left list
        if (!left) return right;
        // If right list is empty, return left list
        if (!right) return left;
        // Decide the merge order based on the ascending parameter
        if ((ascending && toUpperCase(left->name) <= toUpperCase(right->name)) || (!ascending && toUpperCase(left->name) >= toUpperCase(right->name))) {
        // If left should come before right (ascending) or after right (descending)
			left->next = merge(left->next, right, ascending);
            return left;
        } else {
        // If right should come before left (ascending) or after left (descending)
            right->next = merge(left, right->next, ascending);
            return right;
        }
    }
    // Find the middle of the list to split it
    Food* getMiddle(Food* head) {
        if (head == nullptr) return head;
        Food* slow = head;
        Food* fast = head->next;
// Traverse the list with two pointers until fast reaches the end
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }

        return slow;// Return the middle node
    }
    	// Perform merge sort on the list
    Food* mergeSort(Food* head, bool ascending) {
        if (!head || !head->next) return head;
		// Find the middle node of the list
        Food* middle = getMiddle(head);
        Food* nextToMiddle = middle->next;

        middle->next = nullptr;// Split the list into two halves
		// Recursively sort the two halves
        Food* left = mergeSort(head, ascending);
        Food* right = mergeSort(nextToMiddle, ascending);
		// Merge the sorted halves
        return merge(left, right, ascending);
    }

	
public:
    FoodList() : head(nullptr) {}

     void readDataFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Failed to open file: " << filename << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name, category, priceStr;
            float price;
            if (getline(ss, name, ',') && getline(ss, category, ',') && getline(ss, priceStr, ',')) {
                if (priceStr.substr(0, 2) == "RM") {
                    priceStr = priceStr.substr(2); // Remove "RM" prefix
                }
                price = stof(priceStr); // Convert string to float
                addFood(name, category, price);
            } else {
                cerr << "Invalid data format in file: " << filename << endl;
            }
        }

        file.close();
    }
    string toUpperCase(const string& str) const {
        string upperStr = str;
        transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
        return upperStr;
    }
bool isFoodNameDuplicate(const string& name) {
    string upperName = toUpperCase(name);
    Food* temp = head;
    while (temp != nullptr) {
        if (toUpperCase(temp->name) == upperName) {
            return true;
        }
        temp = temp->next;
    }
    return false;
}
    void addFood(const string& name, const string& category, float price) {
    	// Allocate memory for a new Food node
        Food* newNode = new Food;
        newNode->name = name;
        newNode->category = category;
        newNode->price = price;
        newNode->next = nullptr;
		// Insert the new node at the end of the linked list
        if (head == nullptr) {
            head = newNode;
        } else {
            Food* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            // Insert newNode at the end
            temp->next = newNode;
        }
    }
	void saveDataToFile(const string& filename) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }

    Food* temp = head;
    while (temp != nullptr) {
    	
        outFile <<toUpperCase(temp->name)<< "," << temp->category << "," <<fixed<<setprecision(2)<< temp->price << endl;
        temp = temp->next;
    }

    outFile.close();
}
 void sortByNameAZ() {
        head = mergeSort(head, true);
    }

    void sortByNameZA() {
        head = mergeSort(head, false);
    }
    void printFoodList() {
        Food* temp = head;
        int i=0;
        string count;
        while (temp != nullptr) {
			count = to_string(i + 1) + ".";
            cout <<setw(4)<<count<<setw(35) << toUpperCase(temp->name)<< setw(15) << temp->category << setw(2)<<"RM"<<setw(8)<<fixed<<setprecision(2)<< temp->price << endl;
            temp = temp->next;
            i++;
        }
    }
    int getCount() {
    int count = 0;
    Food* temp = head;

    // Traverse the linked list and increment count for each node
    while (temp != nullptr) {
        count++;
        temp = temp->next;
    }

    return count;
	}
	Food* getFoodAtIndex(int index) {
    if (index < 1) {
        cerr << "Invalid index. Index must be greater than or equal to 1." << endl;
        return nullptr;
    }

    Food* temp = head;
    int count = 1; // Start counting from index 1

    // Traverse the linked list until the specified index
    while (temp != nullptr && count < index) {
        temp = temp->next;
        count++;
    }

    // Check if the index is valid
    if (temp == nullptr) {
        cerr << "Index out of range." << endl;
        return nullptr;
    }

    return temp;
}
void deleteFood(const string& nameToDelete) {
    if (head == NULL) {
        cerr << "Food list is empty. Nothing to delete." << endl;
        return;
    }

    Food* current = head;
    Food* prev = NULL;

    // Search for the food item to delete
    while (current != NULL && current->name != nameToDelete) {
        prev = current;
        current = current->next;
    }

    // If the food item was not found
    if (current == NULL) {
        cerr << "\nFood item '" << nameToDelete << "' not found in the list." << endl;
        return;
    }

    // If the found food item is the head of the list
    if (prev == NULL) {
        head = head->next;
        delete current;
    } else {
        prev->next = current->next;
        delete current;
    }
	saveDataToFile("fooddata.txt");
    cout << "\nFood item '" << nameToDelete << "' deleted successfully." << endl;
	 cout << "Back to menu management page..." << endl;
}

void searchByCategory(const string& targetCategory) {
	system("cls");
    bool found = false; // Flag to indicate if any matching food items are found
    Food* temp = head; // Temporary pointer to traverse the food linked list
    int count = 0; // Count of matching food items
    string uppercaseTargetCategory = targetCategory;
    // Convert target category to uppercase
    transform(uppercaseTargetCategory.begin(), uppercaseTargetCategory.end(), uppercaseTargetCategory.begin(), ::toupper); 
    string word;
    stringstream ss(uppercaseTargetCategory);

    // Process target category word by word
    while (ss >> word) {
        // Traverse the food linked list to find foods matching the specified category
        temp = head;
        while (temp != nullptr) {
            string uppercaseCategory = temp->category;
             // Convert current food's category to uppercase
            transform(uppercaseCategory.begin(), uppercaseCategory.end(), uppercaseCategory.begin(), ::toupper);

            // If the current food's category contains the word from the target category, print food information
            if (uppercaseCategory.find(word) != string::npos) {
                if (!found) {
                    cout << "\n\nFood items in category '" << targetCategory << "':" << endl;
                    cout << "--------------------------------------------------------------------" << endl;
                    cout << setw(3) << "No.  " << setw(35) << left << "Food" << setw(15) << "Category" << setw(10) << "Price" << endl;
                    cout << "--------------------------------------------------------------------" << endl;
                    found = true;
                }
                cout << setw(4) << ++count << setw(35) << left << temp->name << setw(15) << temp->category << setw(2) << "RM" << setw(8) << fixed << setprecision(2) << temp->price << endl;
            }
            temp = temp->next; // Move to the next food node
        }
    }

    if (!found) {
        cout << "No food items found in category '" << targetCategory << "'." << endl;
    }
    cout<<"\n\nBack To Menu Manegement Page..."<<endl;
}  

void searchByname(const string& targetname) {
	system("cls");
    bool found = false; // Flag to indicate if any matching food items are found
    Food* temp = head; // Temporary pointer to traverse the food linked list
    int count = 0; // Count of matching food items
    string uppercaseTargetName = targetname;
    transform(uppercaseTargetName.begin(), uppercaseTargetName.end(), uppercaseTargetName.begin(), ::toupper); // Convert target category to uppercase

    string word;
    stringstream ss(uppercaseTargetName);

    // Process target category word by word
    while (ss >> word) {
        // Traverse the food linked list to find foods matching the specified category
        temp = head;
        while (temp != nullptr) {
            string uppercasename = temp->name;
            transform(uppercasename.begin(), uppercasename.end(), uppercasename.begin(), ::toupper); // Convert current food's category to uppercase

            // If the current food's category contains the word from the target category, print food information
            if (uppercasename.find(word) != string::npos) {
                if (!found) {
					cout << "\n\nSearch Result of '" << targetname << "' :" << endl;
                    cout << "--------------------------------------------------------------------" << endl;
                    cout << setw(3) << "No.  " << setw(35) << left << "Food" << setw(15) << "Category" << setw(10) << "Price" << endl;
                    cout << "--------------------------------------------------------------------" << endl;
                    found = true;
                }
                cout << setw(4) << ++count << setw(35) << left << temp->name << setw(15) << temp->category << setw(2) << "RM" << setw(8) << fixed << setprecision(2) << temp->price << endl;
            }
            temp = temp->next; // Move to the next food node
        }
    }

    if (!found) {
        cout << "No food items found in category '" << targetname << "'." << endl;
    }
    cout<<"\n\nBack To Menu Manegement Page..."<<endl;
}
  
  void insertionSort(Food*& head, bool ascending) {
    if (head == nullptr || head->next == nullptr) return;

    Food* sorted = nullptr;// Pointer to the sorted part of the list
    Food* current = head;// Pointer to traverse the original list
	// Traverse through the original list
    while (current != nullptr) {
        Food* next = current->next;
		// Insert current node into the sorted list
        if (ascending && (sorted == nullptr || current->price <= sorted->price) ||
            !ascending && (sorted == nullptr || current->price >= sorted->price)) {
            current->next = sorted;
            sorted = current;// Update sorted list head to current node
        } else {
    // Update sorted list head to current node
            Food* temp = sorted;
            while (temp->next != nullptr && ((ascending && temp->next->price < current->price) ||
                                             (!ascending && temp->next->price > current->price))) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }

        current = next;// Move to the next node in the original list
    }

    head = sorted;// Update the head pointer of the original list to the sorted list head
}

  void sortByPriceAscending() {
    insertionSort(head, true);
}

void sortByPriceDescending() {
    insertionSort(head, false);
}
  
};


class foodmenu :public FoodList{
	
	double price; 
	public: 
	foodmenu()
	{   readDataFromFile("fooddata.txt");}
	friend bool isValidPrice(double price);
	void displayaction()
	{	string ans,fname,foodname;
		bool valid=false;
		cout<<"===================================================================="<<endl;
		cout<<"\t\tMenu Management"<<endl;
		cout<<"===================================================================="<<endl;
		cout<<setw(3)<<"No.  "<<setw(35)<<left<<"Food"<<setw(15)<<"Category"<<setw(10)<<"Price"<<endl;
		cout<<"--------------------------------------------------------------------"<<endl;
		printFoodList();
		cout<<"\nOperation:\n1.Add Food"<<endl;
		cout<<"2.Edit Food"<<endl;
		cout<<"3.Delete Food"<<endl;
		cout<<"4.Search by Category Name (linear search)"<<endl;
		cout<<"5.Search by Food Name (linear search)"<<endl; 
		cout << "6.Sort Food by A-Z (merge sort)" << endl;
		cout << "7.Sort Food by Z-A (merge sort)" << endl;
cout << "8.Sort Food by price low to high (insertion sort)" << endl;
cout << "9.Sort Food by price high to low (insertion sort)" << endl;
		cout << "\nIf you want to go back to the main page, please enter 'm'." << endl;
		cout << "If you want to exit, please enter 'e'." << endl;
		cout<<"\nPlease select an action by entering its number: ";
		cin>>ans;
		do{	cin.clear();
		if(checkinput(ans))
		{
			valid=true;
		}
		else if(ans=="1")
		{	valid=true;
			addfood();
		}else if(ans=="2")
		{valid=true;
			editfood();
		}
		else if(ans=="3")
		{valid=true;
		deletefood();
		}else if (ans == "4") {
		    cin.ignore();
		    valid = true;
		    string category;
		    cout<<"----------------------------"<<endl;
		    cout<<"   Search By Category"<<endl;
		    cout<<"----------------------------"<<endl;
		    cout << "Enter Category Name : ";
		    getline(cin, category);
		    searchByCategory(category);
		    displayaction();
		}else if (ans == "5") {
		    cin.ignore();
		    valid = true;
		    string category;
		    cout<<"----------------------------"<<endl;
		    cout<<"   Search By Food Name"<<endl;
		    cout<<"----------------------------"<<endl;
		    cout << "Enter Food Name : ";
		    getline(cin, foodname);
		    searchByname(foodname);
		    displayaction();
		}
		else if (ans == "6") {
                valid = true;
                sortByNameAZ();
                system("cls");
                cout << "Food sorted by A-Z :\n";
                displayaction();
            } else if (ans == "7") {
                valid = true;
                sortByNameZA();
                system("cls");
                cout << "Food sorted by Z-A :\n";
                displayaction();
            }else if (ans == "8") {
    valid = true;
    sortByPriceAscending();
    system("cls");
    cout << "Food sorted by price in ascending order:\n";
    displayaction();
} else if (ans == "9") {
    valid = true;
    sortByPriceDescending();
    system("cls");
    cout << "Food sorted by price in descending order:\n";
    displayaction();
}
else{
			cout<<"Invalid selection.Please reenter your selection : ";
			cin>>ans;}
		}while(!valid);
		
		
	}
	void addfood()
	{	system("cls");
	string c;
	bool valid=false;
		cout<<"============================================="<<endl;
		cout<<"\t\tAdd Food"<<endl;
		cout<<"============================================="<<endl;
		cout<<"Category"<<endl;
		cout<<"1.Main Course"<<endl;
		cout<<"2.Side Dishes  "<<endl;
		cout<<"3.Beverages "<<endl;
		cout << "\nIf you want to go back to the previous page, please enter 'b'." << endl;
		cout << "If you want to go back to the main page, please enter 'm'." << endl;
		cout << "If you want to exit, please enter 'e'." << endl;
		cout << "\nPlease choose one category to which you wish to add food: ";
		cin>>c;
		do{	cin.clear();
			if(!checkinput(c))
			{
			if(c=="1"){
				valid=true;
				addfood("Main Course");
				
			}else if(c=="2"){
				valid=true;
				addfood("Side Dishes");
			}else if(c=="3"){
				valid=true;
				addfood("Beverages");
			}
			else if(c=="B"||c=="b"){
				valid=true;
				system("cls");
				displayaction();
			}else{
				cout<<"Invalid Selection.Please reenter your choice:";
				cin>>c;
			}
				
			}else{
				valid=true;
			}
			
		}while(!valid);
	}
	void addfood(string category)
	{	string foodname,confirm;
		bool valid=false;
		
		cout<<"**************************************"<<endl;
		cout<<"	Add Food into "<<category<<endl;
		cout<<"**************************************"<<endl;
			cout << "\nIf you want to go back to the Menu Management page, please enter 'b'." << endl;
		cout << "If you want to go back to the main page, please enter 'm'." << endl;
		cout << "If you want to exit, please enter 'e'." << endl;
		do{
		cout<<"Please enter food name  : ";
		fflush(stdin);	
			getline(cin,foodname);
			if(!checkinput(foodname)){
				if(isFoodNameDuplicate(foodname))
			{
				cout<<"Food Name Already Exist.Please try again.\n"<<endl;
			}
			}
			if(foodname=="B"||foodname=="b"){
				system("cls");
				displayaction();break;
			}
		}while(isFoodNameDuplicate(foodname));
	
		cout<<"Please enter food price : RM";
		while (!(cin >> price) || !isValidPrice(price)) {
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid food price : RM";
        } else {
            cout << "Invalid price. Please enter a valid positive number: ";
        }
    }
    displayfooddetails(category,foodname,price);
    cout<<"Please Check the new food details\nAre you sure to add the food into "<<category<<"?[Y/N]:";
    do{
    	cin>>confirm;
    	cin.clear();
    	if(confirm=="Y"||confirm=="y")
    	{
    		addFood(foodname,category,price);
    		saveDataToFile("fooddata.txt");
    		system("cls");
    		cout<<"\nNew Food Added Successfully!\nBack To Food Managemnt Page\n"<<endl;
    		displayaction();
    		valid=true;
    		
		}else if(confirm=="N"||confirm=="n")
		{	valid=true;system("cls");

			cout<<"Back to menu management page..."<<endl;
			displayaction();
		}else{
			cout<<"Invalid Input.Please reenter your input : ";
		}
	}while(!valid);
	}
	void displayfooddetails(string category,string fname,float price)
	{
		cout<<"\n-------------------------"<<endl;
		cout<<"\tFood Details"<<endl;
		cout<<"-------------------------"<<endl;
		cout<<"Food Name  : "<<fname<<endl;
		cout<<"Category   : "<<category<<endl;
		cout << "Food Price : RM" << fixed << setprecision(2) << price << endl;
	}
	
void editfood() {
    system("cls");
    cout << "====================================================================" << endl;
    cout << "    Edit Food  " << endl;
    cout << "====================================================================" << endl;
    cout << setw(3) << "No.  " << setw(35) << left << "Food" << setw(15) << "Category" << setw(10) << "Price" << endl;
    cout << "--------------------------------------------------------------------" << endl;
    printFoodList();
    cout << "\nIf you want to go back to the previous page, please enter 'b'." << endl;
    cout << "If you want to go back to the main page, please enter 'm'." << endl;
    cout << "If you want to exit, please enter 'e'." << endl;
    
    string input;
    bool inputvalid = false;
    int index;
    cout << "Enter the index of the food item you want to edit: ";
    cin >> input;

    do {
        if (input == "B" || input == "b") {
            inputvalid = true;
            system("cls");
            displayaction();
            return; // Exit the function after going back
        }
        if (!checkinput(input)) {
            if (isdigit(input[0])) {
                index = stoi(input);
                if (index < 1 || index > getCount()) {
                    cout << "Invalid index. Please enter a valid index: ";
                    cin >> input;
                } else {
                    inputvalid = true;
                }
            } else {
                cout << "Invalid input. Please enter a valid input: ";
                cin >> input;
            }
        } else {
            inputvalid = true;
        }
    } while (!inputvalid);

    // Retrieve the food item from the linked list
    Food* temp = getFoodAtIndex(index);

    // Create a copy of the original food item
    Food original = *temp;

    // Display the details of the food item to be edited
    displayfooddetails(temp->category, temp->name, temp->price);
    
    // Prompt the user for changes
    string newName, newCategory;
    double newPrice;
    string ans, confirm, choice;
    cout << "\nIf you want to go back to the Menu Management page, please enter 'b'." << endl;
    cout << "If you want to go back to the main page, please enter 'm'." << endl;
    cout << "If you want to exit, please enter 'e'." << endl;

    do {
        cout << "\nChoose what you want to edit:" << endl;
        cout << "1. Food Name" << endl;
        cout << "2. Category" << endl;
        cout << "3. Price" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
cin.clear(); // Clear error flags
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        if (choice == "1") {
        	do{
        	cout << "Enter the new food name: ";
            getline(cin, newName);

            if (!checkinput(newName)) {
                if (isFoodNameDuplicate(newName)) {
                    cout << "Food name already exists. Please try again." << endl;
                    continue;
                }else{
            	temp->name = newName;
            	break;
			} 
        }cin.clear(); // Clear error flags
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
			}while(true);
            
        } else if (choice == "2") {
            cout << "\nCategory options:" << endl;
            cout << "1. Main Course" << endl;
            cout << "2. Side Dishes" << endl;
            cout << "3. Beverages" << endl;
            cout << "Choose the new category (1, 2, 3): ";
            cin >> ans;

            while (true) {
                if (ans == "1") {
                    newCategory = "Main Course";
                    break;
                } else if (ans == "2") {
                    newCategory = "Side Dishes";
                    break;
                } else if (ans == "3") {
                    newCategory = "Beverages";
                    break;
                } else {
                    cout << "Invalid selection. Please enter 1, 2, or 3: ";
                    cin >> ans;
                }
            }
            temp->category = newCategory;
        } else if (choice == "3") {
            cout << "Enter the new price (RM): ";
            while (!(cin >> newPrice) || !isValidPrice(newPrice)) {
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a valid food price (RM): ";
                } else {
                    cout << "Invalid price. Please enter a valid positive number: ";
                }
            }
            temp->price = newPrice;
        } else if (choice == "b" || choice == "B") {
            // Restore original data
            *temp = original;
            system("cls");
            displayaction();
            return; // Exit the function after going back
        } else if (choice == "m" || choice == "M") {
            // Restore original data
            *temp = original;
            system("cls");
            mainmenu();
            return; // Exit the function after going back
        } else if (choice == "e" || choice == "E") {
            exit(0);
        } else {
            cout << "Invalid selection. Please enter 1, 2, 3, 'b', 'm', or 'e': ";
        }

        // Ask if user wants to edit another property
        cout << "\nDo you want to edit another property? (yes/no): ";
        cin >> confirm;
    } while (confirm == "yes" || confirm == "Yes" || confirm == "YES"|| confirm == "Y"|| confirm == "y");

    displayfooddetails(temp->category, temp->name, temp->price);
    cout << "Please check the new food details.\nAre you sure to edit the food? [Y/N]: ";
    
    bool valid = false;
    do {
        cin >> confirm;
        cin.clear();
        if (confirm == "Y" || confirm == "y") {
            // Save the updated data to the file
            saveDataToFile("fooddata.txt");
            system("cls");
            cout << "\nFood item edited successfully!\n" << endl;
            displayaction();
            valid = true;
        } else if (confirm == "N" || confirm == "n") {
            // Restore original data
            *temp = original;
            valid = true;
            system("cls");
            cout << "Back to menu management page..." << endl;
            displayaction();
        } else {
            cout << "Invalid input. Please reenter your input: ";
        }
    } while (!valid);
}

void deletefood() {
    system("cls");
    bool inputvalid=false;
    int index=0;
    cout << "====================================================================" << endl;
    cout << "\t\tDelete Food" << endl;
    cout << "====================================================================" << endl;
    cout << setw(3) << "No.  " << setw(35) << left << "Food" << setw(15) << "Category" << setw(10) << "Price" << endl;
    cout << "--------------------------------------------------------------------" << endl;
	printFoodList();
    cout << "\nIf you want to go back to the previous page, please enter 'b'." << endl;
    cout << "If you want to go back to the main page, please enter 'm'." << endl;
    cout << "If you want to exit, please enter 'e'." << endl;
    string nameToDelete;
    cout << "\nEnter the Index of the food item you want to delete: ";
		fflush(stdin);
	    getline(cin, nameToDelete);
   do {
		if(nameToDelete=="B"||nameToDelete=="b"){
	    	system("cls");
	    	displayaction();
		}
	    if (!checkinput(nameToDelete)) {
	        if (isdigit(nameToDelete[0])) { 
	            index = stoi(nameToDelete);
	            if (index < 1 || index > getCount()) {
	                cout << "Invalid index. Please enter a valid index: ";
	                getline(cin, nameToDelete);
	            } else {
	                inputvalid = true;
	            }
	        } else {
	            cout << "Invalid input. Please enter a valid input: ";
	            getline(cin, nameToDelete);
	        }
	    } else {
	        inputvalid = true;
	    }
} while (!inputvalid);
 	bool deleteans=false;
 	string confirm;
 	Food* temp = getFoodAtIndex(index);
	displayfooddetails(temp->category,temp->name,temp->price);
	cout<<"Are you sure you want to delete this food ? [Y/N] : ";
	do{
    	cin>>confirm;
    	cin.clear();
    	if(confirm=="Y"||confirm=="y")
    	{
		   nameToDelete=temp->name;
		 // Delete the food item
	    deleteFood(nameToDelete);
	    saveDataToFile("fooddata.txt");
	    system("cls");
	    cout<<"Food deleted successfully!!!"<<endl;
	    cout<<"Back to menu management page..."<<endl;
	    displayaction();
    		deleteans=true;
    		
		}else if(confirm=="N"||confirm=="n")
		{	deleteans=true;
			system("cls");
			cout<<"Back to menu management page..."<<endl;
			displayaction();
		}else{
			cout<<"Invalid Input.Please reenter your input : ";
		}
	}while(!deleteans);
   
}


	
};


class ReservationList {
private:
    CategoryNode* categoryHead;
    ReservationNode* head;
    ReservationNode* tail;
    const int reservationYear = 2024;
friend void mainmenu();
    ReservationNode* mergeSortedLists(ReservationNode* left, ReservationNode* right, bool ascending = true) {
        if (!left) return right;
        if (!right) return left;

        if ((ascending ? (left->reservationDateTime < right->reservationDateTime) : (left->reservationDateTime > right->reservationDateTime))) {
            left->next = mergeSortedLists(left->next, right, ascending);
            return left;
        } else {
            right->next = mergeSortedLists(left, right->next, ascending);
            return right;
        }
    }

    ReservationNode* mergeSort(ReservationNode* head, bool ascending) {
        if (!head || !head->next) return head;

        ReservationNode* slow = head;
        ReservationNode* fast = head->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        ReservationNode* mid = slow->next;
        slow->next = nullptr;

        ReservationNode* left = mergeSort(head, ascending);
        ReservationNode* right = mergeSort(mid, ascending);

        return mergeSortedLists(left, right, ascending);
    }

    ReservationNode* mergeSortedByName(ReservationNode* left, ReservationNode* right) {
        if (!left) return right;
        if (!right) return left;

        if (left->customerName < right->customerName) {
            left->next = mergeSortedByName(left->next, right);
            return left;
        } else {
            right->next = mergeSortedByName(left, right->next);
            return right;
        }
    }

    ReservationNode* mergeSortByName(ReservationNode* head) {
        if (!head || !head->next) return head;

        ReservationNode* slow = head;
        ReservationNode* fast = head->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        ReservationNode* mid = slow->next;
        slow->next = nullptr;

        ReservationNode* left = mergeSortByName(head);
        ReservationNode* right = mergeSortByName(mid);

        return mergeSortedByName(left, right);
    }

        void loadFoodItems() {
        ifstream inFile("fooddata.txt");
        if (!inFile) {
            cerr << "Failed to open food data file for reading." << endl;
            return;
        }
        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            FoodItem* item = new FoodItem();
            if (getline(ss, item->name, ',') && getline(ss, item->category, ',') && (ss >> item->price)) {
                item->next = nullptr;
                addFoodItem(item);
            }
        }
        inFile.close();
    }

    void addFoodItem(FoodItem* item) {
        CategoryNode* currentCategory = categoryHead;
        while (currentCategory && currentCategory->category != item->category) {
            currentCategory = currentCategory->next;
        }

        if (!currentCategory) {
            currentCategory = new CategoryNode();
            currentCategory->category = item->category;
            currentCategory->foodHead = nullptr;
            currentCategory->next = categoryHead;
            categoryHead = currentCategory;
        }

        item->next = currentCategory->foodHead;
        currentCategory->foodHead = item;
    }

        void addFoodOrder(ReservationNode* reservation, FoodItem* item, int quantity) {
        FoodItem* newOrder = new FoodItem(*item);
        newOrder->next = reservation->foodOrders;
        reservation->foodOrders = newOrder;
        newOrder->quantity = quantity;
        newOrder->price = item->price;
    }
    
    void searchByName(const string& substring) {
    string searchInput = substring;
    // Correctly convert the search input to lower case
    std::transform(searchInput.begin(), searchInput.end(), searchInput.begin(),
                   [](unsigned char c) -> unsigned char { return std::tolower(static_cast<int>(c)); });

    head = mergeSortByName(head); // Ensures the list is sorted by name for more intuitive output.

    bool found = false;
    ReservationNode* tempHead = nullptr;
    ReservationNode* tempTail = nullptr;

    for (ReservationNode* current = head; current != nullptr; current = current->next) {
        string currentName = current->customerName;
        // Convert current reservation name to lower case
        transform(currentName.begin(), currentName.end(), currentName.begin(),
                  [](unsigned char c) { return std::tolower(static_cast<int>(c)); });

        // Check if the current name contains the given substring, now case-insensitive
        if (currentName.find(searchInput) != string::npos) { // find returns npos if no match is found
            ReservationNode* newNode = new ReservationNode(*current);
            newNode->next = nullptr;
            if (!tempHead) {
                tempHead = tempTail = newNode;
            } else {
                tempTail->next = newNode;
                tempTail = tempTail->next;
            }
            found = true;
        }
    }

    if (found) {
    	tempHead = mergeSort(tempHead, true);
        printSReservations(tempHead);
    } else {
        cout << "No reservation found containing: " << substring << endl;
    }

    // Clean up temporary list
    while (tempHead) {
        ReservationNode* temp = tempHead;
        tempHead = tempHead->next;
        delete temp;
    }
}

void searchByDate() {
    int month = getMonthFromUser("Enter reservation month (MM): ");
    string date = selectDayFromFile(month);

    // Construct a full date string in YYYY-MM-DD format for matching
    string fullDate = to_string(reservationYear) + "-" + date; // reservationYear is 2024 as per your structure

    head = mergeSort(head, true); // Ensure the list is sorted by date for more effective searching

    bool found = false;
    ReservationNode* tempHead = nullptr;
    ReservationNode* tempTail = nullptr;

    for (ReservationNode* current = head; current != nullptr; current = current->next) {
        // Only compare the date part (first 10 characters of the reservationDateTime)
        if (current->reservationDateTime.substr(0, 10) == fullDate) {
            ReservationNode* newNode = new ReservationNode(*current); // Use the copy constructor
            newNode->next = nullptr;
            if (!tempHead) {
                tempHead = tempTail = newNode;
            } else {
                tempTail->next = newNode;
                tempTail = tempTail->next;
            }
            found = true;
        }
    }

    if (found) {
    	tempHead = mergeSortByName(tempHead);
        printSReservations(tempHead);
    } else {
        cout << "No reservations found on the date: " << fullDate << endl;
    }

    // Clean up temporary list
    while (tempHead) {
        ReservationNode* temp = tempHead;
        tempHead = tempHead->next;
		delete temp;}
		}
void viewUnsortedReservations() {
    if (!head) {
        cout << "No reservations to display." << endl;
    } else {
        cout << "Displaying unsorted reservations:" << endl;
        printSReservations(head);
    }
}
    void displayMainMenu() {
	           cout<<"Operation : "<<endl
             << "1. View And Sort All Reservation by Date Ascending" << endl
             << "2. View And Sort All Reservation by Date Descending" << endl
             << "3. View And Sort All Reservation by Name Ascending" << endl
             << "4. Search Reservation by Name and sort by date ascending" << endl
             << "5. Search Reservation by Date and sort by name ascending" << endl
             << "\nIf you want to go back to the main page, please enter 'm'." << endl
    		 << "If you want to exit, please enter 'e'." << endl
             << "Please enter your selection: ";mainMenu();
    }

        double calculateTotal(const ReservationNode* reservation) const {
        double total = 0.0;
        FoodItem* currentItem = reservation->foodOrders;
        while (currentItem) {
            total += currentItem->price * currentItem->quantity;
            currentItem = currentItem->next;
        }
        return total;
    }

        void clearReservations() {
        while (head != nullptr) {
            ReservationNode* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
    }

        void loadReservations() {
        ifstream inFile("reservations.txt");
        if (!inFile) {
            cerr << "Failed to open file for reading." << endl;
            return;
        }
        clearReservations();
        string line;

        while (getline(inFile, line)) {
            stringstream ss(line);
            string part, itemDetails;
            ReservationNode* newNode = new ReservationNode();

            getline(ss, newNode->customerName, ',');
            getline(ss, newNode->reservationDateTime, ',');
            getline(ss, part, ',');
            newNode->people = stoi(part);
            getline(ss, newNode->phone, ';');

            while (getline(ss, itemDetails, ';')) {
                stringstream itemStream(itemDetails);
                string name, price, quantity;
                getline(itemStream, name, ':');
                getline(itemStream, price, ':');
                getline(itemStream, quantity);

                FoodItem* item = new FoodItem{name, "", stod(price), stoi(quantity), nullptr};
                addFoodOrder(newNode, item, item->quantity);
            }

            if (!head) {
                head = tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
        }
        inFile.close();
    }

    int getMonthFromUser(const string& prompt) {
        int month;
        do {
            cout << prompt;
            cin >> month;
            if (month < 6 || month > 12) {
                cout << "Invalid month. Please enter a month between 06 and 12.\n";
            } else {
                break;
            }
        } while (true);
        return month;
    }

    bool validateDayInFile(int month, const string& day) {
        string filename;
        switch (month) {
            case 6: filename = "JUNE.txt"; break;
            case 7: filename = "JULY.txt"; break;
            case 8: filename = "AUGUST.txt"; break;
            case 9: filename = "SEP.txt"; break;
            case 10: filename = "OCT.txt"; break;
            case 11: filename = "NOV.txt"; break;
            case 12: filename = "DEC.txt"; break;
            default: return false;
        }

        ifstream file(filename);
        if (!file) {
            cerr << "Failed to open day file: " << filename << endl;
            return false;
        }

        string line, availableDay;
        while (getline(file, line)) {
            stringstream ss(line);
            while (ss >> availableDay) {
                if (availableDay.length() < 2) {
                    availableDay = "0" + availableDay;
                }
                if (availableDay == day) {
                    return true;
                }
            }
        }
        return false;
    }

    string selectDayFromFile(int month) {
        string filename;
        string monthn;
        switch (month) {
            case 6: filename = "JUNE.txt"; monthn = "JUNE"; break;
            case 7: filename = "JULY.txt"; monthn = "JULY"; break;
            case 8: filename = "AUGUST.txt"; monthn = "AUGUST"; break;
            case 9: filename = "SEP.txt"; monthn = "SEPTEMBER"; break;
            case 10: filename = "OCT.txt"; monthn = "OCTOBER"; break;
            case 11: filename = "NOV.txt"; monthn = "NOVEMBER"; break;
            case 12: filename = "DEC.txt"; monthn = "DECEMBER"; break;
            default: cerr << "Invalid month provided."; return "01";
        }

        ifstream file(filename);
        if (!file) {
            cerr << "Failed to open day file: " << filename << endl;
            return "01";
        }

        cout << endl << "Available days from " << monthn << " : " << endl << endl;
        string line, day;
        while (getline(file, line)) {
            stringstream ss(line);
            while (ss >> day) {
                day = day.length() < 2 ? "0" + day : day;
                cout << day << " ";
            }
            cout << endl;
        }
        file.close();

        string selectedDay;
        bool isValidDay = false;

        do {
            cout << "Select a day (DD): ";
            cin >> selectedDay;
            if (selectedDay.length() < 2) {
                selectedDay = "0" + selectedDay;
            }
            isValidDay = validateDayInFile(month, selectedDay);
            if (!isValidDay) {
                cout << "Invalid day. Please select a day from the list." << endl;
            }
        } while (!isValidDay);

        string formattedMonth = month < 10 ? "0" + to_string(month) : to_string(month);
        return formattedMonth + "-" + selectedDay;
    }

    void printSReservations(ReservationNode* tempHead) const {
    if (!tempHead) {
        cout << "No reservations to display." << endl;
        return;
    }

   
    char viewDetails;
    do {
    cout << "--------------------------------------------------------------------------------------------------------" << endl;
    cout << setw(5) << left << "No."
         << setw(30) << left << "Customer Name"
         << setw(21) << "Date And Time"
         << setw(8) << "People"
         << setw(17) << "Phone Number"
         << setw(20) << "Total Price" << endl;
    cout << "--------------------------------------------------------------------------------------------------------" << endl;
    int index = 1;
    for (ReservationNode* temp = tempHead; temp != nullptr; temp = temp->next, ++index) {
        cout << setw(5) << left << index
             << setw(30) << left << temp->customerName
             << setw(21) << temp->reservationDateTime
             << setw(8) << temp->people
             << setw(17) << temp->phone
             <<"RM "<< setw(20) << fixed << setprecision(2) << calculateTotal(temp) << endl;
    }
    cout << "--------------------------------------------------------------------------------------------------------" << endl;
        
        cout << "Do you want to view the details of any reservation? (y/n): ";
        cin >> viewDetails;
        if (tolower(viewDetails) == 'y') {
            int reservationIndex;
            cout << "Enter the reservation number to view details: ";
            cin >> reservationIndex;
            ReservationNode* reservation = getReservationAtFilteredIndex(tempHead, reservationIndex);
            if (reservation) {
                printXReservation(reservation);
            } else {
                cout << "Invalid reservation number." << endl;
            }
        }
    } while (tolower(viewDetails) == 'y');
}


    void printXReservation(const ReservationNode* reservation) const {
        if (!reservation) {
            cout << "Invalid reservation reference provided." << endl;
            return;
        }
        cout<<"===================================="<<endl;
        cout<<"Reservation Details"<<endl;
        cout<<"===================================="<<endl;
        cout <<endl
            << "Name : " << reservation->customerName << endl
            << "Order Date : " << reservation->reservationDateTime << endl
            << "Total of People : " << reservation->people << " people" << endl
            << "Contact Number : " << reservation->phone << endl
            << "Orders :" << endl;
        int itemIndex = 1;
        FoodItem* currentOrder = reservation->foodOrders;
        double total = 0.0;
        while (currentOrder) {
            cout << "   " << itemIndex++ << ". " << currentOrder->name
                << ", RM " << fixed << setprecision(2) << currentOrder->price
                << " (" << currentOrder->quantity << " qty)" << endl;  // Corrected display
            total += currentOrder->price * currentOrder->quantity;
            currentOrder = currentOrder->next;
        }
        cout << "   Total Price: RM " << fixed << setprecision(2) << total << endl << endl;
    }

    
    ReservationNode* getReservationAtFilteredIndex(ReservationNode* filteredHead, int index) const {
        if (index < 1) {
            return nullptr;
        }
        ReservationNode* temp = filteredHead;
        for (int count = 1; temp != nullptr && count < index; temp = temp->next, ++count);
        return temp;
    }

    public:
    ReservationList() : categoryHead(nullptr), head(nullptr), tail(nullptr) {
        loadFoodItems();
    }

    ~ReservationList() {
        clearReservations();
    }

    void mainMenu() {
        string input;
        bool valid=false;
            string ans;
            cin >> ans;
do{	
		cin.clear();
		if(checkinput(ans))
		{	valid=true;	
		}
		else if(ans=="1")
		{	valid=true;
		system("cls");
		head = mergeSort(head, true);
			cout << "Reservations sorted by date (Ascending):\n";
			printSReservations(head);
			displayMainMenu();
		}else if(ans=="2")
		{	valid=true;
			system("cls");
			head = mergeSort(head, false);
			cout << "Reservations sorted by date (Descending):\n";
			printSReservations(head);
			displayMainMenu();
		}else if(ans=="3")
		{	valid=true;
		system("cls");
		head = mergeSortByName(head);
			cout << "Reservations sorted by customer name (Ascending):\n";
			printSReservations(head);
			displayMainMenu();;
		}else if (ans == "4") {
		  valid=true;
		  cout<<"=================================================================="<<endl;
		cout<<" Search reservation by Name and sort by date ascending"<<endl;
		  cout<<"=================================================================="<<endl;
		  cout << "Enter part of the name to search : "<<endl;
          cin.ignore(numeric_limits<streamsize>::max(), '\n');
          getline(cin, input);
          searchByName(input);
		  displayMainMenu();
		}else if (ans == "5") {
		   valid=true;
		   cout<<"=================================================================="<<endl;
		cout<<" Search Reservation by Date and sort by name ascending"<<endl;
		  cout<<"=================================================================="<<endl;
		   cout << "***Enter the date to search***"<<endl;
           searchByDate();
		   displayMainMenu();
		}else{
		cout<<"Invalid selection.Please reenter your selection : ";
			cin>>ans;
		}
		}while(!valid);
           
        
    }

        void printSortedReservations() const {
        for (ReservationNode* temp = head; temp != nullptr; temp = temp->next) {
            printXReservation(temp);
        }
    }

        ReservationNode* getReservationAtIndex(int index) const {
        if (index < 1 || index > getCount()) {
            return nullptr;
        }
        ReservationNode* temp = head;
        for (int count = 1; temp != nullptr && count < index; temp = temp->next, ++count);
        return temp;
    }

    int getCount() const {
        int count = 0;
        for (ReservationNode* temp = head; temp; temp = temp->next) {
            ++count;
        }
        return count;
    }
    };


bool login;

void mainmenu()
{	foodmenu menu;
	usermanager u("users.txt");
	ReservationList reservations;
string selection;
bool valid=false;

string adminname,password;
 while(!login){
 	string adminname, password;

        while (true) {
            cout << "\n-----------------------------------" << endl;
            cout << "\tAdmin Login " << endl;
            cout << "-----------------------------------" << endl;
            cout << "Please enter your username: ";
            getline(cin, adminname);
            cout << "Please enter your password: ";
            getline(cin, password);
            if (a.login(adminname, password)) {
                cout << "Login successfully!" << endl;
                system("cls");
                login=true;
                break;
            } else {
                cout << "Invalid credentials or account locked! Please try again." << endl;
            }
        }
 }
	cout<<"===================================================================="<<endl;
	cout<<"\tRestaurant Reservation Management System"<<endl;
	cout<<"===================================================================="<<endl;
	cout<<"1.Menu Management"<<endl;
	cout<<"2.View Reservation "<<endl;
	cout<<"3.User Management"<<endl;
	cout<<"4.Edit Profile And Change Password"<<endl;
	if (a.getisSuperAdmin()) {
	cout<<"5.Admin Management"<<endl;	 
		 }
	
	cout << "If you want to exit, please enter 'e'." << endl;
	cout<<"\nPlease enter your selection : "; 
	cin>>selection;
	do{
	cin.clear();
	if(selection=="e"||selection=="E")
	{
		 cout << "\nLogging out..." << endl;
		exit(0);
	}
	else if(selection=="1")
	{menu.displayaction();
	valid=true;
	}else if(selection=="2")
	{system("cls");
	cout << endl
             << "====================================================================" << endl
             << "\tView Reservation" << endl
             << "====================================================================" << endl;
             reservations.loadReservations();
             reservations.viewUnsortedReservations();
	reservations.displayMainMenu();
	valid=true;
	}else if(selection=="3")
	{system("cls");
	u.usermenu();
	valid=true;
	}else if(selection=="4")
	{ system("cls");
	a.editProfile();
	valid=true;
	}else if(selection=="5"&&a.getisSuperAdmin())
	{ system("cls");
	a.adminmenu();
	valid=true;
	}else{
		cout<<"Invalid selection.Please reenter your choice : ";
		cin>>selection;
	}
	
	
	}while(!valid);
}
void clearScreen() {
        // Clear the screen
        #if defined(_WIN32)
            system("cls");
        #else
            system("clear");
        #endif
    }
     

int main()
{	mainmenu();
	
}
