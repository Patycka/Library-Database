#pragma once
using namespace std;

class LibraryUser
{
	string name;
	list<int> borrowedBooks;
public:
	LibraryUser(string name);
	~LibraryUser();

	string getName();
	list<int> getBooks();
	bool hasBook(int bookId);
	bool hasAnyBooks();
	void addBook(int bookId);
	void removeBook(int bookId);
};

