#pragma once
using namespace std;

class BooksRepository
{
	string _pathToFile;

	TiXmlElement* getBooksRepositoryXmlElement(TiXmlHandle docHandle);
	TiXmlElement* getFirstBookXmlElement(TiXmlHandle docHandle);
	int getBookXmlElementIdProperty(TiXmlElement* bookElement);
	TiXmlElement* bookToXmlElement(Book book);
	Book xmlElementToBook(TiXmlElement* bookElement);
	int getNewId();
public:
	BooksRepository(string pathToFile);
	~BooksRepository();

	//*********************************************
	// public API for managing books
	//*********************************************
	const list<Book> getAll();
	list<Book> getByName(string name);
	Book getById(int bookId);
	void add(Book user);
	void remove(Book user);
	bool exists(Book user);
	void update(Book user);
};

