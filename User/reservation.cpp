#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
#include <iomanip>
#include <limits>
#include <regex>
#include <map>
#include <vector>

using namespace std;

struct FoodItem 
{
    string name;
    string category;
    double price;
};

struct ReservationNode 
{
    string customerName;
    string reservationDateTime;  
    int people;
    string phone;
    vector<pair<FoodItem, int>> foodOrders; // Store food item and quantity
    ReservationNode* next = nullptr;
};

class ReservationList 
{
private:

    vector<FoodItem> foodItems;
    map<string, vector<FoodItem>> categorizedFood;
    ReservationNode* head = nullptr;
    ReservationNode* tail = nullptr;
    const int reservationYear = 2024;

    void loadFoodItems() {
    ifstream inFile("fooddata.txt");
    if (!inFile) 
    {
        cerr << "Failed to open food data file for reading." << endl;
        return;
    }
    string line;
    while (getline(inFile, line)) 
    {
            stringstream ss(line);
            FoodItem item;
            if (getline(ss, item.name, ',') && getline(ss, item.category, ',') && (ss >> item.price)) 
            {
                foodItems.push_back(item);
                categorizedFood[item.category].push_back(item);
            }
        }
        inFile.close();
    }

    void displayMainMenu() 
    {
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
        for (const auto& category : categorizedFood) 
        {
            cout << i++ << ". " << category.first << endl;
        }
        cout << i << ". Main Menu" << endl;
        }

    void toUpperCase(string& str) {
        for (char& c : str) 
        {
            c = toupper(c);
        }
    }


    void addFoodToReservation(ReservationNode* reservation) 
    {
        int choice;
        do {
            cout<<endl;
            cout <<"--MENU--"<<endl;
            displayCategories();
            cout << "Select a category (or choose " << categorizedFood.size() + 1 << " for Main Menu): ";
            cin >> choice;
            if (choice < 1 || choice > categorizedFood.size() + 1)
             {
                cout << "Invalid option, please choose again.\n";
                continue;
            }
            if (choice == categorizedFood.size() + 1) 
            {
                break;  // Exit on 'Main Menu'
            }

            auto it = categorizedFood.begin();
            advance(it, choice - 1);  // Adjust to zero-based index

            cout <<endl<< "Menu for " << it->first << ":\n";
            int j = 1;
            for (const auto& item : it->second) 
            {
                cout << j++ << ". " << item.name << " - RM" << fixed << setprecision(2) << item.price << endl;
            }

            cout <<endl<< "Which item do you want (0 to cancel): ";
            int itemChoice;
            cin >> itemChoice;
            if (itemChoice < 1 || itemChoice > it->second.size()) 
            {
                cout << "Invalid selection, returning to categories.\n";
                continue;
            }

            const FoodItem& selectedItem = it->second[itemChoice - 1];
            cout <<endl<< "How many quantities of " << selectedItem.name << "?: ";
            int quantity;
            cin >> quantity;
            if (quantity > 0) 
            {
                reservation->foodOrders.push_back(make_pair(selectedItem, quantity));
                cout << quantity << " of " << selectedItem.name << " added. Add more or return to Main Menu.\n";
            } else 
            {
                cout << "Invalid quantity, returning to categories.\n";
            }
        } while (true);
    }



    double calculateTotal(const ReservationNode* reservation) const 
    {
        double total = 0.0;
        for (const auto& order : reservation->foodOrders) 
        {
            total += order.first.price * order.second;
        }
        return total;
    }

