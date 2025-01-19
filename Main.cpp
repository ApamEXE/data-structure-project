//Main.cpp
#define _CRT_SECURE_NO_WARNINGS
// suppressed warning kalau pakai ctime
#include "HotelReservation.h"
#include <iostream>

using namespace std;

void showMenu() {
    cout << "\n--- Hotel Reservation System ---" << endl;
    cout << "1. Admin Login" << endl;
    cout << "2. Guest Reservation" << endl;
    cout << "3. Price Calculator" << endl;
    cout << "4. Exit" << endl;
    cout << "Please select an option (1-4): ";
}

int main() {
    Hotel hotel;
    GuestList guestList;
    guestList.loadFromFile("reservations.txt");
    // Initialize Admin object here properly
    Admin admin;  // Ensure admin is properly initialized

    int choice;
    while (true) {
        showMenu();
        cin >> choice;

        switch (choice) {
        case 1: {
            // Admin Login
            string user, pass;
            cout << "\n--- Admin Login ---" << endl;
            cout << "Enter username: ";
            cin >> user;
            cout << "Enter password: ";
            cin >> pass;

            if (admin.login(user, pass)) {  // Ensure the admin object is initialized
                cout << "\nLogin successful!" << endl;

                // Admin options menu
                int adminChoice;
                do {
                    cout << "\n--- Admin Menu ---" << endl;
                    cout << "1. View Available Rooms" << endl;
                    cout << "2. Update Room Availability" << endl;
                    cout << "3. View Reservation List" << endl;
                    cout << "4. Search Reservation" << endl;
                    cout << "5. Process Reservations" << endl;  // New option to process reservations
                    cout << "6. Extend Reservations" << endl;
                    cout << "7. Logout" << endl;
                    cout << "Please select an option (1-7): ";
                    cin >> adminChoice;

                    switch (adminChoice) {
                    case 1:
                        hotel.displayAvailableRoomsByType(guestList);
                        break;
                    case 2: {
                        int roomNumber;
                        bool availability;
                        cout << "Enter Room Number to update: ";
                        cin >> roomNumber;
                        cout << "Enter new availability (1 for available, 0 for not available): ";
                        cin >> availability;
                        hotel.updateRoomAvailability(roomNumber, availability, guestList);
                        break;
                    }
                    case 3:
                        guestList.loadFromFile("reservations.txt");
                        if (guestList.isEmpty()) {
                            cout << "No Reservation is available." << endl;
                        }
                        else {
                            guestList.displayAll();
                        }
                        break;
                    case 4: { // Search Reservation
                        string searchTerm;
                        cout << "\n--- Search Reservation ---" << endl;
                        cout << "Enter guest name, phone number, or room number: ";
                        cin.ignore(); // Clear the input buffer
                        getline(cin, searchTerm);

                        Guest* foundGuest = guestList.searchGuest(searchTerm);
                        cout << "---------------------------------------------------------------------------------" << endl;
                        cout << "| Room Number | Room Type |      Name      |     Phone    | Time of Reservation |" << endl;
                        cout << "---------------------------------------------------------------------------------" << endl;

                        if (foundGuest) {
                            cout << "| " << setw(12) << left << foundGuest->getRoomNumber()
                                << "| " << setw(10) << left << foundGuest->getRoomType()
                                << "| " << setw(15) << left << foundGuest->getName()
                                << "| " << setw(13) << left << foundGuest->getPhone()
                                << "| " << setw(20) << left << foundGuest->getRTime() << "|" << endl;
                        }
                        else {
                            cout << "| No reservation found for: " << searchTerm << "                                                   |" << endl;
                        }

                        cout << "---------------------------------------------------------------------------------" << endl;
                        break;

                    }
                    case 5: { // Process Reservations (Queue)
                        hotel.processWaitingList(); // Process the next guest in the queue
                        break;
                    }
                    case 6: {
                        //Extend Reservation
                        int roomNumber;
                        int additionalDays;
                        cout << "\n--- Extend Reservation ---" << endl;
                        cout << "Enter room number to extend reservation: ";
                        cin >> roomNumber;
                        cout << "Enter number of days to extend: ";
                        cin >> additionalDays;

                        // Find guest by room number
                        Guest* guest = guestList.searchGuest(to_string(roomNumber));
                        if (guest != nullptr) {
                            // Extend reservation by additional days
                            string originalTime = guest->getRTime();
                            processReservationOrCalculatePrice(hotel, guestList, guest->getRoomType()[0],
                                additionalDays, true, originalTime);

                            cout << "Reservation extended for " << additionalDays << " days." << endl;
                        }
                        else {
                            cout << "No reservation found for room number " << roomNumber << endl;
                        }
                        break;
                    }
                    case 7:
                        cout << "Logging out..." << endl;
                        break;
                    default:
                        cout << "Invalid choice, try again." << endl;
                    }
                } while (adminChoice != 7);
            }
            else {
                cout << "Invalid login credentials. Please try again." << endl;
            }
            break;
        }
        case 2: {
            // Guest Reservation
            char roomType;
            int days;
            cout << "\n--- Guest Reservation ---" << endl;
            cout << "Room types:\n S for Single \t\t[Rm 100 per day]\n D for Double \t\t[Rm 150 per day]\n P for Penthouse \t[Rm 500 per day]\n T for Suite \t\t[Rm 200 per day]\n";
            cout << "\nEnter room type : ";
            cin >> roomType;
            cout << "Enter number of days: ";
            cin >> days;

            // set time of the reservation
            time_t now = time(0);
            tm* localTime = localtime(&now);

            string rtime =
                (localTime->tm_mon + 1 < 10 ? "0" : "") + to_string(localTime->tm_mon + 1) + "/" +  // Month
                (localTime->tm_mday < 10 ? "0" : "") + to_string(localTime->tm_mday) + "/" +        // Day
                to_string(localTime->tm_year + 1900) + "-" +                                       // Year
                (localTime->tm_hour < 10 ? "0" : "") + to_string(localTime->tm_hour) + ":" +       // Hour
                (localTime->tm_min < 10 ? "0" : "") + to_string(localTime->tm_min) + ":" +        // Minute
                (localTime->tm_sec < 10 ? "0" : "") + to_string(localTime->tm_sec);               // Second

            // Process reservation
            processReservationOrCalculatePrice(hotel, guestList, roomType, days, true, rtime);

            break;
        }
        case 3: {
            // Price Calculator
            char roomType;
            int days;
            string rtime = "";
            cout << "\n--- Price Calculator ---" << endl;
            cout << "Room types:\n S for Single \t\t[Rm 100 per day]\n D for Double \t\t[Rm 150 per day]\n P for Penthouse \t[Rm 500 per day]\n T for Suite \t\t[Rm 200 per day]\n";
            cout << "\nEnter room type : ";
            cin >> roomType;
            cout << "Enter number of days: ";
            cin >> days;

            // Just calculate the price
            processReservationOrCalculatePrice(hotel, guestList, roomType, days, false, rtime);
            break;
        }
        case 4:
            cout << "Exiting program..." << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }

    }

    return 0;
}