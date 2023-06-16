#pragma once
using namespace std;

class Book
{
	int id;
	string name;
	string returnDate;
	string reader;
public:
	Book(string bookName);
	~Book();

	int getId();
	bool isBorrowed();
	bool isReturnTimeExpired();
	string getName();
	string getReturnDate();
	string getReader();
	void setId(int newId);
	void setReader(string readerName);
	void setReturnDate(string date);
};

