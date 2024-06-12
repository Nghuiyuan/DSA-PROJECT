#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
#include <iomanip>
#include <limits>

using namespace std;

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

class UserManager;
class ReservationList;
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

class ReservationList {
private:
    CategoryNode* categoryHead;
    ReservationNode* head;
    ReservationNode* tail;
    const int reservationYear = 2024;
    User* u;

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

    void displayMainMenu() {
        cout << endl
             << "====================================================================" << endl
             << "\tRestaurant Reservation Management System" << endl
             << "====================================================================" << endl
             << "1. Add Reservation" << endl
             << "2. Edit Reservation" << endl
             << "3. View Reservation" << endl
             << "4. Exit" << endl
             << "Please enter your selection: ";
    }

    void displayCategories() const {
        int i = 1;
        CategoryNode* currentCategory = categoryHead;
        while (currentCategory) {
            cout << i++ << ". " << currentCategory->category << endl;
            currentCategory = currentCategory->next;
        }
        cout << i << ". Main Menu" << endl;
    }

    void toUpperCase(string& str) {
        for (char& c : str) {
            c = toupper(c);
        }
    }

    void addFoodToReservation(ReservationNode* reservation) {
        int choice;
        do {
            cout << endl;
            cout << "--MENU--" << endl;
            displayCategories();
            cout << "Select a category (or choose " << getCategoryCount() + 1 << " for Main Menu): ";
            cin >> choice;
            if (choice < 1 || choice > getCategoryCount() + 1) {
                cout << "Invalid option, please choose again.\n";
                continue;
            }
            if (choice == getCategoryCount() + 1) {
                break;  // Exit on 'Main Menu'
            }

            CategoryNode* selectedCategory = getCategoryAt(choice - 1);
            if (!selectedCategory) {
                cout << "Invalid selection, returning to categories.\n";
                continue;
            }

            cout << endl << "Menu for " << selectedCategory->category << ":\n";
            int j = 1;
            FoodItem* currentItem = selectedCategory->foodHead;
            while (currentItem) {
                cout << j++ << ". " << currentItem->name << " - RM" << fixed << setprecision(2) << currentItem->price << endl;
                currentItem = currentItem->next;
            }

            cout << endl << "Which item do you want (0 to cancel): ";
            int itemChoice;
            cin >> itemChoice;
            if (itemChoice < 1 || itemChoice > getFoodCount(selectedCategory)) {
                cout << "Invalid selection, returning to categories.\n";
                continue;
            }

            currentItem = getFoodAt(selectedCategory, itemChoice - 1);
            if (!currentItem) {
                cout << "Invalid selection, returning to categories.\n";
                continue;
            }

            cout << endl << "How many quantities of " << currentItem->name << "?: ";
            int quantity;
            cin >> quantity;
            if (quantity > 0) {
                addFoodOrder(reservation, currentItem, quantity);
                cout << quantity << " of " << currentItem->name << " added. Add more or return to Main Menu.\n";
            } else {
                cout << "Invalid quantity, returning to categories.\n";
            }
        } while (true);
    }

