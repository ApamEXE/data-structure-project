//Reservation.cpp
#define _CRT_SECURE_NO_WARNINGS
#include "HotelReservation.h"
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

// Guest class methods
void Guest::display() const {
    cout << "Name: " << name << ", Phone: " << phone
        << ", Room Number: " << roomNumber << ", Room Type: " << roomType
        << ", Time of reservation: " << rtime << endl;
}

string Guest::toString() const {
    return "Name: " + name + ", Phone: " + phone +
        ", Room Number: " + to_string(roomNumber) +
        ", Room Type: " + roomType + ", Time of reservation: " + rtime;
}

//NUR HAFIZAH
// GuestList class methods
GuestList::~GuestList() {
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
}


//NUR HAFIZAH
void GuestList::addGuest(const Guest& guest) {
    // Check if the guest already exists in the list (based on room number)
    Node* current = head;
    while (current != nullptr) {
        if (current->data.getRoomNumber() == guest.getRoomNumber()) {
            // Guest with the same room number already exists, so return
            return;
        }
        current = current->next;
    }

    // If no duplicate found, add the new guest to the list
    Node* newNode = new Node(guest);
    newNode->next = head;
    head = newNode;
}

//NUR HAFIZAH
void GuestList::removeGuest(int roomNumber) {
    Node* current = head;
    Node* prev = nullptr;

    while (current != nullptr) {
        if (current->data.getRoomNumber() == roomNumber) {
            if (prev == nullptr) {
                head = current->next;
            }
            else {
                prev->next = current->next;
            }
            delete current;
            return;
        }
        prev = current;
        current = current->next;
    }
    cout << "Guest with room number " << roomNumber << " not found." << endl;
}

//HAFIZAH & NAFIZ
// Display all guests in the guest list
void GuestList::displayAll() const {
    if (isEmpty()) {
        cout << "No guests in the reservation list." << endl;
        return;
    }
    Node* current = head;
    cout << "\n--- Guest Reservation List ---" << endl;
    cout << "-----------------------------------------------------------------------"
        << endl;
    cout << "| Name           | Phone        | Room No |   Time of reservation     |"
        << endl;
    cout << "-----------------------------------------------------------------------"
        << endl;

    while (current != nullptr) {
        const Guest& guest = current->data;
        string rtime = guest.getRTime();
        cout << "| " << setw(15) << left << guest.getName() << "| " << setw(13)
            << left << guest.getPhone() << "| " << setw(8) << left
            << guest.getRoomNumber() << "| " << setw(26) << left
            << rtime.substr(0, rtime.length() - 1) << "|" << endl;
        current = current->next;
    }

    cout << "-----------------------------------------------------------------------"
        << endl;
}

//NUR HAFIZAH
void GuestList::saveToFile(const string& OFile) const {
    ofstream outFile(OFile);
    if (!outFile) {
        cout << "Error opening file: " << OFile << endl;
        return;
    }
    Node* current = head;
    while (current != nullptr) {
        outFile << current->data.getName() << " " << current->data.getPhone() << " "
            << current->data.getRoomNumber() << " "
            << current->data.getRoomType() << " " << current->data.getRTime()
            << endl;
        current = current->next;
    }
}

////NUR HAFIZAH
bool GuestList::isEmpty() const { return head == nullptr; }

//NUR HAFIZAH
void GuestList::loadFromFile(const string& filename) {
    ifstream inFile(filename);
    if (!inFile) {
        cout << "Error opening file: " << filename << endl;
        return;
    }

    string name, phone, roomType;
    int roomNumber;
    string rtime;

    while (inFile >> name >> phone >> roomNumber >> roomType >> rtime) {
        addGuest(Guest(name, phone, roomNumber, roomType, rtime));
    }

    inFile.close();
}

// Room class methods
void Room::display() const {
    cout << "Room Number: " << roomNumber << ", Type: " << type
        << ", Available: " << (isAvailable ? "Yes" : "No") << endl;
}

//NUR HAFIZAH
// Admin class methods
bool Admin::login(const string& user, const string& pass) const {
    return user == username && pass == password;
}

// Hotel class methods
Hotel::Hotel() { initializeRooms(); }

