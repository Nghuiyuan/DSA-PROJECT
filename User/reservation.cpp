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
    FoodItem* next = nullptr; // For linked list of categorized food items
};

struct FoodCategory {
    string category;
    FoodItem* itemsHead = nullptr; // Head of the linked list of items in this category
    FoodCategory* next = nullptr; // For linked list of categories
};

struct FoodOrder {
    FoodItem* item;
    int quantity;
    FoodOrder* next = nullptr; // For linked list of food orders
};

struct ReservationNode {
    string customerName;
    string reservationDateTime;
    int people;
    string phone;
    FoodOrder* foodOrders = nullptr; // Linked list of food orders
    ReservationNode* next = nullptr;
};

class ReservationList {
private:
    FoodItem* foodItemsHead = nullptr; // Head of the linked list of all food items
    FoodCategory* categoriesHead = nullptr; // Head of the linked list of food categories
    ReservationNode* head = nullptr;
    ReservationNode* tail = nullptr;
    const int reservationYear = 2024;

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
            FoodItem* item = new FoodItem;
            if (getline(ss, item->name, ',') && getline(ss, item->category, ',') && (ss >> item->price)) {
                item->next = foodItemsHead;
                foodItemsHead = item;

                FoodCategory* category = findOrCreateCategory(item->category);
                item->next = category->itemsHead;
                category->itemsHead = item;
            }
        }
        inFile.close();
    }

    FoodCategory* findOrCreateCategory(const string& category) {
        FoodCategory* current = categoriesHead;
        while (current) {
            if (current->category == category) {
                return current;
            }
            current = current->next;
        }
        FoodCategory* newCategory = new FoodCategory{category};
        newCategory->next = categoriesHead;
        categoriesHead = newCategory;
        return newCategory;
    }

    void searchByName(const string& searchName) {
        head = mergeSortByName(head);

        bool found = false;
        for (ReservationNode* current = head; current != nullptr; current = current->next) {
            if (current->customerName.find(searchName) != string::npos) {
                printSingleReservation(current);
                found = true;
            }
        }
        if (!found) {
            cout << "No reservation found under the name containing: " << searchName << endl;
        }
    }

    void searchByDate(const string& searchDate) {
        head = mergeSort(head, true);

        bool found = false;
        for (ReservationNode* current = head; current != nullptr; current = current->next) {
            if (current->reservationDateTime.substr(0, 10) == searchDate) {
                printSingleReservation(current);
                found = true;
            }
        }
        if (!found) {
            cout << "No reservations found on the date: " << searchDate << endl;
        }
    }

    void displayMainMenu() {
        cout << endl
             << "====================================================================" << endl
             << "\tRestaurant Reservation Management System" << endl
             << "====================================================================" << endl
             << "1. Add Reservation" << endl
             << "2. Edit Reservation" << endl
             << "3. View Reservation" << endl
             << "4. Search Reservation by Name" << endl
             << "5. Search Reservation by Date" << endl
             << "6. Sort by Date Ascending" << endl
             << "7. Sort by Date Descending" << endl
             << "8. Sort by Name Ascending" << endl
             << "9. Exit" << endl
             << "Please enter your selection: ";
    }

    void displayCategories() const {
        int i = 1;
        for (FoodCategory* category = categoriesHead; category; category = category->next) {
            cout << i++ << ". " << category->category << endl;
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
                break;
            }

            FoodCategory* category = getCategoryAtIndex(choice - 1);

            cout << endl << "Menu for " << category->category << ":\n";
            int j = 1;
            for (FoodItem* item = category->itemsHead; item; item = item->next) {
                cout << j++ << ". " << item->name << " - RM" << fixed << setprecision(2) << item->price << endl;
            }

            cout << endl << "Which item do you want (0 to cancel): ";
            int itemChoice;
            cin >> itemChoice;
            if (itemChoice < 1 || itemChoice > getItemCount(category)) {
                cout << "Invalid selection, returning to categories.\n";
                continue;
            }

            FoodItem* selectedItem = getItemAtIndex(category, itemChoice - 1);
            cout << endl << "How many quantities of " << selectedItem->name << "?: ";
            int quantity;
            cin >> quantity;
            if (quantity > 0) {
                FoodOrder* order = new FoodOrder{selectedItem, quantity};
                order->next = reservation->foodOrders;
                reservation->foodOrders = order;
                cout << quantity << " of " << selectedItem->name << " added. Add more or return to Main Menu.\n";
            } else {
                cout << "Invalid quantity, returning to categories.\n";
            }
        } while (true);
    }

    double calculateTotal(const ReservationNode* reservation) const {
        double total = 0.0;
        for (FoodOrder* order = reservation->foodOrders; order; order = order->next) {
            total += order->item->price * order->quantity;
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
            cout << "Processing line: " << line << endl;
            stringstream ss(line);
            string customerData, itemDetails;
            getline(ss, customerData, ';');
            stringstream customerStream(customerData);
            string part;
            ReservationNode* newNode = new ReservationNode();
            newNode->foodOrders = nullptr;

            getline(customerStream, newNode->customerName, ',');
            getline(customerStream, newNode->reservationDateTime, ',');
            getline(customerStream, part, ',');
            newNode->people = stoi(part);
            getline(customerStream, newNode->phone, ',');

            FoodOrder* lastOrder = nullptr;
            while (getline(ss, itemDetails, ';')) {
                if (itemDetails.empty()) continue;
                cout << "Item details: " << itemDetails << endl;
                stringstream itemStream(itemDetails);
                string name, price, quantity;
                getline(itemStream, name, ':');
                getline(itemStream, price, ':');
                getline(itemStream, quantity);

                FoodItem* item = findFoodItemByName(name);
                if (item) {
                    FoodOrder* order = new FoodOrder{item, stoi(quantity)};
                    if (!newNode->foodOrders) {
                        newNode->foodOrders = order;
                    } else {
                        lastOrder->next = order;
                    }
                    lastOrder = order;
                    cout << "Added order: " << name << ", Quantity: " << quantity << endl;
                } else {
                    cout << "Item not found: " << name << endl;
                }
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

    FoodItem* findFoodItemByName(const string& name) {
        for (FoodItem* item = foodItemsHead; item; item = item->next) {
            if (item->name == name) {
                return item;
            }
        }
        return nullptr;
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
            for (FoodOrder* order = temp->foodOrders; order; order = order->next) {
                outFile << ";" << order->item->name << ":" << order->item->price << ":" << order->quantity;
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

    string selectDayFromFile(int month) {
        string filename;
        string monthn;
        switch (month) {
            case 6:
                filename = "JUNE.txt";
                monthn = "JUNE";
                break;
            case 7:
                filename = "JULY.txt";
                monthn = "JULY";
                break;
            case 8:
                filename = "AUGUST.txt";
                monthn = "AUGUST";
                break;
            case 9:
                filename = "SEP.txt";
                monthn = "SEPTEMBER";
                break;
            case 10:
                filename = "OCT.txt";
                monthn = "OCTOBER";
                break;
            case 11:
                filename = "NOV.txt";
                monthn = "NOVEMBER";
                break;
            case 12:
                filename = "DEC.txt";
                monthn = "DECEMBER";
                break;
            default:
                cerr << "Invalid month provided.";
                return "01";
        }

        ifstream file(filename);
        if (!file) {
            cerr << "Failed to open day file: " << filename << endl;
            return "01";
        }

        cout << endl << "Available days from " << monthn << " : " << endl;
        cout << endl;
        string availableDays[31];
        int count = 0;
        string line, day;
        while (getline(file, line)) {
            stringstream ss(line);
            while (ss >> day) {
                day = day.length() < 2 ? "0" + day : day; // Format each day as two-digit string if necessary
                availableDays[count++] = day;
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
                selectedDay = "0" + selectedDay; // Ensure two-digit format for comparison
            }
            for (int i = 0; i < count; ++i) {
                if (availableDays[i] == selectedDay) {
                    isValidDay = true;
                    break;
                }
            }
            if (!isValidDay) {
                cout << "Invalid day selected. Please choose a day listed in the file.\n";
            }
        } while (!isValidDay);

        string formattedMonth = month < 10 ? "0" + to_string(month) : to_string(month); // Ensure the month is two digits
        return formattedMonth + "-" + selectedDay;
    }

    string getTimeFromUser(const string& prompt) {
        int choice;
        cout << prompt << endl;
        // Display time slots in a consistent format
        for (int i = 0; i < 20; ++i) {
            int hour = 10 + i / 2;
            string minute = (i % 2 == 0) ? "00" : "30";
            string am_pm = (hour >= 12) ? " PM" : " AM";
            if (hour > 12) hour -= 12;
            cout << (i + 1) << ". " << setw(2) << setfill('0') << hour << ":" << minute << am_pm << endl;
        }
        cout << "Select a time slot (1-20): ";

        while (!(cin >> choice) || choice < 1 || choice > 20) {
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard incorrect input
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
            // Extract HH:MM AM/PM from the current reservation datetime
            string currentTime = reservation->reservationDateTime.substr(11); // Get everything after the date part (includes time and AM/PM)

            // Construct the new datetime string with the new date and the extracted time
            reservation->reservationDateTime = to_string(reservationYear) + "-" + newDate + " " + currentTime;
        } else {
            cout << "Error: Current reservation date-time format is invalid." << endl;
        }
    }

    void editTime(ReservationNode* reservation) {
        // Call the same function used in adding a reservation to get the new time
        string newTime = getTimeFromUser("Choose new reservation time:");

        // Assume the reservation date-time string is formatted correctly as "YYYY-MM-DD HH:MM"
        if (reservation->reservationDateTime.length() >= 16) {
            // Extract just the date part (first 10 characters represent the date "YYYY-MM-DD")
            string currentDate = reservation->reservationDateTime.substr(0, 10);

            // Combine the extracted date with the new time to update the reservation
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
            // Ignore input buffer only if it's necessary. Check if there is something to ignore.
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

void printSReservation(const ReservationNode* head) const {
    if (!head) {
        cout << "No reservations found.\n";
        return;
    }

    cout << "Available Reservations:" << endl;
    cout << "----------------------------------------------------------------------------------------------------" << endl;
    cout << setfill(' ') << setw(5) << left << "Index"
         << setw(30) << "Customer Name"
         << setw(20) << "Date and Time"
         << setw(8) << "People"
         << setw(15) << "Phone Number"
         << setw(15) << left << "Total Price" << endl;
    cout << "----------------------------------------------------------------------------------------------------" << endl;

    int index = 1;
    const ReservationNode* current = head;
    while (current) {
        double totalPrice = calculateTotal(current);  // Calculate the total price for each reservation
        cout << setfill(' ') << setw(5) << left << index++
             << setw(30) << current->customerName
             << setw(20) << current->reservationDateTime
             << setw(8) << current->people
             << setw(15) << current->phone
             << "RM " << setw(14) << right << fixed << setprecision(2) << totalPrice << endl;

        current = current->next;
    }
    selectAndPrintReservationDetails(head);
}



void viewReservationDetails(const ReservationNode* reservation) const {
    int itemIndex = 1;
    double total = 0.0;
    cout << "Order Details for " << reservation->customerName << ":" << endl;
        for (FoodOrder* order = reservation->foodOrders; order; order = order->next) {
            cout << "   " << itemIndex++ << ". " << order->item->name
                << ", RM " << fixed << setprecision(2) << order->item->price
                << " (" << order->quantity << " qty)" << endl;
            total += order->item->price * order->quantity;
        }
    cout << "   Total Price: RM " << fixed << setprecision(2) << total << endl << endl;
}

void selectAndPrintReservationDetails(const ReservationNode* head) const {

    cout << "Enter the index of the reservation you want to view details for: ";
    int choice;
    cin >> choice;

    const ReservationNode* current = head;

    for (int i = 1; i < choice && current != nullptr; i++) {
        current = current->next;
    }

    if (current == nullptr) {
        cout << "Invalid index." << endl;
    } else {
        viewReservationDetails(current);
    }
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
        double total = 0.0;
        for (FoodOrder* order = reservation->foodOrders; order; order = order->next) {
            cout << "   " << itemIndex++ << ". " << order->item->name
                << ", RM " << fixed << setprecision(2) << order->item->price
                << " (" << order->quantity << " qty)" << endl;
            total += order->item->price * order->quantity;
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
        double total = 0.0;
        for (FoodOrder* order = reservation->foodOrders; order; order = order->next) {
            cout << "   " << itemIndex++ << ". " << order->item->name
                << ", RM " << fixed << setprecision(2) << order->item->price
                << " (" << order->quantity << " qty)" << endl;
            total += order->item->price * order->quantity;
        }
        cout << "   Total Price: RM " << fixed << setprecision(2) << total << endl << endl;
    }

    void editFood(ReservationNode* reservation) {
        cout << endl << "Current food orders:\n";
        int oi = 1;
        for (FoodOrder* order = reservation->foodOrders; order; order = order->next) {
            cout << oi++ << ". " << order->item->name << " - Qty: " << order->quantity << ", Price per item: RM" << fixed << setprecision(2) << order->item->price << endl;
        }

        cout << endl << "Select the food order to edit (0 to cancel): ";
        int foodIndex;
        while (!(cin >> foodIndex) || foodIndex < 0 || foodIndex > getOrderCount(reservation->foodOrders)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (foodIndex == 0) return;
            cout << "Invalid selection. Please enter a valid index or 0 to cancel: ";
        }

        if (foodIndex == 0) return;

        FoodOrder* selectedOrder = getOrderAtIndex(reservation->foodOrders, foodIndex - 1);
        cout << endl << "Selected: " << selectedOrder->item->name << " - Qty: " << selectedOrder->quantity << ", Price per item: RM" << fixed << setprecision(2) << selectedOrder->item->price << endl;
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
                    removeOrderFromList(&reservation->foodOrders, selectedOrder);
                    delete selectedOrder;
                    cout << "Order removed.\n";
                }
                break;
            }
            case 2: {
                cout << endl << "Select a new food item to replace: ";
                displayCategories();
                int categoryChoice;
                cout << "Select category: ";
                while (!(cin >> categoryChoice) || categoryChoice < 1 || categoryChoice > getCategoryCount()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid category. Please select a valid category: ";
                }

                FoodCategory* category = getCategoryAtIndex(categoryChoice - 1);
                int j = 1, itemChoice;
                cout << "Food : " << endl;
                for (FoodItem* item = category->itemsHead; item; item = item->next) {
                    cout << j++ << ". " << item->name << " - RM" << fixed << setprecision(2) << item->price << endl;
                }
                cout << endl << "Select food item to replace with: ";
                while (!(cin >> itemChoice) || itemChoice < 1 || itemChoice > getItemCount(category)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid item. Please select a valid food item: ";
                }
                FoodItem* newItem = getItemAtIndex(category, itemChoice - 1);
                int newQty;
                cout << endl << "Enter quantity for the new item: ";
                while (!(cin >> newQty) || newQty < 1) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid quantity. Please enter a positive number: ";
                }
                selectedOrder->item = newItem;
                selectedOrder->quantity = newQty;
                break;
            }
            case 3:
                removeOrderFromList(&reservation->foodOrders, selectedOrder);
                delete selectedOrder;
                cout << "Item removed from the order.\n";
                break;
        }
        saveReservations();
        cout << "Order edited successfully!\n";
    }

    void removeOrderFromList(FoodOrder** head, FoodOrder* target) {
        if (*head == target) {
            *head = (*head)->next;
            return;
        }
        FoodOrder* current = *head;
        while (current && current->next != target) {
            current = current->next;
        }
        if (current) {
            current->next = target->next;
        }
    }

    bool isValidPhone(const string& phone) const {
        return (phone.length() == 10 && phone.substr(0, 2) == "01");
    }

    bool isValidPeople(int people) const {
        return (people > 0 && people <= 15);
    }

    int getCategoryCount() const {
        int count = 0;
        for (FoodCategory* category = categoriesHead; category; category = category->next) {
            ++count;
        }
        return count;
    }

    FoodCategory* getCategoryAtIndex(int index) const {
        FoodCategory* category = categoriesHead;
        for (int i = 0; i < index; ++i) {
            category = category->next;
        }
        return category;
    }

    int getItemCount(FoodCategory* category) const {
        int count = 0;
        for (FoodItem* item = category->itemsHead; item; item = item->next) {
            ++count;
        }
        return count;
    }

    FoodItem* getItemAtIndex(FoodCategory* category, int index) const {
        FoodItem* item = category->itemsHead;
        for (int i = 0; i < index; ++i) {
            item = item->next;
        }
        return item;
    }

    int getOrderCount(FoodOrder* orders) const {
        int count = 0;
        for (FoodOrder* order = orders; order; order = order->next) {
            ++count;
        }
        return count;
    }

    FoodOrder* getOrderAtIndex(FoodOrder* orders, int index) const {
        FoodOrder* order = orders;
        for (int i = 0; i < index; ++i) {
            order = order->next;
        }
        return order;
    }

public:
    ReservationList() {
        loadFoodItems();
    }
    ~ReservationList() {
        clearReservations();
    }

    void mainMenu() {
        string input;        
        loadReservations(); // Load reservations at the start
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
                    cout << "Enter part of the name to search for: ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the remaining newline character
                    getline(cin, input);
                    searchByName(input);
                    break;

                case 5: // Search by date
                    cout << "Enter the date to search for (YYYY-MM-DD): ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the remaining newline character
                    getline(cin, input);
                    searchByDate(input);
                    break;
                case 6:
                    head = mergeSort(head, true); // Sort by date ascending
                    cout << "Reservations sorted by date (Ascending).\n";
                    printReservations();
                    break;
                case 7:
                    head = mergeSort(head, false); // Sort by date descending
                    cout << "Reservations sorted by date (Descending).\n";
                    printReservations();
                    break;
                case 8:
                    head = mergeSortByName(head); // Sort by name ascending
                    cout << "Reservations sorted by name (Ascending).\n";
                    printReservations();
                case 9:
                    cout << "Exiting...\n";
                    running = false;
                    break;
                default:
                    cout << "Invalid selection. Please re-enter your choice.\n";
                    break;
            }
        }
    }

    void addReservation() {
        string customerName, phone, time, date;
        int people, month;

        cout << endl << "Enter customer name: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, customerName);

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

        cout << "Enter the customer phone number (must start with '01' and be 10 digits long): ";
        cin >> phone;
        while (!isValidPhone(phone)) {
            cout << "Invalid phone number. Please enter a valid phone number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> phone;
        }

        ReservationNode* newNode = new ReservationNode{customerName, reservationDateTime, people, phone};
        cout << endl << "Do you want to order food? (yes/no): ";
        string response;
        cin >> response;
        toUpperCase(response);
        if (response == "YES") {
            addFoodToReservation(newNode);
            cout << endl << "Total Cost: RM " << fixed << setprecision(2) << calculateTotal(newNode) << endl;
        }

        if (!tail) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }

        saveReservations();
        cout << endl << "Reservation added successfully!!!" << endl;
        printFinalReservation(newNode);
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
                saveReservations(); // Save changes after each valid operation
                printSingleReservation(reservation); // Display updated reservation
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
            for (FoodOrder* order = temp->foodOrders; order; order = order->next) {
                cout << "   " << itemIndex++ << ". " << order->item->name << ", RM " << fixed << setprecision(2) << order->item->price
                    << " (" << order->quantity << " qty)" << endl;
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

int main() {
    ReservationList reservations;
    reservations.mainMenu();
    return 0;
}
