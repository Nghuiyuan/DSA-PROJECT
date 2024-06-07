#include<iostream>
#include <limits>
#include<cstring>
#include<iomanip>
#include <fstream>
#include <cstdlib>
#include <sstream> 
#include <cctype> 
#include <algorithm> 


using namespace std;
void mainmenu();
struct Food {
    string name;
    string category;
    float price;
    Food* next;
};

bool checkinput(string input) {
    if(input=="m"||input=="M") {
    	system("cls");
        mainmenu();
        return true;
    } else if(input=="e"||input=="E") {
        cout<<"\nEnd Program....";
        exit(0);
    }
    return false;    
}

bool isValidPrice(double price) {
    return price > 0; 
}

class FoodList {
private:
    Food* head;// Pointer to the head of the linked list
        // Merge two sorted lists based on ascending order
 	 Food* merge(Food* left, Food* right, bool ascending) {
        if (!left) return right;
        if (!right) return left;
        // Decide the merge order based on the ascending parameter
        if ((ascending && left->name <= right->name) || (!ascending && left->name >= right->name)) {
            left->next = merge(left->next, right, ascending);
            return left;
        } else {
            right->next = merge(left, right->next, ascending);
            return right;
        }
    }
    // Find the middle of the list to split it
    Food* getMiddle(Food* head) {
        if (head == nullptr) return head;
        Food* slow = head;
        Food* fast = head->next;

        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }

