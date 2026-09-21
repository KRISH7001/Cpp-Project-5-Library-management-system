📁 Project Overview

This project contains a simple C++ program that demonstrates object-oriented programming concepts such as abstraction, encapsulation, inheritance, polymorphism, dynamic memory management, and exception handling.

The program included is:

Library Management System – Add, catalog, search, check out, and return different types of library items through a menu-driven console interface.
📌 Program Included
Library Management System

File: LibraryManagementSystem.cpp

This program allows the user to:

Add different types of library items to the catalog
View all items in the catalog
Search for an item by title
Check out an item
Return an item
Remove an item from the catalog
Use a menu-driven console interface

The item types included are:

Book
DVD
Magazine
🔧 C++ Concepts Used

The program demonstrates:

An abstract LibraryItem base class with pure virtual functions
Derived classes overriding base-class behavior
Virtual functions and override for polymorphism
Encapsulated private data members with public getters/setters
Dynamic memory management (new / delete)
A Library class for managing an array of LibraryItem*
Custom exception classes plus a general catch (const exception&) handler
Menu-driven programming using switch
User input using cin and getline
🏗️ Class Hierarchy
text
LibraryItem (abstract)
│
├── Book
├── DVD
└── Magazine
🔒 Encapsulation

Each LibraryItem keeps its core data private and exposes it only through getters and setters:

cpp
string title;
string author;
string dueDate;
cpp
getTitle();      setTitle(newTitle);
getAuthor();     setAuthor(newAuthor);
getDueDate();    setDueDate(newDueDate);

This demonstrates restricting direct access to an object's data and validating changes through controlled methods.

🧩 Polymorphism in Action

The Library class stores items as base-class pointers and calls the same method on every item, letting each derived class run its own version:

cpp
LibraryItem* libraryItems[MAX_ITEMS];
...
libraryItems[i]->checkOut();
libraryItems[i]->displayDetails();

A Book, a DVD, and a Magazine each respond differently to the exact same call.

⚠️ Exception Handling

Invalid input is caught with dedicated exception types:

cpp
InvalidInputException      // e.g. negative quantity, bad ISBN format
ItemNotAvailableException  // e.g. checking out an item already checked out
ItemNotFoundException      // e.g. searching for an ID that doesn't exist

All are wrapped up by a general handler:

cpp
catch (const exception& e) {
    cout << "An unexpected error occurred: " << e.what() << endl;
}
🛠️ Requirements

You can run this program using:

GCC / G++ Compiler
Visual Studio Code
Build & Run
bash
g++ -std=c++17 -Wall -o lms LibraryManagementSystem.cpp
./lms
🎯 Learning Objectives

This project helps practice:

Basic C++ syntax
Classes, constructors, and destructors
Abstraction using abstract base classes and pure virtual functions
Encapsulation with private members and public accessors
Inheritance
Runtime polymorphism
Dynamic memory management using new and delete
Custom and standard exception handling
Input and output using cin, cout, and getline
Menu-driven program design
Basic problem solving
📂 Project Structure
text
Library-Management-System-Project/
│
├── README.md
└── LibraryManagementSystem.cpp
👨‍💻 Author
KRISH SAPARIYA
