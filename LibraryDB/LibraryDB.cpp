// LibraryDB.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

using namespace std;

void example1();
void initUsersDB();
void checkReaders();
void checkBooks();
void checkReadersWithExpiredBooks();
void checkBookUser(int bookId);
void addNewBook(string bookName);
void addNewUser(string userName);
void removeBook(int bookId, bool force);
void removeUser(string userName, bool force);
void lendBookToUser(int bookId, string userName, string returnDate);

int main()
{
	
	ViewController* viewcontroller = new ViewController();

	string choice;
	string title, user, data;
	int bookId, flagId;
	

	while (1) {
		cout << "MENU" << endl;
		cout << "  [1] Check readers in library" << endl;
		cout << "  [2] Check books" << endl;
		cout << "  [3] Check who has expired books" << endl;
		cout << "  [4] Who has a specified book" << endl;
		cout << "  [5] Add new book" << endl;
		cout << "  [6] Add new user" << endl;
		cout << "  [7] Lend book to user" << endl;
		cout << "  [8] Remove a book" << endl;
		cout << "  [9] Remove user" << endl;
		cout << "  [10] Exit." << endl;

		cout << "Select option by pressing button 1-10 and confirm by clicking ENTER." << endl;

		getline(cin, choice);

		if (choice == "1") {
			checkReaders();
		}
		else if (choice == "2") {
			checkBooks();
		}			
		else if (choice == "3") {
			checkReadersWithExpiredBooks();
		}
		else if (choice == "4") {
			flagId = 0;
			cout << "Print ID of the book: ";
			while (!(cin >> bookId)) {
				cin.clear();
				cin.sync();
				getchar();	
				if (flagId == 0) {
					cout << "Incorrect data. Try again." << endl;
					flagId = 1;
				}			
			}
			checkBookUser(bookId);
			getchar();
		}
		else if (choice == "5") {
			cout << "Print the title of the book: " << endl;
			getline(cin, title);
			addNewBook(title);
		}
		else if (choice == "6") {
			cout << "Print user name: ";
			getline(cin, user);
			addNewUser(user);
		}
		else if (choice == "7") {
			cout << "Print ID of the book: ";
			while (!(cin >> bookId)) {
				cin.clear();
				cin.sync();
				getchar();
				if (flagId == 0) {
					cout << "Incorrect data. Try again." << endl;
					flagId = 1;
				}
			}
			getchar();
			cout << "Print user name: ";
			getline(cin, user);
			
			cout << "Print data of donation in format dd-mm-rrrr ";
			getline(cin, data);
			lendBookToUser(bookId, user, data);

		}
		else if (choice == "8") {
			flagId = 0;
			cout << "Print ID of the book: ";
			while (!(cin >> bookId)) {
				cin.clear();
				cin.sync();
				getchar();
				if (flagId == 0) {
					cout << "Incorrect data. Try again." << endl;
					flagId = 1;
				}
			}
			removeBook(bookId, 0);
		}
		else if (choice == "9") {
			cout << "Print user name: ";
			getline(cin, user);
			removeUser(user, 0);
		}
		else if (choice == "10") {
			cout << "This is the end of the program! See you soon. \n" << endl;
			getchar();
			return 0;
		}
		else {
			cout << "Incorrect data. Try again" << endl;
		}


	}
	
	getchar();
	getchar();
	return 0;
}



// EXAMPLE OF HIGH LEVEL LIBRARY MANAGMENT
void checkReaders() {
	try {
		ViewController* viewcontroller = new ViewController();
		viewcontroller->printUsers();

		delete(viewcontroller);
	}
	catch (runtime_error& e) {
		cout << e.what() << endl; 
		getchar(); 
	}
}

void checkBooks() {
	try {
		ViewController* viewcontroller = new ViewController();
		viewcontroller->printBooks();

		delete(viewcontroller);
	}
	catch (runtime_error& e) {
		cout << e.what() << endl;
		getchar();
	}
}