Hotel::~Hotel() {
    for (int i = 0; i < 10; ++i) {
        delete rooms[i];
    }
}

//NUR HAFIZAH
void Hotel::initializeRooms() {
    string roomTypes[] = { "Single", "Double", "Suite",  "Penthouse", "Single",
                          "Double", "Suite",  "Single", "Double",    "Penthouse" };
    for (int i = 0; i < 10; ++i) {
        rooms[i] = new Room(roomTypes[i], i + 1);
    }
}

//NUR HAFIZAH
void Hotel::displayAvailableRoomsByType(GuestList& guestList) {
    // Load guests from the file
    guestList.loadFromFile("reservations.txt");

    // Update room availability based on the guest list
    auto guests = guestList.getAllGuests(); // Get all guests as a vector

    for (int i = 0; i < 10; ++i) {
        bool isOccupied = false;
        for (const auto& guest : guests) {
            if (rooms[i]->getRoomNumber() == guest.getRoomNumber()) {
                rooms[i]->setAvailability(false);        // Room is not available
                rooms[i]->setGuestName(guest.getName()); // Set guest name
                rooms[i]->setTime(guest.getRTime());     // Set reservation time
                isOccupied = true;
                break; // No need to check other guests for this room
            }
        }
        if (!isOccupied) {
            rooms[i]->setAvailability(true); // Room is available if no guest is found
            rooms[i]->setGuestName("");      // Clear guest name
            rooms[i]->setTime("");           // Clear reservation time
        }
    }

    // Now display the available rooms
    cout << "\n--- Available Rooms ---" << endl;
    cout << "-----------------------------------------------------------------------------------------------------------------------"
        << endl;
    cout << "| Room Number |    Room Type   | Available  |       Guest Name      |      Phone Number     |   Time of Reservation   |"
        << endl;
    cout << "-----------------------------------------------------------------------------------------------------------------------"
        << endl;

    bool foundAvailable = false;

    for (int i = 0; i < 10; ++i) {

        string availability = rooms[i]->checkAvailability() ? "Yes" : "No";
        string guestName = "No guest";
        string phoneNumber = "N/A";
        string rtime = "N/A";

        if (!rooms[i]->checkAvailability()) {
            // Find the guest occupying this room
            for (const auto& guest : guests) {
                if (rooms[i]->getRoomNumber() == guest.getRoomNumber()) {
                    guestName = guest.getName();
                    phoneNumber = guest.getPhone();
                    rtime = guest.getRTime();
                    break;
                }
            }
        }

        // Ensure all data is consistently formatted and prevents blank rows
        cout << "| " << setw(12) << left << rooms[i]->getRoomNumber() << "| "
            << setw(15) << left << rooms[i]->getType() << "| " << setw(11) << left << availability << "| "
            << setw(22) << left << guestName << "| " << setw(22) << left << phoneNumber << "| "
            << setw(23) << left << (rtime == "N/A" ? rtime : rtime.substr(0, rtime.length() - 1)) << " | " << endl;
        foundAvailable = true;
    }

    if (!foundAvailable) {
        cout << "| No available rooms to display.                                                                             |"
            << endl;
    }

    cout << "-----------------------------------------------------------------------------------------------------------------------"
        << endl;
}

void Hotel::updateRoomAvailability(int roomNumber, bool isAvailable,
    GuestList& guestList) {
    for (int i = 0; i < 10; ++i) {
        if (rooms[i]->getRoomNumber() == roomNumber) {
            rooms[i]->setAvailability(isAvailable);
            if (!isAvailable) {
                // When setting to not available, ask for guest details
                string guestName, phoneNumber;
                cout << "Enter guest name for room " << roomNumber << ": ";
                cin.ignore(); // Clear the newline character from the input buffer
                getline(cin, guestName);
                cout << "Enter guest phone number for room " << roomNumber << ": ";
                getline(cin, phoneNumber);
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

                rooms[i]->setGuestName(guestName);
                guestList.addGuest(Guest(guestName, phoneNumber, roomNumber,
                    rooms[i]->getType(), rtime));
            }
            else {
                // When setting to available, remove the guest from the guest list
                string guestName = rooms[i]->getGuestName();
                if (!guestName.empty()) {
                    guestList.removeGuest(roomNumber); // Remove guest by room number
                }
                rooms[i]->setGuestName(""); // Clear the guest name
            }
            // Save the updated guest list to a file
            guestList.saveToFile("reservations.txt"); // Specify the filename you want
        }
    }
}

