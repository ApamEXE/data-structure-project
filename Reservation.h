#ifndef RESERVATION_H
#define RESERVATION_H

#include <iostream>
#include <string>

using namespace std;

// Guest class to represent guest details
class Guest {
private:
    string name;
    string phone;
    int roomNumber;
    string roomType;

public:
    Guest(string n = "", string p = "", int r = 0, string t = "")
        : name(n), phone(p), roomNumber(r), roomType(t) {}

    void display() const;
    string toString() const;

    // Getters for file I/O
    string getName() const { return name; }
    string getPhone() const { return phone; }
    int getRoomNumber() const { return roomNumber; }
    string getRoomType() const { return roomType; }
};

// Node structure for linked list
struct Node {
    Guest data;
    Node* next;

    Node(const Guest& guest) : data(guest), next(nullptr) {}
};

// GuestList class to manage list of guests
class GuestList {
private:
    Node* head;

public:

    GuestList() : head(nullptr) {}
    ~GuestList();
    void addGuest(const Guest& guest);
    void removeGuest(int roomNumber);
    void displayAll() const;
    bool findGuest(int roomNumber, Guest& guest) const;
    void saveToFile(const string& OFile) const;
    void loadFromFile(const string& iFile);
    bool isEmpty() const;

};

// Room class to represent room details
class Room {
private:
    string type;
    int roomNumber;
    bool isAvailable;
    string guestName;


public:
    Room(string t = "", int r = 0, bool avail = true)
        : type(t), roomNumber(r), isAvailable(avail) {}

    void display() const;
    bool checkAvailability() const { return isAvailable; }
    void setAvailability(bool avail) { isAvailable = avail; }
    int getRoomNumber() const { return roomNumber; }
    string getType() const { return type; }
    string getGuestName() const { return guestName; }
    void setGuestName(const string& name);
};

// Admin class for managing the admin login
class Admin {
private:
    string username = "pizah";
    string password = "Hafizah123";

public:
    bool login(const string& user, const string& pass) const;
};




// Hotel class to manage rooms and reservations
class Hotel {
private:
    Room* rooms[10];  // Array of room pointers (no vector)

public:
    Hotel();
    ~Hotel();
    void initializeRooms();
    void saveRoomsToFile(const string& iFile) const;
    void displayAvailableRoomsByType() const;
    bool reserveRoomByType(const string& roomType, string guestName, string guestPhone, GuestList& guestList);
    void updateRoomAvailability(int roomNumber, bool isAvailable);
};

#endif