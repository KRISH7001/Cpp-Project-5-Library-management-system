#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>
#include <iomanip>
 
using namespace std;
 
const int MAX_ITEMS = 100;
 
// ------------------------------------------------------------
// Custom Exception Classes
// ------------------------------------------------------------
class InvalidInputException : public runtime_error {
public:
    explicit InvalidInputException(const string& msg) : runtime_error(msg) {}
};
 
class ItemNotAvailableException : public runtime_error {
public:
    explicit ItemNotAvailableException(const string& msg) : runtime_error(msg) {}
};
 
class ItemNotFoundException : public runtime_error {
public:
    explicit ItemNotFoundException(const string& msg) : runtime_error(msg) {}
};
 
// ------------------------------------------------------------
// Utility: very small ISBN format validator
// A valid ISBN here is assumed as 13 digits, optionally with hyphens,
// e.g. 978-0-13-468599-1  OR  9780134685991
// ------------------------------------------------------------
bool isValidISBN(const string& isbn) {
    int digitCount = 0;
    for (char c : isbn) {
        if (isdigit(static_cast<unsigned char>(c))) {
            digitCount++;
        } else if (c != '-') {
            return false; // any character other than digit/hyphen is invalid
        }
    }
    return digitCount == 13;
}
 
// ==============================================================
//  Abstract Base Class : LibraryItem
// ==============================================================
class LibraryItem {
private:
    // ---- Encapsulated (private) data members ----
    string title;
    string author;
    string dueDate;
    bool checkedOut;
 
protected:
    string itemID;
 
public:
    LibraryItem(const string& id, const string& t, const string& a)
        : title(t), author(a), dueDate("N/A"), checkedOut(false), itemID(id) {
        if (t.empty() || a.empty()) {
            throw InvalidInputException("Title and Author cannot be empty.");
        }
    }
 
    virtual ~LibraryItem() {
        cout << "[Memory released] Item \"" << title << "\" destroyed." << endl;
    }
 
    // ---- Public getters ----
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getDueDate() const { return dueDate; }
    string getID() const { return itemID; }
    bool isCheckedOut() const { return checkedOut; }
 
    // ---- Public setters (with basic validation) ----
    void setTitle(const string& newTitle) {
        if (newTitle.empty()) throw InvalidInputException("Title cannot be empty.");
        title = newTitle;
    }
    void setAuthor(const string& newAuthor) {
        if (newAuthor.empty()) throw InvalidInputException("Author cannot be empty.");
        author = newAuthor;
    }
    void setDueDate(const string& newDueDate) {
        dueDate = newDueDate;
    }
    void setCheckedOutStatus(bool status) { checkedOut = status; }
 
    // ---- Pure virtual functions (Abstraction) ----
    virtual void checkOut() = 0;
    virtual void returnItem() = 0;
    virtual void displayDetails() const = 0;
 
    // Common helper reused by derived classes
    virtual string itemType() const = 0;
};
 
// ==============================================================
//  Derived Class : Book
// ==============================================================
class Book : public LibraryItem {
private:
    string isbn;
    int quantity;
 
public:
    Book(const string& id, const string& t, const string& a,
         const string& isbnNum, int qty)
        : LibraryItem(id, t, a), isbn(isbnNum), quantity(qty) {
 
        if (qty < 0) {
            throw InvalidInputException("Book quantity cannot be negative.");
        }
        if (!isValidISBN(isbnNum)) {
            throw InvalidInputException("Invalid ISBN format for book: " + t);
        }
    }
 
    int getQuantity() const { return quantity; }
    string getISBN() const { return isbn; }
 
    void checkOut() override {
        if (isCheckedOut() || quantity <= 0) {
            throw ItemNotAvailableException("Book \"" + getTitle() + "\" is not available for checkout.");
        }
        quantity--;
        if (quantity == 0) setCheckedOutStatus(true);
        setDueDate("14 days from today");
        cout << "Book \"" << getTitle() << "\" checked out successfully. "
             << "Remaining copies: " << quantity << endl;
    }
 
    void returnItem() override {
        quantity++;
        setCheckedOutStatus(false);
        setDueDate("N/A");
        cout << "Book \"" << getTitle() << "\" returned successfully. "
             << "Total copies now: " << quantity << endl;
    }
 
