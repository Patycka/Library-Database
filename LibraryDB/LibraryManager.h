#pragma once
using namespace std;

class LibraryManager
{
public:
	LibraryManager();
	~LibraryManager();

	//*********************************************
	// public API for library managment
	//*********************************************
	void addNewUserToLibrary(string userName);
	void removeUserFromLibrary(string userName, bool force = false);
	void addNewBookToLibrary(string bookName);
	void removeBookFromLibrary(int bookId, bool force = false);
	void lendBookToUser(int bookId, string userName, string returnDate);
	void returnBookFromUser(int bookId, string userName);
	
	Book getBook(int bookId);
	LibraryUser getUser(string userName);
	list<Book> getBooks(string bookName);
	Book getFirstAvailableBook(string bookName);
	list<Book> getUserBooks(string userName);
	LibraryUser getCurrentBookUser(int bookId);
	list<LibraryUser> getAllUsers();
	list<Book> getAllBooks();
	list<Book> getAllBorrowedBooks();
	list<Book> getAllExpiredBooks();
	list<LibraryUser> getAllUsersWithExpiredBooks();
};