void checkReadersWithExpiredBooks() {
	try {
		ViewController* viewcontroller = new ViewController();
		viewcontroller->printUsersWithExpiredBooks();

		delete(viewcontroller);
	}
	catch (runtime_error& e) {
		cout << e.what() << endl;
		getchar();
	}
}


void checkBookUser(int bookId) {
	try {
		ViewController* viewcontroller = new ViewController();
		viewcontroller->printBookUser(bookId);

		delete(viewcontroller);
	}
	catch (runtime_error& e) {
		cout << e.what() << endl;
		getchar();
	}
}

void addNewBook(string bookName) {
	try {
		LibraryManager* manager = new LibraryManager();

		manager->addNewBookToLibrary(bookName);

		delete(manager);
	}
	catch (runtime_error& e) {
		cout << e.what() << endl; 
		getchar();
	}
}

void addNewUser(string userName) {
	try {
		LibraryManager* manager = new LibraryManager();

		manager->addNewUserToLibrary(userName);

		delete(manager);
	}
	catch (runtime_error& e) {
		cout << e.what() << endl;
		getchar();
	}
}

void removeBook(int bookId, bool force) {
	string choiceRemove;
	try {
		LibraryManager* manager = new LibraryManager();

		manager->removeBookFromLibrary(bookId, force);

		delete(manager);
	}
	catch (runtime_error& e) {
		cout << e.what() << endl;
		if (e.what() == EXCEPTION_BOOK_IS_BORROWED) {
			cout << "Do you still want to remove the book? Print yes or no" << endl;
			if (choiceRemove == "yes") {
				removeBook(bookId, 1);
			}
			else if (choiceRemove == "no") {
				cout << "Return to menu" << endl;
			}
			else {
				cout << "Incorrect data. Try again" << endl;
			}
		}
		getchar();
	}
}

void lendBookToUser(int bookId, string userName, string returnDate) {
	try {
		LibraryManager* manager = new LibraryManager();

		manager->lendBookToUser(bookId, userName, returnDate);

		delete(manager);
	}
	catch (runtime_error& e) {
		cout << e.what() << endl;
		getchar();
	}
}

void removeUser(string userName, bool force) {
	string choiceRemove;
	try {
		LibraryManager* manager = new LibraryManager();

		manager->removeUserFromLibrary(userName, force);

		delete(manager);
	}
	catch (runtime_error& e) {
		cout << e.what() << endl;
		if (e.what() == EXCEPTION_USER_HAS_NON_RETURNED_BOOKS) {
			cout << "Do you still want to remove this user? Print yes or no" << endl;
			if (choiceRemove == "yes") {
				removeUser(userName, 1);
			}
			else if (choiceRemove == "no") {
				cout << "Return to menu" << endl;
			}
			else {
				cout << "Incorrect data. Try again" << endl;
			}
		}
	}
}


void initUsersDB() {
	try {
		LibraryManager* lm = new LibraryManager();
		lm->addNewUserToLibrary("jan kowalski");
		lm->addNewUserToLibrary("bozena skalska");
		lm->addNewUserToLibrary("tomasz nowak");
	}
	catch (runtime_error& e) {
		cout << e.what() << endl;
	}
}

void example1() {
	try {
		LibraryManager* manager = new LibraryManager();

		manager->addNewBookToLibrary("Mistrz i Malgorzata");

		manager->addNewUserToLibrary("Jan Kowalski");

		Book firstAvailableBook = manager->getFirstAvailableBook("mistrz i malgorzata");

		int bookId = firstAvailableBook.getId();

		manager->lendBookToUser(bookId, "jan kowalski", "28-04-2017");

		manager->returnBookFromUser(bookId, "jan kowalski");

		manager->removeBookFromLibrary(bookId);

		manager->removeUserFromLibrary("jan kowalski");

		delete(manager);
		getchar(); // wait for key before closing
	}
	catch (runtime_error& e) {
		cout << e.what() << endl; // prints exception message on screen
		getchar(); // wait for key before closing
	}
}