#include <iostream>
#include "Reservation.h"
#include <fstream>
#include <iomanip>

using namespace std;

// Guest class to display guest information
void Guest::display() const {
    cout << "Name: " << name << ", Phone: " << phone << ", Room Number: " << roomNumber << ", Room Type: " << roomType << endl;
}

string Guest::toString() const {
    return "Name: " + name + ", Phone: " + phone + ", Room Number: " + to_string(roomNumber) + ", Room Type: " + roomType;
}

// GuestList destructor to clean up dynamically allocated memory
GuestList::~GuestList() {
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
}

// Add a guest to the guest list only if not already present
void GuestList::addGuest(const Guest& guest) {
    Node* current = head;

    // Check for duplication
    while (current != nullptr) {
        if (current->data.getRoomNumber() == guest.getRoomNumber()) {
            cout << "Guest for room " << guest.getRoomNumber() << " is already in the list." << endl;
            return;
        }
        current = current->next;
    }

    // Add new guest
    Node* newNode = new Node(guest);
    newNode->next = head;
    head = newNode;
}


// Remove a guest from the guest list by room number
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

// Display all guests in the guest list
void GuestList::displayAll() const {
    if (isEmpty()) {
        cout << "No guests in the reservation list." << endl;
        return;
    }

    Node* current = head;
    cout << "\n--- Guest Reservation List ---" << endl;
    cout << "---------------------------------------------" << endl;
    cout << "| Name            | Phone         | Room No |" << endl;
    cout << "---------------------------------------------" << endl;

    while (current != nullptr) {
        const Guest& guest = current->data;
        cout << "| " << setw(15) << left << guest.getName()
            << "| " << setw(13) << left << guest.getPhone()
            << "| " << setw(8) << left << guest.getRoomNumber() << "|" << endl;
        current = current->next;
    }

    cout << "---------------------------------------------" << endl;
}


// Find a guest by room number
bool GuestList::findGuest(int roomNumber, Guest& guest) const {
    Node* current = head;
    while (current != nullptr) {
        if (current->data.getRoomNumber() == roomNumber) {
            guest = current->data;
            return true;
        }
        current = current->next;
    }
    return false;
}

// Save guest list to file
void GuestList::saveToFile(const string& OFile) const {
    ofstream outFile(OFile);
    if (!outFile) {
        cout << "Error opening file for saving data." << endl;
        return;
    }

    Node* current = head;
    while (current != nullptr) {
        outFile << current->data.getName() << " "
            << current->data.getPhone() << " "
            << current->data.getRoomNumber() << " "
            << current->data.getRoomType() << endl;
        current = current->next;
    }

    outFile.close();
}

// Check if the guest list is empty
bool GuestList::isEmpty() const {
    return head == nullptr;
}

// Load guest list from file
void GuestList::loadFromFile(const string& iFile) {
    ifstream inFile(iFile);
    if (!inFile) {
        cout << "Error opening file for loading data." << endl;
        return;
    }
    string name, phone, roomType;
    int roomNumber;

    while (inFile >> name >> phone >> roomNumber >> roomType) {
        addGuest(Guest(name, phone, roomNumber, roomType));
    }

    inFile.close();
}

// Room class to display room details
void Room::display() const {
    cout << "Room Number: " << roomNumber << ", Type: " << type
        << ", Available: " << (isAvailable ? "Yes" : "No") << endl;
}

void Room::setGuestName(const string& name) {
    guestName = name;  // Set the guest name in the Room object
}

// Admin class for login functionality
bool Admin::login(const string& user, const string& pass) const {
    return user == username && pass == password;
}

// Hotel constructor: initialize rooms
Hotel::Hotel() {
    for (int i = 0; i < 10; ++i) {
        rooms[i] = nullptr;
    }
    initializeRooms();
}

// Hotel destructor: clean up rooms
Hotel::~Hotel() {
    for (int i = 0; i < 10; ++i) {
        delete rooms[i];
    }
}

// Initialize rooms in the hotel
void Hotel::initializeRooms() {
    string roomTypes[] = { "Single", "Double", "Suite", "Penthouse", "Single", "Double", "Suite", "Single", "Double", "Penthouse" };

    for (int i = 0; i < 10; ++i) {
        rooms[i] = new Room(roomTypes[i], i + 1);  // Initialize rooms with type and room number
    }
}

// Display available rooms by type
void Hotel::displayAvailableRoomsByType() const {
    cout << "\n--- Available Rooms ---" << endl;
    cout << "----------------------------------------------------------------" << endl;
    cout << "| Room Number | Room Type      | Available  |   Guest Name     |" << endl;
    cout << "----------------------------------------------------------------" << endl;

    bool foundAvailable = false;

    for (int i = 0; i < 10; ++i) {
        string availability = rooms[i]->checkAvailability() ? "Yes" : "No";
        string guestName = rooms[i]->checkAvailability() ? "No guest" : rooms[i]->getGuestName();

        cout << "| " << setw(11) << left << rooms[i]->getRoomNumber() << " | "
            << setw(14) << left << rooms[i]->getType() << " | "
            << setw(10) << left << availability << " |"
            << setw(18) << left << guestName << " |" << endl;
        foundAvailable = true;
    }

    if (!foundAvailable) {
        cout << "| No available rooms to display.                              |" << endl;
    }

    cout << "-----------------------------------------------------------------" << endl;
}

// Reserve room by type and add guest to the guest list
bool Hotel::reserveRoomByType(const string& roomType, string guestName, string guestPhone, GuestList& guestList) {
    for (int i = 0; i < 10; ++i) {
        if (rooms[i]->checkAvailability() && rooms[i]->getType() == roomType) {
            // Mark room as reserved
            rooms[i]->setAvailability(false);
            rooms[i]->setGuestName(guestName);

            // Ensure no duplicate guest is added
            Guest guest(guestName, guestPhone, rooms[i]->getRoomNumber(), roomType);
            guestList.addGuest(guest);

            cout << "Room " << rooms[i]->getRoomNumber() << " reserved for " << guestName << endl;
            return true;
        }
    }
    cout << "No available rooms of type " << roomType << " found." << endl;
    return false;
}


// Update room availability
void Hotel::updateRoomAvailability(int roomNumber, bool isAvailable) {
    for (int i = 0; i < 10; ++i) {
        if (rooms[i]->getRoomNumber() == roomNumber) {
            rooms[i]->setAvailability(isAvailable);
            if (isAvailable) {
                rooms[i]->setGuestName("");  // Clear the guest's name
                cout << "Room " << roomNumber << " is now available." << endl;
            }
            return;
        }
    }
    cout << "Room number " << roomNumber << " not found." << endl;
}