    void displayDetails() const override {
        cout << "----------------------------------------\n";
        cout << "Type        : Book\n";
        cout << "ID          : " << getID() << "\n";
        cout << "Title       : " << getTitle() << "\n";
        cout << "Author      : " << getAuthor() << "\n";
        cout << "ISBN        : " << isbn << "\n";
        cout << "Quantity    : " << quantity << "\n";
        cout << "Due Date    : " << getDueDate() << "\n";
        cout << "Status      : " << (isCheckedOut() ? "Fully Checked Out" : "Available") << "\n";
    }
 
    string itemType() const override { return "Book"; }
};
 
// ==============================================================
//  Derived Class : DVD
// ==============================================================
class DVD : public LibraryItem {
private:
    int durationMinutes;
 
public:
    DVD(const string& id, const string& t, const string& a, int duration)
        : LibraryItem(id, t, a), durationMinutes(duration) {
        if (duration <= 0) {
            throw InvalidInputException("DVD duration must be positive.");
        }
    }
 
    int getDuration() const { return durationMinutes; }
 
    void checkOut() override {
        if (isCheckedOut()) {
            throw ItemNotAvailableException("DVD \"" + getTitle() + "\" is already checked out.");
        }
        setCheckedOutStatus(true);
        setDueDate("7 days from today");
        cout << "DVD \"" << getTitle() << "\" checked out successfully." << endl;
    }
 
    void returnItem() override {
        if (!isCheckedOut()) {
            throw ItemNotFoundException("DVD \"" + getTitle() + "\" was not checked out.");
        }
        setCheckedOutStatus(false);
        setDueDate("N/A");
        cout << "DVD \"" << getTitle() << "\" returned successfully." << endl;
    }
 
    void displayDetails() const override {
        cout << "----------------------------------------\n";
        cout << "Type        : DVD\n";
        cout << "ID          : " << getID() << "\n";
        cout << "Title       : " << getTitle() << "\n";
        cout << "Director/Author : " << getAuthor() << "\n";
        cout << "Duration    : " << durationMinutes << " minutes\n";
        cout << "Due Date    : " << getDueDate() << "\n";
        cout << "Status      : " << (isCheckedOut() ? "Checked Out" : "Available") << "\n";
    }
 
    string itemType() const override { return "DVD"; }
};
 
// ==============================================================
//  Derived Class : Magazine
// ==============================================================
class Magazine : public LibraryItem {
private:
    int issueNumber;
 
public:
    Magazine(const string& id, const string& t, const string& a, int issueNo)
        : LibraryItem(id, t, a), issueNumber(issueNo) {
        if (issueNo <= 0) {
            throw InvalidInputException("Issue number must be positive.");
        }
    }
 
    int getIssueNumber() const { return issueNumber; }
 
    void checkOut() override {
        if (isCheckedOut()) {
            throw ItemNotAvailableException("Magazine \"" + getTitle() + "\" is already checked out.");
        }
        setCheckedOutStatus(true);
        setDueDate("3 days from today");
        cout << "Magazine \"" << getTitle() << "\" checked out successfully." << endl;
    }
 
    void returnItem() override {
        if (!isCheckedOut()) {
            throw ItemNotFoundException("Magazine \"" + getTitle() + "\" was not checked out.");
        }
        setCheckedOutStatus(false);
        setDueDate("N/A");
        cout << "Magazine \"" << getTitle() << "\" returned successfully." << endl;
    }
 
    void displayDetails() const override {
        cout << "----------------------------------------\n";
        cout << "Type        : Magazine\n";
        cout << "ID          : " << getID() << "\n";
        cout << "Title       : " << getTitle() << "\n";
        cout << "Publisher   : " << getAuthor() << "\n";
        cout << "Issue No.   : " << issueNumber << "\n";
        cout << "Due Date    : " << getDueDate() << "\n";
        cout << "Status      : " << (isCheckedOut() ? "Checked Out" : "Available") << "\n";
    }
 
    string itemType() const override { return "Magazine"; }
};
 
