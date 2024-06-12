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

        void addFoodOrder(ReservationNode* reservation, FoodItem* item, int quantity) {
        FoodItem* newOrder = new FoodItem(*item);
        newOrder->next = reservation->foodOrders;
        reservation->foodOrders = newOrder;
        newOrder->quantity = quantity;
        newOrder->price = item->price;
    }
    
void searchByName(const string& searchName) {
    head = mergeSortByName(head); // Ensures the list is sorted by name for more intuitive output.

    bool found = false;
    ReservationNode* tempHead = nullptr;
    ReservationNode* tempTail = nullptr;

    for (ReservationNode* current = head; current != nullptr; current = current->next) {
        if (current->customerName == searchName) {  // Exact match condition
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
        printSReservations(tempHead);
    } else {
        cout << "No reservation found under the name: " << searchName << endl;
    }

    // Clean up temporary list
    while (tempHead) {
        ReservationNode* temp = tempHead;
        tempHead = tempHead->next;
        delete temp;
    }
}




void searchByDate() {
    int month = getMonthFromUser("Enter reservation month (06-12): ");
    string date = selectDayFromFile(month);

    head = mergeSort(head, true);

    bool found = false;
    ReservationNode* tempHead = nullptr;
    ReservationNode* tempTail = nullptr;

    for (ReservationNode* current = head; current != nullptr; current = current->next) {
        if (current->reservationDateTime.substr(5, 5) == date) {  // Matching the formatted date (MM-DD)
            ReservationNode* newNode = new ReservationNode(*current);
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
        printSReservations(tempHead);
    } else {
        cout << "No reservations found on the date: " << date << endl;
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
             << "4. Search Reservation by Name" << endl
             << "5. Search Reservation by Date" << endl
             << "6. Sort by Date Ascending" << endl
             << "7. Sort by Date Descending" << endl
             << "8. Sort by Name Ascending" << endl
             << "9. Exit" << endl
             << "Please enter your selection: ";
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
        loadReservations();
        bool running = true;
        while (running) {
            displayMainMenu();
            int selection;
            cin >> selection;

            switch (selection) {
                case 4:
                    cout << "Enter part of the name to search for: ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    getline(cin, input);
                    searchByName(input);
                    break;

                case 5:
                    cout << "Enter the date to search for: ";
                    searchByDate();
                    break;
                case 6:
                    head = mergeSort(head, true);
                    cout << "Reservations sorted by date (Ascending).\n";
                    printSReservations(head);
                    break;
                case 7:
                    head = mergeSort(head, false);
                    cout << "Reservations sorted by date (Descending).\n";
                    printSReservations(head);
                    break;
                case 8:
                    head = mergeSortByName(head);
                    cout << "Reservations sorted by name (Ascending).\n";
                    printSReservations(head);
                    break;
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

int main() {
    ReservationList reservations;
    reservations.mainMenu();
    return 0;
}