        return slow;
    }
    // Perform merge sort on the list
    Food* mergeSort(Food* head, bool ascending) {
        if (!head || !head->next) return head;

        Food* middle = getMiddle(head);
        Food* nextToMiddle = middle->next;

        middle->next = nullptr;

        Food* left = mergeSort(head, ascending);
        Food* right = mergeSort(nextToMiddle, ascending);

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

    void addFood(const string& name, const string& category, float price) {
        Food* newNode = new Food;
        newNode->name = name;
        newNode->category = category;
        newNode->price = price;
        newNode->next = nullptr;

        if (head == nullptr) {
            head = newNode;
        } else {
            Food* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
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
        outFile << temp->name << "," << temp->category << "," <<fixed<<setprecision(2)<< temp->price << endl;
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
            cout <<setw(4)<<count<<setw(35) << temp->name << setw(15) << temp->category << setw(2)<<"RM"<<setw(8)<<fixed<<setprecision(2)<< temp->price << endl;
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
    transform(uppercaseTargetCategory.begin(), uppercaseTargetCategory.end(), uppercaseTargetCategory.begin(), ::toupper); // Convert target category to uppercase

    string word;
    stringstream ss(uppercaseTargetCategory);

    // Process target category word by word
    while (ss >> word) {
        // Traverse the food linked list to find foods matching the specified category
        temp = head;
        while (temp != nullptr) {
            string uppercaseCategory = temp->category;
            transform(uppercaseCategory.begin(), uppercaseCategory.end(), uppercaseCategory.begin(), ::toupper); // Convert current food's category to uppercase

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

    Food* sorted = nullptr;
    Food* current = head;

    while (current != nullptr) {
        Food* next = current->next;

        if (ascending && (sorted == nullptr || current->price <= sorted->price) ||
            !ascending && (sorted == nullptr || current->price >= sorted->price)) {
            current->next = sorted;
            sorted = current;
        } else {
            Food* temp = sorted;
            while (temp->next != nullptr && ((ascending && temp->next->price < current->price) ||
                                             (!ascending && temp->next->price > current->price))) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }

        current = next;
    }

    head = sorted;
}

  void sortByPriceAscending() {
    insertionSort(head, true);
}

void sortByPriceDescending() {
    insertionSort(head, false);
}
  
};

class foodmenu{
	FoodList food;
	public: 
	foodmenu()
	{    food.readDataFromFile("fooddata.txt");
	}
	
	void displayaction()
	{	string ans,fname,foodname;
		bool valid=false;
		cout<<"===================================================================="<<endl;
		cout<<"\t\tMenu Management"<<endl;
		cout<<"===================================================================="<<endl;
		cout<<setw(3)<<"No.  "<<setw(35)<<left<<"Food"<<setw(15)<<"Category"<<setw(10)<<"Price"<<endl;
		cout<<"--------------------------------------------------------------------"<<endl;
		food.printFoodList();
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
		    food.searchByCategory(category);
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
		    food.searchByname(foodname);
		    displayaction();
		}
		else if (ans == "6") {
                valid = true;
                food.sortByNameAZ();
                system("cls");
                cout << "Food sorted by A-Z :\n";
                displayaction();
            } else if (ans == "7") {
                valid = true;
                food.sortByNameZA();
                system("cls");
                cout << "Food sorted by Z-A :\n";
                displayaction();
            }else if (ans == "8") {
    valid = true;
    food.sortByPriceAscending();
    system("cls");
    cout << "Food sorted by price in ascending order:\n";
    displayaction();
} else if (ans == "9") {
    valid = true;
    food.sortByPriceDescending();
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
		double foodprice;
		bool valid=false;
		
		cout<<"**************************************"<<endl;
		cout<<"	Add Food into "<<category<<endl;
		cout<<"**************************************"<<endl;
		cout<<"Please enter food name  : ";fflush(stdin);
		getline(cin,foodname);
		cout<<"Please enter food price : RM";
		while (!(cin >> foodprice) || !isValidPrice(foodprice)) {
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid food price : RM";
        } else {
            cout << "Invalid price. Please enter a valid positive number: ";
        }
    }
    displayfooddetails(category,foodname,foodprice);
    cout<<"Please Check the new food details\nAre you sure to add the food into "<<category<<"?[Y/N]:";
    do{
    	cin>>confirm;
    	cin.clear();
    	if(confirm=="Y"||confirm=="y")
    	{
    		food.addFood(foodname,category,foodprice);
    		food.saveDataToFile("fooddata.txt");
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
    cout<<"===================================================================="<<endl;
	cout<<"	Edit Food  "<<endl;
	cout<<"===================================================================="<<endl;
	cout<<setw(3)<<"No.  "<<setw(35)<<left<<"Food"<<setw(15)<<"Category"<<setw(10)<<"Price"<<endl;
	cout<<"--------------------------------------------------------------------"<<endl;
	food.printFoodList();
	cout << "\nIf you want to go back to the previous page, please enter 'b'." << endl;
	cout << "If you want to go back to the main page, please enter 'm'." << endl;
	cout << "If you want to exit, please enter 'e'." << endl;
    string input;
    bool valid=false;
    bool inputvalid=false;
    int index;
    cout << "Enter the index of the food item you want to edit: ";
    cin >> input;   
do {
if(input=="B"||input=="b"){
    	system("cls");
    	displayaction();
	}
    if (!checkinput(input)) {
        if (isdigit(input[0])) { 
            index = stoi(input);
            if (index < 1 || index > food.getCount()) {
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
    Food* temp = food.getFoodAtIndex(index);
	displayfooddetails(temp->category,temp->name,temp->price);
    // Prompt the user for changes
    string newName, newCategory;
    float newPrice;
    string category,ans,confirm;
    cout << "\nEnter the new name: ";
	fflush(stdin);
    getline(cin,newName);
    cout<<"\nCategory"<<endl;
	cout<<"1.Main Course"<<endl;
	cout<<"2.Side Dishes  "<<endl;
	cout<<"3.Beverages "<<endl;
    cout << "Choose the new category: ";
    cin>>ans;
    do{	cin.clear();
		if(ans=="1")
		{	valid=true;
			newCategory="Main Course";
		}else if(ans=="2")
		{	valid=true;
			newCategory="Side Dishes";
		}
		else if(ans=="3")
		{	valid=true;
			newCategory="Beverages";
		}else{
			cout<<"Invalid selection.Please reenter your selection : ";
			cin>>ans;}
		}while(!valid);
    cout << "\nEnter the new price: RM";
    while (!(cin >> newPrice) || !isValidPrice(newPrice)) {
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid food price : RM";
        } else {
            cout << "Invalid price. Please enter a valid positive number: ";
        }
    }
	displayfooddetails(newCategory,newName,newPrice);
	cout<<"Please Check the new food details\nAre you sure to edit the food ? [Y/N]:";
    do{
    	cin>>confirm;
    	cin.clear();
    	if(confirm=="Y"||confirm=="y")
    	{
    		// Update the food item
		    temp->name = newName;
		    temp->category = newCategory;
		    temp->price = newPrice;
		
		    // Save the updated data to the file
		    food.saveDataToFile("fooddata.txt");
			system("cls");
		    cout << "\nFood item edited successfully!\n" << endl;
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
void deletefood() {
    system("cls");
    bool inputvalid=false;
    int index=0;
    cout << "====================================================================" << endl;
    cout << "\t\tDelete Food" << endl;
    cout << "====================================================================" << endl;
    cout << setw(3) << "No.  " << setw(35) << left << "Food" << setw(15) << "Category" << setw(10) << "Price" << endl;
    cout << "--------------------------------------------------------------------" << endl;
    food.printFoodList();
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
	            if (index < 1 || index > food.getCount()) {
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
 	Food* temp = food.getFoodAtIndex(index);
	displayfooddetails(temp->category,temp->name,temp->price);
	cout<<"Are you sure you want to delete this food ? [Y/N] : ";
	do{
    	cin>>confirm;
    	cin.clear();
    	if(confirm=="Y"||confirm=="y")
    	{
		   nameToDelete=temp->name;
		 // Delete the food item
	    food.deleteFood(nameToDelete);
	    food.saveDataToFile("fooddata.txt");
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

void mainmenu()
{	foodmenu menu;
string selection;
bool valid=false;
	cout<<"===================================================================="<<endl;
	cout<<"\tRestaurant Reservation Management System"<<endl;
	cout<<"===================================================================="<<endl;
	cout<<"1.Menu Management"<<endl;
	cout << "If you want to exit, please enter 'e'." << endl;
	cout<<"\nPlease enter your selection : "; 
	cin>>selection;
	do{
	cin.clear();
	if(selection=="e"||selection=="E")
	{
		cout<<"\nEnd Program....."<<endl;
		exit(0);
	}
	else if(selection=="1")
	{menu.displayaction();
	valid=true;
	}else{
		cout<<"Invalid selection.Please reenter your choice : ";
		cin>>selection;
	}
	
	
	}while(!valid);
}

int main()
{	mainmenu();
	
}