    void clearReservations() 
    {
        while (head != nullptr) 
        {
            ReservationNode* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
    }

    void loadReservations()
    {
        ifstream inFile("reservations.txt");
        if (!inFile) 
        {
            cerr << "Failed to open file for reading." << endl;
            return;
        }
        clearReservations();
        string line;

        while (getline(inFile, line)) 
        {
            stringstream ss(line);
            string part, itemDetails;
            ReservationNode* newNode = new ReservationNode();

            getline(ss, newNode->customerName, ',');
            getline(ss, newNode->reservationDateTime, ',');
            getline(ss, part, ',');
            newNode->people = stoi(part);
            getline(ss, newNode->phone, ';');

            while (getline(ss, itemDetails, ';')) 
            {
                stringstream itemStream(itemDetails);
                string name, price, quantity;
                getline(itemStream, name, ':');
                getline(itemStream, price, ':');
                getline(itemStream, quantity);

                FoodItem item = {name, "", stod(price)};
                newNode->foodOrders.push_back(make_pair(item, stoi(quantity)));
            }

            if (!head) 
            {
                head = tail = newNode;
            } 
            else 
            {
                tail->next = newNode;
                tail = newNode;
            }
        }
        inFile.close();
    }


    void saveReservations() 
    {
        ofstream outFile("reservations.txt");
        if (!outFile) 
        {
            cerr << "Failed to open file for writing." << endl;
            return;
        }
        for (ReservationNode* temp = head; temp != nullptr; temp = temp->next) 
        {
            outFile << temp->customerName << ","
                    << temp->reservationDateTime << ","
                    << temp->people << ","
                    << temp->phone;
            for (auto& order : temp->foodOrders) 
            {
                // Ensure clear and parseable output format
                outFile << "; " << order.first.name << ":" << order.first.price << ":" << order.second;
            }
            outFile << endl;  // End of one reservation's data
        }
        outFile.close();
    }


    int getMonthFromUser(const string& prompt) 
    {
        int month;
        do 
        {
            cout << prompt;
            cin >> month;
            if (month < 6 || month > 12) 
            {
                cout << "Invalid month. Please enter a month between 06 and 12.\n";
            } 
            else 
            {
                break;
            }
        } while (true);
        return month;
    }

    string selectDayFromFile(int month) 
    {
        string filename;
        string monthn;
        switch (month) 
        {
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
            default: cerr << "Invalid month provided."; return "01"; 
        }

        ifstream file(filename);
        if (!file) 
        {
            cerr << "Failed to open day file: " << filename << endl;
            return "01"; 
        }

        vector<string> availableDays;
        string line, day;
        cout << endl<<"Available days from " << monthn<< " : " <<endl;
        cout<<endl;
        while (getline(file, line)) 
        {
            stringstream ss(line);
            while (ss >> day) 
            { // Extract each day from the line
                day = day.length() < 2 ? "0" + day : day; // Format each day as two-digit string if necessary
                availableDays.push_back(day);
                cout << day << " ";
            }
            cout << endl;
        }
        file.close();

        string selectedDay;
        bool isValidDay = false;

        do 
        {
            cout << "Select a day (DD): ";
            cin >> selectedDay;
            if (selectedDay.length() < 2) 
            {
                selectedDay = "0" + selectedDay; // Ensure two-digit format for comparison
            }
            if (find(availableDays.begin(), availableDays.end(), selectedDay) != availableDays.end()) 
            {
                isValidDay = true;
            } 
            else 
            {
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
        for (int i = 0; i < 20; ++i) 
        {
            int hour = 10 + i / 2;
            string minute = (i % 2 == 0) ? "00" : "30";
            string am_pm = (hour >= 12) ? " PM" : " AM";
            if (hour > 12) hour -= 12;
            cout << (i + 1) << ". " << setw(2) << setfill('0') << hour << ":" << minute << am_pm << endl;
        }
        cout << "Select a time slot (1-20): ";

        while (!(cin >> choice) || choice < 1 || choice > 20) 
        {
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

    void editPeople(ReservationNode* reservation) 
    {
        int newPeople;
        bool validInput = false;
        while (!validInput) {
            cout << "Enter new number of people (1-15): ";
            cin >> newPeople;
            if (isValidPeople(newPeople)) 
            {
                reservation->people = newPeople;
                validInput = true;
            }
            else 
            {
                cout << "Invalid number. Please enter a number from 1 to 15.\n";
            }
        }
    }

    void editDate(ReservationNode* reservation) 
    {
        int month = getMonthFromUser("Enter reservation month (06-12): ");
        string newDate = selectDayFromFile(month);
        if (reservation->reservationDateTime.length() >= 16) 
        {
            // Extract HH:MM AM/PM from the current reservation datetime
            string currentTime = reservation->reservationDateTime.substr(11); // Get everything after the date part (includes time and AM/PM)

            // Construct the new datetime string with the new date and the extracted time
            reservation->reservationDateTime = to_string(reservationYear) + "-" + newDate + " " + currentTime;
        }
        else 
        {
            cout << "Error: Current reservation date-time format is invalid." << endl;
        }
    }


    void editTime(ReservationNode* reservation) 
    {
        // Call the same function used in adding a reservation to get the new time
        string newTime = getTimeFromUser("Choose new reservation time:");

        // Assume the reservation date-time string is formatted correctly as "YYYY-MM-DD HH:MM"
        if (reservation->reservationDateTime.length() >= 16) 
        {
            // Extract just the date part (first 10 characters represent the date "YYYY-MM-DD")
            string currentDate = reservation->reservationDateTime.substr(0, 10);

            // Combine the extracted date with the new time to update the reservation
            reservation->reservationDateTime = currentDate + " " + newTime;
        } 
        else 
        {
            cout << "Error: Current reservation date-time format is invalid." << endl;
        }
    }

    void editPhone(ReservationNode* reservation) 
    {
        bool validInput = false;
        string newPhone;
        while (!validInput) {
            cout << "Enter new phone number: ";
            // Ignore input buffer only if it's necessary. Check if there is something to ignore.
            if (cin.peek() == '\n') {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            getline(cin, newPhone);
            if (isValidPhone(newPhone)) 
            {
                reservation->phone = newPhone;
                validInput = true;
            } 
            else 
            {
                cout << "Invalid phone number. It must be 10 digits long and start with '01'.\n";
            }
        }
    }

    void printSingleReservation(const ReservationNode* reservation) const 
    {
        if (!reservation) 
        {
            cout << "Invalid reservation reference provided." << endl;
            return;
        }
        cout << endl<<"Updated Reservation Details :" << endl
            << "Name : " << reservation->customerName << endl
            << "Order Date : " << reservation->reservationDateTime << endl
            << "Total of People : " << reservation->people << " people" << endl
            << "Contact Number : " << reservation->phone << endl
            << "Orders :" << endl;
        int itemIndex = 1;
        double total = 0.0;
        for (const auto& order : reservation->foodOrders) 
        {
            cout << "   " << itemIndex++ << ". " << order.first.name
                << ", RM " << fixed << setprecision(2) << order.first.price
                << " (" << order.second << " qty)" << endl;
            total += order.first.price * order.second;
        }
        cout << "   Total Price: RM " << fixed << setprecision(2) << total << endl << endl;
    }

    void printFinalReservation(const ReservationNode* reservation) const 
    {
        if (!reservation) 
        {
            cout << "Invalid reservation reference provided." << endl;
            return;
        }
        cout << endl<<"Final Reservation Details :" << endl
            << "Name : " << reservation->customerName << endl
            << "Order Date : " << reservation->reservationDateTime << endl
            << "Total of People : " << reservation->people << " people" << endl
            << "Contact Number : " << reservation->phone << endl
            << "Orders :" << endl;
        int itemIndex = 1;
        double total = 0.0;
        for (const auto& order : reservation->foodOrders) 
        {
            cout << "   " << itemIndex++ << ". " << order.first.name
                << ", RM " << fixed << setprecision(2) << order.first.price
                << " (" << order.second << " qty)" << endl;
            total += order.first.price * order.second;
        }
        cout << "   Total Price: RM " << fixed << setprecision(2) << total << endl << endl;
    }


    void editFood(ReservationNode* reservation) 
    {
        cout << endl << "Current food orders:\n";
        int oi = 1;
        for (const auto& order : reservation->foodOrders) 
        {
            cout << oi++ << ". " << order.first.name << " - Qty: " << order.second << ", Price per item: RM" << fixed << setprecision(2) << order.first.price << endl;
        }

        cout << endl << "Select the food order to edit (0 to cancel): ";
        int foodIndex;
        while (!(cin >> foodIndex) || foodIndex < 0 || foodIndex > reservation->foodOrders.size()) 
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (foodIndex == 0) return;
            cout << "Invalid selection. Please enter a valid index or 0 to cancel: ";
        }

        if (foodIndex == 0) return;

        auto& selectedOrder = reservation->foodOrders[foodIndex - 1];
        cout << endl << "Selected: " << selectedOrder.first.name << " - Qty: " << selectedOrder.second << ", Price per item: RM" << fixed << setprecision(2) << selectedOrder.first.price << endl;
        cout << endl << "Options: \n1. Change Quantity\n2. Replace Item\n3. Remove Item\nEnter your choice: ";
        int choice;
        while (!(cin >> choice) || choice < 1 || choice > 3) 
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please enter a valid choice (1-3): ";
        }

        switch (choice) 
        {
            case 1: 
            {
                int newQty;
                cout << endl << "Enter new quantity (0 to cancel order): ";
                while (!(cin >> newQty) || newQty < 0) 
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (newQty == 0)
                    {
                        cout << "Order edit cancelled.\n";
                        return;
                    }
                    cout << "Invalid quantity. Please enter a valid quantity or 0 to cancel: ";
                }
                if (newQty > 0) 
                {
                    selectedOrder.second = newQty;
                } 
                else 
                {
                    reservation->foodOrders.erase(reservation->foodOrders.begin() + foodIndex - 1);
                    cout << "Order edit cancelled.\n";
                }
                break;
            }
            case 2: 
            {
                cout << endl << "Select a new food item to replace : ";
                displayCategories();
                int categoryChoice;
                cout << "Select category: ";
                while (!(cin >> categoryChoice) || categoryChoice < 1 || categoryChoice > categorizedFood.size()) 
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid category. Please select a valid category: ";
                }

                auto it = categorizedFood.begin();
                advance(it, categoryChoice - 1);
                int j = 1, itemChoice;
                cout << "Food : " << endl;
                for (const auto& item : it->second) 
                {
                    cout << j++ << ". " << item.name << " - RM" << fixed << setprecision(2) << item.price << endl;
                }
                cout << endl << "Select food item to replace with: ";
                while (!(cin >> itemChoice) || itemChoice < 1 || itemChoice > it->second.size()) 
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid item. Please select a valid food item: ";
                }
                const FoodItem& newItem = it->second[itemChoice - 1];
                int newQty;
                cout << endl << "Enter quantity for the new item: ";
                while (!(cin >> newQty) || newQty < 1) 
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid quantity. Please enter a positive number: ";
                }
                selectedOrder = make_pair(newItem, newQty);
                break;
            }
            case 3:
                reservation->foodOrders.erase(reservation->foodOrders.begin() + foodIndex - 1);
                cout << "Item removed from the order.\n";
                break;
        }
        saveReservations();
        cout << "Order editupdated successfully!\n";
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

    public:

    ReservationList() {
        loadFoodItems(); 
    }
    ~ReservationList() {
        clearReservations();
    }

    void mainMenu() 
    {
            loadReservations(); // Load reservations at the start
            bool running = true;
            while (running) {
                displayMainMenu();
                int selection;
                cin >> selection;

                switch (selection) 
                {
                    case 1:
                        addReservation();
                        break;
                    case 2:
                        printReservations();
                        editReservation();
                        break;
                    case 3:
                        cout<<"--History of Reservations--"<<endl;
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

        void addReservation() 
        {
            string customerName, phone, time, date;
            int people, month;

            cout << endl<<"Enter customer name: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            getline(cin, customerName);

            month = getMonthFromUser("Enter reservation month (06-12): ");
            date = selectDayFromFile(month);

            time = getTimeFromUser("Enter reservation time (HH:MM): ");
            string reservationDateTime = to_string(reservationYear) + "-" + date + " " + time;

            cout << "Enter number of people (1-15): ";
            while (!(cin >> people) || !isValidPeople(people)) 
            {
                cout << "Invalid number. Please enter a number from 1 to 15: ";
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            cout << "Enter the customer phone number (must start with '01' and be 10 digits long): ";
            cin >> phone;
            while (!isValidPhone(phone)) 
            {
                cout << "Invalid phone number. Please enter a valid phone number: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin >> phone;
            }

            ReservationNode* newNode = new ReservationNode{customerName, reservationDateTime, people, phone};
            cout <<endl<< "Do you want to order food? (yes/no): ";
            string response;
            cin >> response;
            toUpperCase(response); 
            if (response == "YES") 
            {
                addFoodToReservation(newNode);
                cout <<endl<<"Total Cost: RM " << fixed << setprecision(2) << calculateTotal(newNode) << endl;
            }

            if (!tail) {
                head = tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }

            saveReservations();
            cout <<endl<<"Reservation added successfully!!!" << endl;
            printFinalReservation(newNode);

        }

        void editReservation() 
        {
            cout << endl<<"Enter the index of the reservation you want to edit: ";
            int index;
            cin >> index;
            ReservationNode* reservation = getReservationAtIndex(index);
            if (!reservation) 
            {
                cout << "Invalid index.\n";
                return;
            }

            bool continueEditing = true;
            while (continueEditing) {
                cout << endl<<"Select what to edit:\n"
                    << "1. Number of People\n"
                    << "2. Date\n"
                    << "3. Time\n"
                    << "4. Phone Number\n"
                    << "5. Food Orders\n"
                    << "6. Finish Editing (return to main menu)\n"
                    << "Enter your choice: ";
                int choice;
                cin >> choice;

                switch (choice)
                {
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
                    case 6:
                {
                        continueEditing = false;
                        printFinalReservation(reservation);
                        break;
                }
                    default:
                        cout << "Invalid choice. Please try again.\n";
                        continue;
                }

                if (choice >= 1 && choice <= 5) 
                {
                    saveReservations(); // Save changes after each valid operation
                    printSingleReservation(reservation); // Display updated reservation
                }
            }
        }


    void printReservations() const 
    {
        int index = 1;
        for (ReservationNode* temp = head; temp != nullptr; temp = temp->next, ++index) {
            cout << endl<<index << ". Name: " << temp->customerName << endl
                << "   Order Date: " << temp->reservationDateTime << endl
                << "   Total of People: " << temp->people << " people" << endl
                << "   Contact Number: " << temp->phone << endl
                << "   Orders: " << endl;
            int itemIndex = 1;
            for (auto& order : temp->foodOrders) {
                cout << "   " << itemIndex++ << ". " << order.first.name << ", RM " << fixed << setprecision(2) << order.first.price
                    << " (" << order.second << " qty)" << endl;
            }
            double total = calculateTotal(temp);
            cout << "   Total Price: RM " << fixed << setprecision(2) << total << endl;
        }
    }

    ReservationNode* getReservationAtIndex(int index) const 
    {
        if (index < 1 || index > getCount())
        {
            return nullptr;
        }
        ReservationNode* temp = head;
        for (int count = 1; temp != nullptr && count < index; temp=temp->next, ++count);
        return temp;
    }

    int getCount() const {
        int count = 0;
        for (ReservationNode* temp = head; temp; temp = temp->next) 
        {
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