    void addFoodOrder(ReservationNode* reservation, FoodItem* item, int quantity) {
        FoodItem* newOrder = new FoodItem(*item);
        newOrder->next = reservation->foodOrders;
        reservation->foodOrders = newOrder;
        newOrder->quantity = quantity;
        newOrder->price = item->price;
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

    void saveReservations() {
        ofstream outFile("reservations.txt");
        if (!outFile) {
            cerr << "Failed to open file for writing." << endl;
            return;
        }
        for (ReservationNode* temp = head; temp != nullptr; temp = temp->next) {
            outFile << temp->customerName << ","
                    << temp->reservationDateTime << ","
                    << temp->people << ","
                    << temp->phone;
            FoodItem* currentOrder = temp->foodOrders;
            while (currentOrder) {
                outFile << ";" << currentOrder->name << ":" << currentOrder->price << ":" << currentOrder->quantity;
                currentOrder = currentOrder->next;
            }
            outFile << endl;
        }
        outFile.close();
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

    string getTimeFromUser(const string& prompt) {
        int choice;
        cout << prompt << endl;
        for (int i = 0; i < 20; ++i) {
            int hour = 10 + i / 2;
            string minute = (i % 2 == 0) ? "00" : "30";
            string am_pm = (hour >= 12) ? " PM" : " AM";
            if (hour > 12) hour -= 12;
            cout << (i + 1) << ". " << setw(2) << setfill('0') << hour << ":" << minute << am_pm << endl;
        }
        cout << "Select a time slot (1-20): ";

        while (!(cin >> choice) || choice < 1 || choice > 20) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid selection. Please enter a valid slot number between 1 and 20: ";
        }

        int fullHour = 10 + (choice - 1) / 2;
        string minute = ((choice - 1) % 2 == 0) ? "00" : "30";
        string am_pm = (fullHour >= 12) ? " PM" : " AM";
        if (fullHour > 12) fullHour -= 12;
        return (fullHour < 10 ? "0" : "") + to_string(fullHour) + ":" + minute + am_pm;
    }

    void editPeople(ReservationNode* reservation) {
        int newPeople;
        bool validInput = false;
        while (!validInput) {
            cout << "Enter new number of people (1-15): ";
            cin >> newPeople;
            if (isValidPeople(newPeople)) {
                reservation->people = newPeople;
                validInput = true;
            } else {
                cout << "Invalid number. Please enter a number from 1 to 15.\n";
            }
        }
    }

    void editDate(ReservationNode* reservation) {
        int month = getMonthFromUser("Enter reservation month (06-12): ");
        string newDate = selectDayFromFile(month);
        if (reservation->reservationDateTime.length() >= 16) {
            string currentTime = reservation->reservationDateTime.substr(11);
            reservation->reservationDateTime = to_string(reservationYear) + "-" + newDate + " " + currentTime;
        } else {
            cout << "Error: Current reservation date-time format is invalid." << endl;
        }
    }

    void editTime(ReservationNode* reservation) {
        string newTime = getTimeFromUser("Choose new reservation time:");

        if (reservation->reservationDateTime.length() >= 16) {
            string currentDate = reservation->reservationDateTime.substr(0, 10);
            reservation->reservationDateTime = currentDate + " " + newTime;
        } else {
            cout << "Error: Current reservation date-time format is invalid." << endl;
        }
    }

    void editPhone(ReservationNode* reservation) {
        bool validInput = false;
        string newPhone;
        while (!validInput) {
            cout << "Enter new phone number: ";
            if (cin.peek() == '\n') {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            getline(cin, newPhone);
            if (isValidPhone(newPhone)) {
                reservation->phone = newPhone;
                validInput = true;
            } else {
                cout << "Invalid phone number. It must be 10 digits long and start with '01'.\n";
            }
        }
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

    void printSingleReservation(const ReservationNode* reservation) const {
        if (!reservation) {
            cout << "Invalid reservation reference provided." << endl;
            return;
        }
        cout << endl << "Updated Reservation Details :" << endl
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
                << " (" << currentOrder->quantity << " qty)" << endl;
            total += currentOrder->price * currentOrder->quantity;
            currentOrder = currentOrder->next;
        }
        cout << "   Total Price: RM " << fixed << setprecision(2) << total << endl << endl;
    }

    void printFinalReservation(const ReservationNode* reservation) const {
        if (!reservation) {
            cout << "Invalid reservation reference provided." << endl;
            return;
        }
        cout << endl << "Final Reservation Details :" << endl
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
                << " (" << currentOrder->quantity << " qty)" << endl;
            total += currentOrder->price * currentOrder->quantity;
            currentOrder = currentOrder->next;
        }
        cout << "   Total Price: RM " << fixed << setprecision(2) << total << endl << endl;
    }

    void editFood(ReservationNode* reservation) {
        cout << endl << "Current food orders:\n";
        int oi = 1;
        FoodItem* currentOrder = reservation->foodOrders;
        while (currentOrder) {
            cout << oi++ << ". " << currentOrder->name << " - Qty: " << currentOrder->quantity
                 << ", Price per item: RM" << fixed << setprecision(2) << currentOrder->price << endl;
            currentOrder = currentOrder->next;
        }

        cout << endl << "Select the food order to edit (0 to cancel): ";
        int foodIndex;
        while (!(cin >> foodIndex) || foodIndex < 0 || foodIndex > getFoodOrderCount(reservation)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (foodIndex == 0) return;
            cout << "Invalid selection. Please enter a valid index or 0 to cancel: ";
        }

        if (foodIndex == 0) return;

        FoodItem* selectedOrder = getFoodOrderAt(reservation, foodIndex - 1);
        if (!selectedOrder) {
            cout << "Invalid selection.\n";
            return;
        }

        cout << endl << "Selected: " << selectedOrder->name << " - Qty: " << selectedOrder->quantity
             << ", Price per item: RM" << fixed << setprecision(2) << selectedOrder->price << endl;
        cout << endl << "Options: \n1. Change Quantity\n2. Replace Item\n3. Remove Item\nEnter your choice: ";
        int choice;
        while (!(cin >> choice) || choice < 1 || choice > 3) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please enter a valid choice (1-3): ";
        }

        switch (choice) {
            case 1: {
                int newQty;
                cout << endl << "Enter new quantity (0 to cancel order): ";
                while (!(cin >> newQty) || newQty < 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (newQty == 0) {
                        cout << "Order edit cancelled.\n";
                        return;
                    }
                    cout << "Invalid quantity. Please enter a valid quantity or 0 to cancel: ";
                }
                if (newQty > 0) {
                    selectedOrder->quantity = newQty;
                } else {
                    removeFoodOrder(reservation, foodIndex - 1);
                    cout << "Order edit cancelled.\n";
                }
                break;
            }
            case 2: {
                cout << endl << "Select a new food item to replace:\n";
                displayCategories();
                int categoryChoice;
                cout << "Select category: ";
                while (!(cin >> categoryChoice) || categoryChoice < 1 || categoryChoice > getCategoryCount()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid category. Please select a valid category: ";
                }

                CategoryNode* selectedCategory = getCategoryAt(categoryChoice - 1);
                if (!selectedCategory) {
                    cout << "Invalid category selection.\n";
                    return;
                }

                cout << endl << "Food:\n";
                int j = 1;
                FoodItem* currentItem = selectedCategory->foodHead;
                while (currentItem) {
                    cout << j++ << ". " << currentItem->name << " - RM" << fixed << setprecision(2) << currentItem->price << endl;
                    currentItem = currentItem->next;
                }

                cout << endl << "Select food item to replace with: ";
                int itemChoice;
                while (!(cin >> itemChoice) || itemChoice < 1 || itemChoice > getFoodCount(selectedCategory)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid item. Please select a valid food item: ";
                }

                currentItem = getFoodAt(selectedCategory, itemChoice - 1);
                if (!currentItem) {
                    cout << "Invalid food item selection.\n";
                    return;
                }

                int newQty;
                cout << endl << "Enter quantity for the new item: ";
                while (!(cin >> newQty) || newQty < 1) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid quantity. Please enter a positive number: ";
                }
                replaceFoodOrder(reservation, foodIndex - 1, currentItem, newQty);
                break;
            }
            case 3:
                removeFoodOrder(reservation, foodIndex - 1);
                cout << "Item removed from the order.\n";
                break;
        }
        saveReservations();
        cout << "Order updated successfully!\n";
    }

    bool isValidPhone(const string& phone) const {
        return (phone.length() == 10 && phone.substr(0, 2) == "01");
    }

    bool isValidPeople(int people) const {
        return (people > 0 && people <= 15);
    }

    bool isValidTime(int hour, int minute) const {
        return (hour >= 10 && hour <= 20 && minute >= 0 && minute < 60);
    }

    int getCategoryCount() const {
        int count = 0;
        CategoryNode* currentCategory = categoryHead;
        while (currentCategory) {
            ++count;
            currentCategory = currentCategory->next;
        }
        return count;
    }

    CategoryNode* getCategoryAt(int index) const {
        int count = 0;
        CategoryNode* currentCategory = categoryHead;
        while (currentCategory && count < index) {
            ++count;
            currentCategory = currentCategory->next;
        }
        return currentCategory;
    }

    int getFoodCount(CategoryNode* category) const {
        int count = 0;
        FoodItem* currentItem = category->foodHead;
        while (currentItem) {
            ++count;
            currentItem = currentItem->next;
        }
        return count;
    }

    FoodItem* getFoodAt(CategoryNode* category, int index) const {
        int count = 0;
        FoodItem* currentItem = category->foodHead;
        while (currentItem && count < index) {
            ++count;
            currentItem = currentItem->next;
        }
        return currentItem;
    }

    int getFoodOrderCount(ReservationNode* reservation) const {
        int count = 0;
        FoodItem* currentOrder = reservation->foodOrders;
        while (currentOrder) {
            ++count;
            currentOrder = currentOrder->next;
        }
        return count;
    }

    FoodItem* getFoodOrderAt(ReservationNode* reservation, int index) const {
        int count = 0;
        FoodItem* currentOrder = reservation->foodOrders;
        while (currentOrder && count < index) {
            ++count;
            currentOrder = currentOrder->next;
        }
        return currentOrder;
    }

    void removeFoodOrder(ReservationNode* reservation, int index) {
        FoodItem* currentOrder = reservation->foodOrders;
        FoodItem* prevOrder = nullptr;
        int count = 0;
        while (currentOrder && count < index) {
            ++count;
            prevOrder = currentOrder;
            currentOrder = currentOrder->next;
        }
        if (currentOrder) {
            if (prevOrder) {
                prevOrder->next = currentOrder->next;
            } else {
                reservation->foodOrders = currentOrder->next;
            }
            delete currentOrder;
        }
    }

    void replaceFoodOrder(ReservationNode* reservation, int index, FoodItem* newItem, int newQty) {
        FoodItem* currentOrder = reservation->foodOrders;
        int count = 0;
        while (currentOrder && count < index) {
            ++count;
            currentOrder = currentOrder->next;
        }
        if (currentOrder) {
            currentOrder->name = newItem->name;
            currentOrder->category = newItem->category;
            currentOrder->price = newItem->price;
            currentOrder->quantity = newQty;  // Store the correct quantity
        }
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
        loadReservations();
        bool running = true;
        while (running) {
            displayMainMenu();
            int selection;
            cin >> selection;

            switch (selection) {
                case 1:
                    addReservation();
                    break;
                case 2:
                    printReservations();
                    editReservation();
                    break;
                case 3:
                    cout << "--History of Reservations--" << endl;
                    printReservations();
                    break;
                case 4:
                    cout << "Exiting...\n";
                    running = false;
                    break;
                default:
                    cout << "Invalid selection. Please re-enter your choice.\n";
                    break;
            }
        }
    }

    void printSortedReservations() const {
        for (ReservationNode* temp = head; temp != nullptr; temp = temp->next) {
            printXReservation(temp);
        }
    }

    void addReservation() {
        string time, date;
        int people, month;

        month = getMonthFromUser("Enter reservation month (06-12): ");
        date = selectDayFromFile(month);

        time = getTimeFromUser("Enter reservation time (HH:MM): ");
        string reservationDateTime = to_string(reservationYear) + "-" + date + " " + time;

        cout << "Enter number of people (1-15): ";
        while (!(cin >> people) || !isValidPeople(people)) {
            cout << "Invalid number. Please enter a number from 1 to 15: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear input buffer


        ReservationNode* newNode = new ReservationNode();
        newNode->customerName = u->getUsername();
        newNode->phone = u->getContactNumber();
        newNode->reservationDateTime = reservationDateTime;
        newNode->people = people; 

        cout << endl << "Do you want to order food? (yes/no): ";
        string response;
        cin >> response;
        toUpperCase(response);
        if (response == "YES") {
            addFoodToReservation(newNode);
            cout << endl << "Total Cost: RM " << fixed << setprecision(2) << calculateTotal(newNode) << endl;
        }

        cout << "Please confirm your reservation details: " << endl;
        printFinalReservation(newNode);
        cout << "Do you want to save this reservation? (yes/no): ";
        cin >> response;
        toUpperCase(response);
        if (response == "YES") {
            if (!tail) {
                head = tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
            saveReservations();
            cout << endl << "Reservation added successfully!!!" << endl;
        } else {
            delete newNode;
            cout << "Reservation cancelled, returning to main menu." << endl;
        }
    }

    void editReservation() {
        cout << endl << "Enter the index of the reservation you want to edit: ";
        int index;
        cin >> index;
        ReservationNode* reservation = getReservationAtIndex(index);
        if (!reservation) {
            cout << "Invalid index.\n";
            return;
        }

        bool continueEditing = true;
        while (continueEditing) {
            cout << endl << "Select what to edit:\n"
                 << "1. Number of People\n"
                 << "2. Date\n"
                 << "3. Time\n"
                 << "4. Phone Number\n"
                 << "5. Food Orders\n"
                 << "6. Finish Editing (return to main menu)\n"
                 << "Enter your choice: ";
            int choice;
            cin >> choice;

            switch (choice) {
                case 1:
                    editPeople(reservation);
                    break;
                case 2:
                    editDate(reservation);
                    break;
                case 3:
                    editTime(reservation);
                    break;
                case 4:
                    editPhone(reservation);
                    break;
                case 5:
                    editFood(reservation);
                    break;
                case 6: {
                    continueEditing = false;
                    printFinalReservation(reservation);
                    break;
                }
                default:
                    cout << "Invalid choice. Please try again.\n";
                    continue;
            }

            if (choice >= 1 && choice <= 5) {
                saveReservations();
                printSingleReservation(reservation);
            }
        }
    }

    void printReservations() const {
        int index = 1;
        for (ReservationNode* temp = head; temp != nullptr; temp = temp->next, ++index) {
            cout << endl << index << ". Name: " << temp->customerName << endl
                 << "   Order Date: " << temp->reservationDateTime << endl
                 << "   Total of People: " << temp->people << " people" << endl
                 << "   Contact Number: " << temp->phone << endl
                 << "   Orders: " << endl;
            int itemIndex = 1;
            FoodItem* currentOrder = temp->foodOrders;
            while (currentOrder) {
                cout << "   " << itemIndex++ << ". " << currentOrder->name
                     << ", RM " << fixed << setprecision(2) << currentOrder->price
                     << " (" << currentOrder->quantity << " qty)" << endl;  // Corrected display
                currentOrder = currentOrder->next;
            }
            double total = calculateTotal(temp);
            cout << "   Total Price: RM " << fixed << setprecision(2) << total << endl;
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
            cout << "1. Profile\n2. Reservation\n3. Logout\n";
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
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

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
                    r.mainMenu();
                } else if (choice == 3) {
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
    ReservationList r;
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