//NUR HAFIZAH
// Reserve a room for a guest by adding them to the queue
void Hotel::reserveRoom(const Guest& guest) {
    reservationQueue.push(guest);  // Add guest to reservation queue
    cout << guest.getName() << " has been added to the reservation queue." << endl;
}

//NUR HAFIZAH
// Process the next reservation (assign a room if available)
void Hotel::processReservation() {
    if (!reservationQueue.empty()) {
        Guest guest = reservationQueue.front();
        reservationQueue.pop();  // Remove the guest from the queue

        // Assign the first available room
        for (int i = 0; i < 10; ++i) {
            if (rooms[i]->checkAvailability()) {
                rooms[i]->setAvailability(false);
                rooms[i]->setGuestName(guest.getName());
                rooms[i]->setTime(guest.getRTime());
                cout << "Room " << rooms[i]->getRoomNumber() << " assigned to " << guest.getName() << endl;
                break;
            }
        }
    }
    else {
        cout << "No reservations in the queue." << endl;
    }
}

//NUR HAFIZAH
// Method to enqueue a guest to the waiting list when no rooms are available
void Hotel::enqueueWaitingList(const Guest& guest) {
    Node* newNode = new Node(guest);  // Create a new node for the guest
    if (waitingListTail == nullptr) {
        waitingListHead = waitingListTail = newNode;  // If the list is empty, set both head and tail
    }
    else {
        waitingListTail->next = newNode;  // Add the new node at the tail of the list
        waitingListTail = newNode;
    }
    cout << guest.getName() << " has been added to the waiting list." << endl;
}

//NUR HAFIZAH
// Process the waiting list by assigning rooms to guests when available
void Hotel::processWaitingList() {
    if (waitingListHead != nullptr) {
        // Try to assign rooms to waiting guests
        Node* current = waitingListHead;
        while (current != nullptr) {
            Guest guest = current->data;  // Get the guest from the node

            // Try to assign the guest to an available room
            for (int i = 0; i < 10; ++i) {
                if (rooms[i]->checkAvailability()) {
                    rooms[i]->setAvailability(false);
                    rooms[i]->setGuestName(guest.getName());
                    rooms[i]->setTime(guest.getRTime());
                    cout << "Room " << rooms[i]->getRoomNumber() << " assigned to waiting guest: " << guest.getName() << endl;

                    // Remove the guest from the waiting list
                    if (current == waitingListHead) {
                        waitingListHead = current->next;  // If it's the first guest, move the head
                        if (waitingListHead == nullptr) {
                            waitingListTail = nullptr;  // If the list is now empty, reset the tail
                        }
                    }
                    else {
                        Node* prev = waitingListHead;
                        while (prev->next != current) {
                            prev = prev->next;  // Find the previous node
                        }
                        prev->next = current->next;  // Remove the current node from the list
                        if (current == waitingListTail) {
                            waitingListTail = prev;  // Update the tail if needed
                        }
                    }
                    delete current;  // Free memory
                    break;
                }
            }
            current = current->next;  // Move to the next guest in the list
        }
    }
    else {
        cout << "No guests in the waiting list." << endl;
    }
};