// ==============================================================
//  Library class : manages the collection of LibraryItem* (catalog)
// ==============================================================
class Library {
private:
    LibraryItem* libraryItems[MAX_ITEMS];
    int itemCount;
    int nextID;
 
public:
    Library() : itemCount(0), nextID(1) {
        for (int i = 0; i < MAX_ITEMS; i++) libraryItems[i] = nullptr;
    }
 
    ~Library() {
        cout << "\nShutting down library... releasing all dynamic memory.\n";
        for (int i = 0; i < itemCount; i++) {
            delete libraryItems[i];
            libraryItems[i] = nullptr;
        }
    }
 
    string generateID() {
        return "ITM" + to_string(nextID++);
    }
 
    void addItem(LibraryItem* item) {
        if (itemCount >= MAX_ITEMS) {
            delete item; // avoid memory leak if we can't store it
            throw runtime_error("Library catalog is full. Cannot add more items.");
        }
        libraryItems[itemCount++] = item;
        cout << "Item added to catalog with ID: " << item->getID() << endl;
    }
 
    int findItemIndex(const string& id) const {
        for (int i = 0; i < itemCount; i++) {
            if (libraryItems[i]->getID() == id) return i;
        }
        return -1;
    }
 
    LibraryItem* findItem(const string& id) const {
        int idx = findItemIndex(id);
        if (idx == -1) {
            throw ItemNotFoundException("No item found with ID: " + id);
        }
        return libraryItems[idx];
    }
 
    void removeItem(const string& id) {
        int idx = findItemIndex(id);
        if (idx == -1) {
            throw ItemNotFoundException("No item found with ID: " + id);
        }
        delete libraryItems[idx];
        for (int i = idx; i < itemCount - 1; i++) {
            libraryItems[i] = libraryItems[i + 1];
        }
        libraryItems[--itemCount] = nullptr;
        cout << "Item " << id << " removed from catalog." << endl;
    }
 
    // Polymorphism: same call, different behaviour depending on runtime type
    void checkOutItem(const string& id) {
        LibraryItem* item = findItem(id); // may throw ItemNotFoundException
        item->checkOut();                  // may throw ItemNotAvailableException
    }
 
    void returnItem(const string& id) {
        LibraryItem* item = findItem(id);
        item->returnItem();
    }
 
    void searchByTitle(const string& keyword) const {
        bool found = false;
        for (int i = 0; i < itemCount; i++) {
            // simple case-sensitive substring search
            if (libraryItems[i]->getTitle().find(keyword) != string::npos) {
                libraryItems[i]->displayDetails();
                found = true;
            }
        }
        if (!found) {
            cout << "No items found matching \"" << keyword << "\".\n";
        }
    }
 
    // Demonstrates polymorphism: iterating LibraryItem* array,
    // calling displayDetails() which resolves to the correct override.
    void displayAllItems() const {
        if (itemCount == 0) {
            cout << "The catalog is currently empty.\n";
            return;
        }
        cout << "\n========== LIBRARY CATALOG (" << itemCount << " items) ==========\n";
        for (int i = 0; i < itemCount; i++) {
            libraryItems[i]->displayDetails();
        }
        cout << "==========================================\n";
    }
 
    int getItemCount() const { return itemCount; }
};
 
// ------------------------------------------------------------
// Input helpers
// ------------------------------------------------------------
void clearInputStream() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
 
int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            clearInputStream();
            cout << "Invalid input. Please enter a whole number.\n";
            continue;
        }
        clearInputStream();
        return value;
    }
}
 
string readLine(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}
 
// ------------------------------------------------------------
// Menu helpers for adding items
// ------------------------------------------------------------
void addBookFlow(Library& library) {
    try {
        string title = readLine("Enter title: ");
        string author = readLine("Enter author: ");
        string isbn = readLine("Enter ISBN (13 digits, hyphens allowed): ");
        int qty = readInt("Enter quantity: ");
 
        Book* book = new Book(library.generateID(), title, author, isbn, qty);
        library.addItem(book);
    } catch (const InvalidInputException& e) {
        cout << "Error adding book: " << e.what() << endl;
    } catch (const exception& e) {
        cout << "Unexpected error: " << e.what() << endl;
    }
}
 
