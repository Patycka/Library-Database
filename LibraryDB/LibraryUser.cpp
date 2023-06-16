#include "stdafx.h"
#include "LibraryUser.h"


LibraryUser::LibraryUser(string newUserName)
{
	name = newUserName;
}

LibraryUser::~LibraryUser()
{
}

string LibraryUser::getName() {
	return this->name;
}

list<int> LibraryUser::getBooks() {
	return this->borrowedBooks;
}

bool LibraryUser::hasBook(int bookId) {
	list<int> books = getBooks();
	for (list<int>::iterator iterator = books.begin(), end = books.end(); iterator != end; ++iterator) {
		if (*iterator == bookId) return true;
	}
	return false;
}

bool LibraryUser::hasAnyBooks() {
	return !getBooks().empty();
}

void LibraryUser::addBook(int bookId) {
	if (!this->hasBook(bookId)) {
		this->borrowedBooks.push_back(bookId);
	}
	else {
		throw runtime_error("Book already exists in user books");
	}
}

void LibraryUser::removeBook(int bookId) {
	if (this->hasBook(bookId)) {
		this->borrowedBooks.remove(bookId);
	}
	else{
		throw runtime_error("Book not found in user books");
	}
}