//AKIF
void processReservationOrCalculatePrice(Hotel& hotel, GuestList& guestList,
    char roomType, int days, bool isReservation, string rtime) {
    int roomNumber = -1;
    double total = 0;
    bool roomAvailable = false;

    // Find an available room
    for (int i = 0; i < hotel.getRoomCount(); ++i) {
        // Check if the room type matches the input character
        if ((roomType == 'S' && hotel.getRoom(i)->getType() == "Single") ||
            (roomType == 'D' && hotel.getRoom(i)->getType() == "Double") ||
            (roomType == 'T' && hotel.getRoom(i)->getType() == "Suite") ||
            (roomType == 'P' && hotel.getRoom(i)->getType() == "Penthouse")) {

            // Check if the room is available
            if (hotel.getRoom(i)->checkAvailability()) {
                roomNumber = hotel.getRoom(i)->getRoomNumber();
                roomAvailable = true;
                break;
            }
        }
    }

    if (roomAvailable) {
        // Room is available, proceed with the reservation or price calculation
        if (hotel.getRoom(roomNumber - 1)->getType() == "Single")
            total = 100 * days;
        else if (hotel.getRoom(roomNumber - 1)->getType() == "Double")
            total = 150 * days;
        else if (hotel.getRoom(roomNumber - 1)->getType() == "Suite")
            total = 200 * days;
        else if (hotel.getRoom(roomNumber - 1)->getType() == "Penthouse")
            total = 500 * days;

        if (isReservation) {
            string guestName, phoneNumber;
            cout << "Enter guest name: ";
            cin.ignore(); // Clear the newline character from the input buffer
            getline(cin, guestName);
            cout << "Enter phone number: ";
            getline(cin, phoneNumber);

            // Create a new guest object
            Guest newGuest(guestName, phoneNumber, roomNumber,
                hotel.getRoom(roomNumber - 1)->getType(), rtime);


            // Mark the room as unavailable
            hotel.getRoom(roomNumber - 1)->setAvailability(false);
            hotel.getRoom(roomNumber - 1)->setGuestName(guestName);

            cout << "Room reserved successfully!" << endl;
            cout << "The total price for " << days << " days is RM " << total << endl;
            cout << "Time of reservation: " << rtime << endl;

            // Save the updated guest list to a file
            guestList.saveToFile("reservations.txt");  // Specify the filename you want
        }
        else {
            // Just calculate and show the price without making a reservation
            cout << "The total price for " << days << " days is RM " << total << endl;
        }
    }
    else {
        // Room is unavailable, add reservation to the waiting queue
        string guestName, phoneNumber;
        cout << "Room is unavailable. Would you like to be added to the waiting list? (yes/no): ";
        string response;
        cin >> response;
        cin.ignore();

        if (response == "yes" || response == "y") {
            // Clear the newline character from the input buffer

            cout << "Enter guest name: ";
            getline(cin, guestName);
            cout << "Enter phone number: ";
            getline(cin, phoneNumber);
            time_t now = time(0);
            string rtime = ctime(&now);

            // Create a new guest and enqueue to waiting list
            Guest newGuest(guestName, phoneNumber, -1, "Waiting", rtime);  // Room number -1 signifies no room yet
            hotel.enqueueWaitingList(newGuest);
        }
        else {
            cout << "No reservation made. The room is unavailable at the moment." << endl;
        }
    }
}

//AKIF
Guest* GuestList::searchGuest(const string& searchTerm) {
    Node* current = head;
    while (current != nullptr) {
        if (current->data.getName() == searchTerm ||
            current->data.getPhone() == searchTerm ||
            to_string(current->data.getRoomNumber()) == searchTerm) {
            return &current->data; // Return the found guest
        }
        current = current->next;
    }
    return nullptr; // Return nullptr if not found
}

// Extend Reservation functionality
void extendReservation(Hotel& hotel, GuestList& guestList, int roomNumber, int additionalDays) {
    Guest* guest = guestList.searchGuest(to_string(roomNumber));
    if (guest != nullptr) {
        time_t now = time(0);
        tm* localTime = localtime(&now);

        string rtime =
            (localTime->tm_mon + 1 < 10 ? "0" : "") + to_string(localTime->tm_mon + 1) + "/" +  // Month
            (localTime->tm_mday < 10 ? "0" : "") + to_string(localTime->tm_mday) + "/" +        // Day
            to_string(localTime->tm_year + 1900) + "-" +                                       // Year
            (localTime->tm_hour < 10 ? "0" : "") + to_string(localTime->tm_hour) + ":" +       // Hour
            (localTime->tm_min < 10 ? "0" : "") + to_string(localTime->tm_min) + ":" +        // Minute
            (localTime->tm_sec < 10 ? "0" : "") + to_string(localTime->tm_sec);               // Second

        processReservationOrCalculatePrice(hotel, guestList, guest->getRoomType()[0], additionalDays, true, rtime);
        cout << "Reservation extended for " << additionalDays << " days." << endl;
    }
    else {
        cout << "No reservation found for room number " << roomNumber << endl;
    }
}