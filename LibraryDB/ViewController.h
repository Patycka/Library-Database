#pragma once
class ViewController
{
public:
	ViewController();
	~ViewController();

	void printUsers();
	void printBooks();
	void printBookUser(int bookId);
	void printUsersWithExpiredBooks();
};

