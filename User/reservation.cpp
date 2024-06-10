#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
#include <iomanip>
#include <limits>
#include <regex>

using namespace std;

struct FoodItem {
    string name;
    string category;
    double price;
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

class ReservationList {
private:
    CategoryNode* categoryHead;
    ReservationNode* head;
    ReservationNode* tail;
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

void searchByName(const string& searchName) {
    head = mergeSortByName(head);

    bool found = false;
    ReservationNode* tempHead = nullptr;
    ReservationNode* tempTail = nullptr;

    for (ReservationNode* current = head; current != nullptr; current = current->next) {
        if (current->customerName.find(searchName) != string::npos) {
            if (!tempHead) {
                tempHead = tempTail = new ReservationNode(*current);
            } else {
                tempTail->next = new ReservationNode(*current);
                tempTail = tempTail->next;
            }
            found = true;
        }
    }
    
    if (found) {
        printSReservations(tempHead); // Call the updated function
    } else {
        cout << "No reservation found under the name containing: " << searchName << endl;
    }

    // Clean up temporary list
    while (tempHead) {
        ReservationNode* temp = tempHead;
        tempHead = tempHead->next;
        delete temp;
    }
}

void searchByDate(const string& searchDate) {
    head = mergeSort(head, true);

    bool found = false;
    ReservationNode* tempHead = nullptr;
    ReservationNode* tempTail = nullptr;

    for (ReservationNode* current = head; current != nullptr; current = current->next) {
        if (current->reservationDateTime.substr(0, 10) == searchDate) {
            if (!tempHead) {
                tempHead = tempTail = new ReservationNode(*current);
            } else {
                tempTail->next = new ReservationNode(*current);
                tempTail = tempTail->next;
            }
            found = true;
        }
    }
    
    if (found) {
        printSReservations(tempHead); // Call the updated function
    } else {
        cout << "No reservations found on the date: " << searchDate << endl;
    }

    // Clean up temporary list
    while (tempHead) {
        ReservationNode* temp = tempHead;
        tempHead = tempHead->next;
        delete temp;
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
        newOrder->price = item->price * quantity;
    }

    double calculateTotal(const ReservationNode* reservation) const {
        double total = 0.0;
        FoodItem* currentItem = reservation->foodOrders;
        while (currentItem) {
            total += currentItem->price;
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

                FoodItem* item = new FoodItem{name, "", stod(price), nullptr};
                addFoodOrder(newNode, item, stoi(quantity));
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
                outFile << ";" << currentOrder->name << ":" << currentOrder->price / currentOrder->price << ":" << currentOrder->price;
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
        } while (!isValidDay);

        string formattedMonth = month < 10 ? "0" + to_string(month) : to_string(month);
        return formattedMonth + "-" + selectedDay;
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
                if (availableDay == day) {
                    return true;
                }
            }
        }
        return false;
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

    int index = 1;
    cout << "--------------------------------------------------------------------------------------------------------" << endl;
    cout << setw(5) << left << "No."
         << setw(30) << left << "Customer Name"
         << setw(20) << "Date And Time"
         << setw(8) << "People"
         << setw(17) << "Phone Number"
         << setw(20) << "Total Price" << endl;
    cout << "--------------------------------------------------------------------------------------------------------" << endl;

    for (ReservationNode* temp = tempHead; temp != nullptr; temp = temp->next, ++index) {
        cout << setw(5) << left << index
             << setw(30) << left << temp->customerName
             << setw(20) << temp->reservationDateTime
             << setw(8) << temp->people
             << setw(17) << temp->phone
             << setw(20) << fixed << setprecision(2) << calculateTotal(temp) << endl;
    }
    cout << "--------------------------------------------------------------------------------------------------------" << endl;

    char viewDetails;
    cout << "Do you want to view the details of any reservation? (y/n): ";
    cin >> viewDetails;
    if (tolower(viewDetails) == 'y') {
        int reservationIndex;
        cout << "Enter the reservation number to view details: ";
        cin >> reservationIndex;
        ReservationNode* reservation = getReservationAtIndex(reservationIndex);
        if (reservation) {
            printSingleReservation(reservation);
        } else {
            cout << "Invalid reservation number." << endl;
        }
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
        FoodItem* currentOrder = reservation->foodOrders;
        double total = 0.0;
        while (currentOrder) {
            cout << "   " << itemIndex++ << ". " << currentOrder->name
                 << ", RM " << fixed << setprecision(2) << currentOrder->price
                 << " (" << currentOrder->price / currentOrder->price << " qty)" << endl;
            total += currentOrder->price;
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
                 << " (" << currentOrder->price / currentOrder->price << " qty)" << endl;
            total += currentOrder->price;
            currentOrder = currentOrder->next;
        }
        cout << "   Total Price: RM " << fixed << setprecision(2) << total << endl << endl;
    }

    void editFood(ReservationNode* reservation) {
        cout << endl << "Current food orders:\n";
        int oi = 1;
        FoodItem* currentOrder = reservation->foodOrders;
        while (currentOrder) {
            cout << oi++ << ". " << currentOrder->name << " - Qty: " << currentOrder->price / currentOrder->price
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

        cout << endl << "Selected: " << selectedOrder->name << " - Qty: " << selectedOrder->price / selectedOrder->price
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
                    selectedOrder->price = selectedOrder->price / selectedOrder->price * newQty;
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
            currentOrder->price = newItem->price * newQty;
        }
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
                cout << "   " << itemIndex++ << ". " << currentOrder->name << ", RM " << fixed << setprecision(2) << currentOrder->price
                     << " (" << currentOrder->price / currentOrder->price << " qty)" << endl;
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

int main() {
    ReservationList reservations;
    reservations.mainMenu();
    return 0;
}
