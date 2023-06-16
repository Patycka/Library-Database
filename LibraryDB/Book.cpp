#include "stdafx.h"
#include "Book.h"

Book::Book(string bookName)
{
	name = bookName;
	id = 0;
}

Book::~Book()
{
}

int Book::getId() {
	return id;
}

bool Book::isBorrowed() {
	return getReader() != "";
}

bool Book::isReturnTimeExpired() {
	time_t now = time(0);
	tm *ltm = new tm;
	localtime_s(ltm, &now);
	ltm->tm_mday -= 1;
	try {
		double seconds = Utils::compareDates(*ltm, Utils::dateStringToDate(getReturnDate()));
		if (seconds < 0.0) return false;
		else return true;
	}
	catch (runtime_error& e) {
		return false;
	}
	
}

string Book::getName() {
	return name;
}

string Book::getReturnDate() {
	return returnDate;
}

string Book::getReader() {
	return reader;
}

void Book::setId(int newId) {
	id = newId;
}

void Book::setReader(string readerName) {
	reader = readerName;
}

void Book::setReturnDate(string date) {
	returnDate = date;
}