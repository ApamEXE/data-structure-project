//HotelReservation.h
#ifndef RESERVATION_H
#define RESERVATION_H

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <queue> // Include the queue header

using namespace std;

// Class to hold guest information
class Guest {
private:
    string name;
    string phone;
    int roomNumber;
    string roomType;
    string rtime;

public:
    Guest() : roomNumber(0) {}
    Guest(string name, string phone, int roomNumber, string roomType,
        string rtime)
        : name(name), phone(phone), roomNumber(roomNumber), roomType(roomType),
        rtime(rtime) {}

    string getName() const { return name; }
    string getPhone() const { return phone; }
    int getRoomNumber() const { return roomNumber; }
    string getRoomType() const { return roomType; }
    string getRTime() const { return rtime; }

    void display() const;
    string toString() const;
};

// Node structure for storing guest info in linked list
struct Node {
    Guest data;
    Node* next;
    Node(Guest data) : data(data), next(nullptr) {}
};

// Class to manage list of guests
class GuestList {
private:
    Node* head;

public:
    GuestList() : head(nullptr) {}
    ~GuestList();
    void addGuest(const Guest& guest);
    void removeGuest(int roomNumber);
    void displayAll() const;
    void saveToFile(const string& OFile) const;
    void loadFromFile(const string& iFile);
    bool isEmpty() const;
    // Method to return all guests as a vector
    std::vector<Guest> getAllGuests() const {
        std::vector<Guest> guests;
        Node* current = head;
        while (current != nullptr) {
            guests.push_back(current->data);
            current = current->next;
        }
        return guests;
    }
    Guest* searchGuest(const string& searchTerm);
};

// Class to represent room info
class Room {
private:
    int roomNumber;
    string type;
    bool isAvailable;
    string guestName;
    string time;

public:
    Room(string type, int roomNumber)
        : roomNumber(roomNumber), type(type), isAvailable(true), guestName("") {}

    int getRoomNumber() const { return roomNumber; }
    string getType() const { return type; }
    bool checkAvailability() const { return isAvailable; }
    void setAvailability(bool availability) { isAvailable = availability; }
    void setGuestName(string name) { guestName = name; }
    string getGuestName() const { return guestName; }
    void setTime(const string& t) { time = t; }
    string getTime() const { return time; }

    void display() const;
};

// Class to represent hotel
class Hotel {
private:
    Room* rooms[10];
    queue<Guest> reservationQueue;  // Queue for managing reservations
    Node* waitingListHead = nullptr;
    Node* waitingListTail = nullptr;

public:
    Hotel();
    ~Hotel();
    void initializeRooms();
    void displayAvailableRoomsByType(GuestList& guestList);
    void updateRoomAvailability(int roomNumber, bool isAvailable, GuestList& guestList);
    void reserveRoom(const Guest& guest);
    void processReservation();
    void processWaitingList();
    void enqueueWaitingList(const Guest& guest);  // Method to add guests to the waiting list



    // Getter functions
    Room* getRoom(int index) const { return rooms[index]; }
    int getRoomCount() const {
        return 10;
    }
};



// Class for admin login
class Admin {
private:
    string username = "pizah";
    string password = "Hafizah123";

public:
    Admin() = default;  // Default constructor to initialize with default values
    Admin(const string& user, const string& pass) : username(user), password(pass) {}  // Parameterized constructor
    bool login(const string& user, const string& pass) const;

};

// Function to handle guest reservation and price calculation
void processReservationOrCalculatePrice(Hotel& hotel, GuestList& guestList,
    char roomType, int days,
    bool isReservation, string rtime);

#endif