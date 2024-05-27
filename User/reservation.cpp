#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
#include <regex>

using namespace std;

struct ReservationNode {
    string customerName;
    string reservationDateTime;  
    int people;
    string phone;
    ReservationNode* next;
};

class ReservationList {
private:
    const string reservationFile = "reservations.txt";
    const int reservationYear = 2024;
    ReservationNode* head = nullptr;
    ReservationNode* tail = nullptr;

public:
    ReservationList() {}
    ~ReservationList() {
        clearReservations();
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
        ifstream inFile(reservationFile);
        if (!inFile) {
            cerr << "Failed to open file for reading." << endl;
            return;
        }
        clearReservations();
        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            ReservationNode* newNode = new ReservationNode();
            getline(ss, newNode->customerName, ',');
            getline(ss, newNode->reservationDateTime, ',');
            ss >> newNode->people;
            ss.ignore(numeric_limits<streamsize>::max(), ',');
            getline(ss, newNode->phone, ',');
            newNode->next = nullptr;
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
        ofstream outFile(reservationFile);
        if (!outFile) {
            cerr << "Failed to open file for writing." << endl;
            return;
        }
        for (ReservationNode* temp = head; temp != nullptr; temp = temp->next) {
            outFile << temp->customerName << ","
                    << temp->reservationDateTime << ","
                    << temp->people << ","
                    << temp->phone << "\n";
        }
        outFile.close();
    }

    void addReservation() {
        string customerName, phone, date, time;
        int people;

        cout << "Enter customer name: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        getline(cin, customerName);

        date = getDateFromUser("Enter reservation date (MM-DD): ");
        time = getTimeFromUser("Enter reservation time (HH:MM): ");
        string reservationDateTime = to_string(reservationYear) + "-" + date + " " + time;

        cout << "Enter number of people (1-15): ";
        while (!(cin >> people) || !isValidPeople(people)) {
            cout << "Invalid number. "<<endl;
            cout<<"Please enter a number from 1 to 15 : ";
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Enter the customer phone number (must start with '01' and be 10 digits long): ";
        cin >> phone;
        while (!isValidPhone(phone)) {
            cout << "Invalid phone number. "<<endl;
            cout<<"Please enter a valid phone number: ";
            cin.clear();
            cin>>phone;
        }

        ReservationNode* newNode = new ReservationNode{customerName, reservationDateTime, people, phone, nullptr};
        if (!tail) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }

        saveReservations();
        cout <<endl<< "Reservation added successfully!!!\n";
    }

    void editReservation() {
        int index;
        cout<< endl << "Enter the index of the reservation you want to edit: ";
        cin >> index;
        ReservationNode* reservation = getReservationAtIndex(index);
        if (!reservation) {
            cout << "Invalid index.\n";
            return;
        }

        cout << endl
             <<"Select what to edit:\n"
             << "1. Number of People\n"
             << "2. Date \n"
             << "3. Time\n"
             << "4. Phone Number\n"
             << "Enter choice: ";
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
            default:
                cout << "Invalid choice.\n";
                break;
        }
        saveReservations();
    }

    void printReservations() const {
        cout <<endl<< "\nCurrent Reservations:\n";
        int index = 1;
        for (ReservationNode* temp = head; temp != nullptr; temp = temp->next, ++index) {
            cout << index << ". " << temp->customerName << " - " << temp->reservationDateTime
                 << " - " << temp->people << " people - " << temp->phone << endl;
        }
    }

    int getCount() const {
        int count = 0;
        for (ReservationNode* temp = head; temp; temp = temp->next) {
            ++count;
        }
        return count;
    }

    ReservationNode* getReservationAtIndex(int index) const {
        if (index < 1 || index > getCount()) {
            return nullptr;
        }
        ReservationNode* temp = head;
        for (int count = 1; temp != nullptr && count < index; temp = temp->next, ++count);
        return temp;
    }

