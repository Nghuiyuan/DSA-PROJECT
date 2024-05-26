#include<iostream>
#include <limits>
#include<cstring>
#include<iomanip>
#include <fstream>
#include <cstdlib>
#include <sstream> // 添加此头文件以使用 std::stringstream
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
    Food* head;

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

    
};

class foodmenu{
	FoodList food;
	public: 
	foodmenu()
	{    food.readDataFromFile("fooddata.txt");
	}
	void displayaction()
	{	string ans;
		bool valid=false;
		cout<<"===================================================================="<<endl;
		cout<<"\t\tFood Management"<<endl;
		cout<<"===================================================================="<<endl;
		cout<<setw(3)<<"No.  "<<setw(35)<<left<<"Food"<<setw(15)<<"Category"<<setw(10)<<"Price"<<endl;
		cout<<"--------------------------------------------------------------------"<<endl;
		food.printFoodList();
		cout<<"\nOperation:\n1.Add Food"<<endl;
		cout<<"2.Edit Food"<<endl;
		cout<<"3.Delete Food"<<endl;
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
		{
			valid=true;
		}else{
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

			cout<<"Back to food management page..."<<endl;
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
    int index;
    cout << "Enter the index of the food item you want to edit: ";
    cin >> index;
    while(index < 1 || index > food.getCount()) {
		cin.clear();
        cout << "Invalid index. Please enter a valid index:" ;
        cin >> index;
        
    }

    // Retrieve the food item from the linked list
    Food* temp = food.getFoodAtIndex(index);
	displayfooddetails(temp->category,temp->name,temp->price);
    // Prompt the user for changes
    string newName, newCategory;
    float newPrice;
    bool valid=false;
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

			cout<<"Back to food management page..."<<endl;
			displayaction();
		}else{
			cout<<"Invalid Input.Please reenter your input : ";
		}
	}while(!valid);
    
}

	
};
void mainmenu()
{	foodmenu menu;
string selection;
bool valid=false;
	cout<<"===================================================================="<<endl;
	cout<<"\tRestaurant Reservation Management System"<<endl;
	cout<<"===================================================================="<<endl;
	cout<<"1.Food Management"<<endl;
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
