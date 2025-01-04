#include<iostream>
#include "Reservation.h"
#include <fstream>

using namespace std;

void showMenu() {
    cout << "\n--- Hotel Reservation System ---" << endl;
    cout << "1. Admin Login" << endl;
    cout << "2. Guest Reservation" << endl;
    cout << "3. Exit" << endl;
    cout << "Please select an option (1-3): ";
}

int main() {
    Hotel hotel;
    GuestList guestList;
    Admin admin;

    int choice;
    while (true) {
        showMenu();  // Display the menu
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

            if (admin.login(user, pass)) {
                cout << "\nLogin successful!" << endl;

                // Admin options menu
                int adminChoice;
                do {
                    cout << "\n--- Admin Menu ---" << endl;
                    cout << "1. View Available Rooms" << endl;
                    cout << "2. Update Room Availability" << endl;
                    cout << "3. View Reservation List" << endl;
                    cout << "4. Logout" << endl;
                    cout << "Please select an option (1-4): ";
                    cin >> adminChoice;

                    switch (adminChoice) {
                    case 1:
                        //  guestList.loadFromFile("iFile.txt");  // Load guest list from file

                          // Now display available rooms
                        hotel.displayAvailableRoomsByType();  // Display available rooms by type
                        break;

                    case 2: {
                        int roomNumber;
                        bool availability;
                        cout << "Enter Room Number to update: ";
                        cin >> roomNumber;
                        cout << "Enter new availability (1 for available, 0 for not available): ";
                        cin >> availability;
                        hotel.updateRoomAvailability(roomNumber, availability);
                        break;
                    }
                    case 3:
                        // Make sure to load the file here before checking
                        guestList.loadFromFile("iFile.txt");  // This loads the guest list from the file

                        if (guestList.isEmpty()) {
                            cout << "No Reservation is available." << endl;
                        }
                        else {
                            guestList.displayAll();
                        }
                        break;
                    case 4:
                        cout << "Logging out..." << endl;
                        break;
                    default:
                        cout << "Invalid choice, try again." << endl;
                    }
                } while (adminChoice != 4);
            }
            else {
                cout << "Invalid login credentials. Please try again." << endl;
            }
            break;
        }
        case 2: {
            // Guest Reservation
            string guestName, guestPhone;
            char roomType, anotherReservation;

            //  do {
            cout << "\n--- Guest Reservation ---" << endl;
            cout << "Enter your name: ";
            cin >> guestName;
            cout << "Enter your phone number: ";
            cin >> guestPhone;


            // Display room types with prices before asking for input
            cout << "\nRoom Types and Prices:" << endl;
            cout << "S: Single (RM 100 per night)" << endl;
            cout << "D: Double (RM 150 per night)" << endl;
            cout << "P: Penthouse (RM 300 per night)" << endl;
            cout << "T: Suite (RM 200 per night)" << endl;

            // Ask user for room type
            cout << "\nEnter room type (S for Single, D for Double, P for Penthouse, T for Suite): ";
            cin >> roomType;

            // Handle room type input and assign the price
            string roomTypeStr;
            int roomPrice = 0;
            switch (roomType) {
            case 'S': case 's':
                roomTypeStr = "Single";
                roomPrice = 100;
                break;
            case 'D': case 'd':
                roomTypeStr = "Double";
                roomPrice = 150;
                break;
            case 'P': case 'p':
                roomTypeStr = "Penthouse";
                roomPrice = 300;
                break;
            case 'T': case 't':
                roomTypeStr = "Suite";
                roomPrice = 200;
                break;
            default:
                cout << "Invalid room type selected. Please try again." << endl;
                continue;  // Skip to the next iteration of the loop
            }


            // Ask how many days they want to stay
            int days;
            cout << "How many days would you like to stay? ";
            cin >> days;

            // Calculate total cost
            int totalCost = roomPrice * days;

            // Display the selected room type and price
            cout << "You have selected " << roomTypeStr << " (RM " << roomPrice << " per night)." << endl;
            cout << "Total cost for " << days << " days: RM " << totalCost << endl;

            // Attempt to reserve the room based on the selected type
            if (!hotel.reserveRoomByType(roomTypeStr, guestName, guestPhone, guestList)) {
                cout << "Reservation failed. Try again later." << endl;
            }
            else {
                cout << "Reservation successful!" << endl;
            }

            // Ask if the user wants to make another reservation
            cout << "\nDo you want to make another reservation? (y/n): ";
            cin >> anotherReservation;
            //} while (anotherReservation == 'y' || anotherReservation == 'Y');

            // Save the updated guest list to the file after reservation(s)
            guestList.saveToFile("iFile.txt");

            break;
        }


        case 3:
            cout << "Exiting program..." << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
/*// Ask user for room type
            cout << "Enter room type (S for Single, D for Double, P for Penthouse, T for Suite): ";
            cin >> roomType;

            // Handle room type input
            string roomTypeStr;
            switch (roomType) {
            case 'S': case 's':
                roomTypeStr = "Single";
                break;
            case 'D': case 'd':
                roomTypeStr = "Double";
                break;
            case 'P': case 'p':
                roomTypeStr = "Penthouse";
                break;
            case 'T': case 't':
                roomTypeStr = "Suite";
                break;
            default:
                cout << "Invalid room type selected. Please try again." << endl;
                continue;  // Skip to the next iteration of the loop
            }*/