private:
    string getDateFromUser(const string& prompt) {
        string date;
        regex datePattern(R"(^\d{2}-\d{2}$)");  
        do {
            cout << prompt;
            cin >> date;
            if (!regex_match(date, datePattern)) {
                cout << "Invalid date format. Please enter date as MM-DD.\n";
            } else {
                int month = stoi(date.substr(0, 2));
                int day = stoi(date.substr(3, 2));
                if (!isValidDate(month, day)) {
                    cout << "Invalid date. Please enter a valid date.\n";
                } else {
                    break;
                }
            }
        } while (true);
        return date;
    }

    string getTimeFromUser(const string& prompt) {
        string time;
        regex timePattern(R"(^\d{2}:\d{2}$)");  
        do {
            cout << prompt;
            cin >> time;
            if (!regex_match(time, timePattern)) {
                cout << "Invalid time format. Please enter time as HH:MM.\n";
            } else {
                int hour = stoi(time.substr(0, 2));
                int minute = stoi(time.substr(3, 2));
                if (!isValidTime(hour, minute)) {
                    cout << "Invalid time. Time must be between 10:00 and 20:59.\n";
                } else {
                    break;
                }
            }
        } while (true);
        return time;
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
    string newDate = getDateFromUser("Enter new date (MM-DD): ");
    if (reservation->reservationDateTime.length() >= 16) {
        string currentTime = reservation->reservationDateTime.substr(11, 5); // Extract HH:MM from YYYY-MM-DD HH:MM
        reservation->reservationDateTime = to_string(reservationYear) + "-" + newDate + " " + currentTime;
    } else {
        cout << "Error: Current reservation date-time format is invalid." << endl;
    }
}

void editTime(ReservationNode* reservation) {
    string newTime = getTimeFromUser("Enter new time (HH:MM): ");
    if (reservation->reservationDateTime.length() >= 16) {
        string currentDate = reservation->reservationDateTime.substr(5, 5); // Extract MM-DD from YYYY-MM-DD HH:MM
        reservation->reservationDateTime = to_string(reservationYear) + "-" + currentDate + " " + newTime;
    } else {
        cout << "Error: Current reservation date-time format is invalid." << endl;
    }
}

    void editPhone(ReservationNode* reservation) {
        bool validInput = false;
        string newPhone;
        while (!validInput) {
            cout << "Enter new phone number: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, newPhone);
            if (isValidPhone(newPhone)) {
                reservation->phone = newPhone;
                validInput = true;
            } else {
                cout << "Invalid phone number. It must be 10 digits long and start with '01'.\n";
            }
        }
    }

    bool isValidPhone(const string& phone) const {
        return (phone.length() == 10 && phone.substr(0, 2) == "01");
    }

    bool isValidPeople(int people) const {
        return (people > 0 && people <= 15);
    }

    bool isValidDate(int month, int day) const {
        int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (month == 2 && day == 29 && isLeapYear(reservationYear)) {
            return true;
        }
        return (month >= 1 && month <= 12 && day >= 1 && day <= days_in_month[month]);
    }

    bool isLeapYear(int year) const {
        return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
    }

    bool isValidTime(int hour, int minute) const {
        return (hour >= 10 && hour <= 20 && minute >= 0 && minute < 60);
    }
};

void mainmenu() {
    ReservationList reservations;
    reservations.loadReservations();

    bool running = true;
    while (running) {
        cout <<  endl 
             << "====================================================================" << endl
             << "\tRestaurant Reservation Management System" << endl
             << "====================================================================" << endl
             << "1. Add Reservation" << endl
             << "2. Edit Reservation" << endl
             << "3. View Reservation"<<endl
             << "4. Exit" << endl
             << "Please enter your selection: ";
        int selection;
        cin >> selection;

        switch (selection) {
            case 1:
                reservations.addReservation();
                break;
            case 2:
                reservations.printReservations();
                reservations.editReservation();
                reservations.printReservations();
                break;
            case 3:
                reservations.printReservations();
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

int main() {
    mainmenu();
    return 0;
}