void addDVDFlow(Library& library) {
    try {
        string title = readLine("Enter title: ");
        string director = readLine("Enter director: ");
        int duration = readInt("Enter duration (minutes): ");
 
        DVD* dvd = new DVD(library.generateID(), title, director, duration);
        library.addItem(dvd);
    } catch (const InvalidInputException& e) {
        cout << "Error adding DVD: " << e.what() << endl;
    } catch (const exception& e) {
        cout << "Unexpected error: " << e.what() << endl;
    }
}
 
void addMagazineFlow(Library& library) {
    try {
        string title = readLine("Enter title: ");
        string publisher = readLine("Enter publisher: ");
        int issueNo = readInt("Enter issue number: ");
 
        Magazine* mag = new Magazine(library.generateID(), title, publisher, issueNo);
        library.addItem(mag);
    } catch (const InvalidInputException& e) {
        cout << "Error adding magazine: " << e.what() << endl;
    } catch (const exception& e) {
        cout << "Unexpected error: " << e.what() << endl;
    }
}
 
void seedSampleData(Library& library) {
    // Pre-populate a few items so the menu has data to demonstrate on first run.
    try {
        library.addItem(new Book(library.generateID(), "The Pragmatic Programmer",
                                  "Andrew Hunt", "978-0-13-595705-9", 3));
        library.addItem(new Book(library.generateID(), "Clean Code",
                                  "Robert C. Martin", "978-0-13-235088-4", 2));
        library.addItem(new DVD(library.generateID(), "Inception",
                                 "Christopher Nolan", 148));
        library.addItem(new Magazine(library.generateID(), "National Geographic",
                                      "NatGeo Society", 245));
    } catch (const exception& e) {
        cout << "Error seeding sample data: " << e.what() << endl;
    }
}
 
// ------------------------------------------------------------
// Main menu-driven console interface
// ------------------------------------------------------------
void printMenu() {
    cout << "\n================ LIBRARY MANAGEMENT SYSTEM ================\n";
    cout << " 1. Add a Book\n";
    cout << " 2. Add a DVD\n";
    cout << " 3. Add a Magazine\n";
    cout << " 4. Display All Items\n";
    cout << " 5. Search Item by Title\n";
    cout << " 6. Check Out an Item\n";
    cout << " 7. Return an Item\n";
    cout << " 8. Remove an Item\n";
    cout << " 9. Exit\n";
    cout << "=============================================================\n";
}
 
int main() {
    Library library;
    seedSampleData(library);
 
    bool running = true;
    while (running) {
        printMenu();
        int choice = readInt("Enter your choice (1-9): ");
 
        try {
            switch (choice) {
                case 1:
                    addBookFlow(library);
                    break;
                case 2:
                    addDVDFlow(library);
                    break;
                case 3:
                    addMagazineFlow(library);
                    break;
                case 4:
                    library.displayAllItems();
                    break;
                case 5: {
                    string keyword = readLine("Enter title keyword to search: ");
                    library.searchByTitle(keyword);
                    break;
                }
                case 6: {
                    string id = readLine("Enter item ID to check out: ");
                    library.checkOutItem(id);
                    break;
                }
                case 7: {
                    string id = readLine("Enter item ID to return: ");
                    library.returnItem(id);
                    break;
                }
                case 8: {
                    string id = readLine("Enter item ID to remove: ");
                    library.removeItem(id);
                    break;
                }
                case 9:
                    cout << "Exiting Library Management System. Goodbye!\n";
                    running = false;
                    break;
                default:
                    cout << "Invalid choice. Please select an option between 1 and 9.\n";
            }
        }
        // Specific exceptions first, then general fallback
        catch (const ItemNotFoundException& e) {
            cout << "Item Error: " << e.what() << endl;
        }
        catch (const ItemNotAvailableException& e) {
            cout << "Availability Error: " << e.what() << endl;
        }
        catch (const InvalidInputException& e) {
            cout << "Input Error: " << e.what() << endl;
        }
        catch (const exception& e) { // general exception handler
            cout << "An unexpected error occurred: " << e.what() << endl;
        }
    }
 
    return 0;
}