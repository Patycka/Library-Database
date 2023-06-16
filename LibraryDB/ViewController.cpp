#include "stdafx.h"
#include "ViewController.h"

using namespace std;


ViewController::ViewController()
{
}


ViewController::~ViewController()
{
}

void ViewController::printUsers() {

	LibraryManager* manager = new LibraryManager();
	list <LibraryUser> users = manager->getAllUsers();
	
	list <LibraryUser>::iterator it;	
	cout << "List of users: " << endl;
	if (users.empty()) {
		cout << "No people in the list" << endl;
	}
	for (it = users.begin(); it != users.end(); ++it) {
		cout << it->getName() << endl;
	}
	
	delete(manager);	
}

void ViewController::printBooks() {

	LibraryManager* manager = new LibraryManager();

	list<Book> allBooks = manager->getAllBooks();

	list <Book>::iterator it;
	cout << "List of books: " << endl;
	if (allBooks.empty()) {
		cout << "Lack of book in the library" << endl;
	}
	for (it = allBooks.begin(); it != allBooks.end(); ++it) {
		if (it->isBorrowed() == 0) {
			cout << "Title: " << it->getName() << " Id: " << it->getId() << " Not borrowed" << endl;
		}
		else {
			cout << "Title: " << it->getName() << " Id: " << it->getId() << " Borrowed" << endl;
		}
	}

	delete(manager);
}

void ViewController::printBookUser(int bookId) {

	LibraryManager* manager = new LibraryManager();

	LibraryUser user = manager->getCurrentBookUser(bookId);

	cout << user.getName() << endl;

	delete(manager);
}

void ViewController::printUsersWithExpiredBooks() {

	LibraryManager* manager = new LibraryManager();

	list <LibraryUser> users = manager->getAllUsersWithExpiredBooks();

	list <LibraryUser>::iterator it;
	cout << "List of users: " << endl;
	if (users.empty()) {
		cout << "No people with expired books" << endl;
	}
	for (it = users.begin(); it != users.end(); ++it) {
		cout << it->getName() << endl;
	}

	delete(manager);
}



