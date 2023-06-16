#include "stdafx.h"
#include "BooksRepository.h"


BooksRepository::BooksRepository(string pathToFile) {

	_pathToFile = pathToFile;

	if (!FileManager::fileExists(_pathToFile)) {
		FileManager::createEmptyRepositoryFile(_pathToFile, "Books");
	}
}

BooksRepository::~BooksRepository() {

}

const list<Book> BooksRepository::getAll() {
	list<Book> books;
	TiXmlDocument doc(_pathToFile.c_str());
	bool fileLoaded = doc.LoadFile();
	if (fileLoaded)
	{
		TiXmlHandle docHandle(&doc);
		TiXmlElement* bookElement = getFirstBookXmlElement(docHandle);

		for (bookElement; bookElement; bookElement = bookElement->NextSiblingElement())
		{
			books.push_back(xmlElementToBook(bookElement));
		}
	}
	else
	{
		throw runtime_error(EXCEPTION_COULD_NOT_LOAD_BOOKS_XML);
	}
	return books;
}

list<Book> BooksRepository::getByName(string name) {
	try {
		bool anyFound = false;
		list<Book> books = getAll();
		list<Book> booksWithMatchingName;
		for (list<Book>::iterator iterator = books.begin(), end = books.end(); iterator != end; ++iterator) {
			if (Utils::caseInvCompare(iterator->getName(), name)) {
				booksWithMatchingName.push_back(*iterator);
				anyFound = true;
			}
		}
		if (anyFound) {
			return booksWithMatchingName;
		}
		else {
			throw runtime_error(EXCEPTION_BOOKS_NOT_FOUND);
		}
	}
	catch (runtime_error& e) {
		throw e;
	}
}

Book BooksRepository::getById(int bookId) {
	try {
		list<Book> books = getAll();
		for (list<Book>::iterator iterator = books.begin(), end = books.end(); iterator != end; ++iterator) {
			if (iterator->getId() == bookId) {
				return *iterator;
			}
		}
		throw runtime_error(EXCEPTION_BOOK_NOT_FOUND);
	}
	catch (runtime_error& e) {
		throw e;
	}
}

void BooksRepository::add(Book book) {

	if (exists(book)) throw runtime_error(EXCEPTION_BOOK_EXISTS);

	TiXmlDocument doc(_pathToFile.c_str());
	bool fileLoaded = doc.LoadFile();
	if (fileLoaded)
	{
		TiXmlHandle docHandle(&doc);
		TiXmlElement*  repositoryRoot = getBooksRepositoryXmlElement(docHandle);
		TiXmlElement* newBookElement = bookToXmlElement(book);
		repositoryRoot->LinkEndChild(newBookElement);
		doc.SaveFile(_pathToFile.c_str());
	}
	else
	{
		throw runtime_error(EXCEPTION_COULD_NOT_LOAD_BOOKS_XML);
	}
}

void BooksRepository::remove(Book book) {
	TiXmlDocument doc(_pathToFile.c_str());
	bool fileLoaded = doc.LoadFile();
	if (fileLoaded)
	{
		TiXmlHandle docHandle(&doc);
		TiXmlElement* bookElement = getFirstBookXmlElement(docHandle);

		for (bookElement; bookElement; bookElement = bookElement->NextSiblingElement())
		{
			int id = getBookXmlElementIdProperty(bookElement);
			if (id == book.getId()) {
				getBooksRepositoryXmlElement(docHandle)->RemoveChild(bookElement);
				doc.SaveFile(_pathToFile.c_str());
				return;
			}
		}

		throw runtime_error(EXCEPTION_BOOK_NOT_FOUND);
	}
	else
	{
		throw runtime_error(EXCEPTION_COULD_NOT_LOAD_BOOKS_XML);
	}
}

bool BooksRepository::exists(Book book) {
	try {
		list<Book> books = getAll();
		for (list<Book>::iterator iterator = books.begin(), end = books.end(); iterator != end; ++iterator) {
			if (book.getId() == iterator->getId()) return true;
		}
		return false;
	}
	catch (runtime_error& e) {
		throw e;
	}
}

void BooksRepository::update(Book book) {
	try {
		remove(book);
		add(book);
	}
	catch (runtime_error& e) {
		throw e;
	}
}

TiXmlElement* BooksRepository::getBooksRepositoryXmlElement(TiXmlHandle docHandle) {
	TiXmlElement* repositoryElement = docHandle.FirstChild("Books").ToElement();
	return repositoryElement;
}

TiXmlElement* BooksRepository::getFirstBookXmlElement(TiXmlHandle docHandle) {
	TiXmlElement* repositoryRoot = getBooksRepositoryXmlElement(docHandle);
	TiXmlElement* bookElement = repositoryRoot->FirstChildElement("Book");
	return bookElement;
}

int BooksRepository::getBookXmlElementIdProperty(TiXmlElement* bookElement) {
	return atoi(bookElement->FirstChildElement("Id")->GetText());
}

Book BooksRepository::xmlElementToBook(TiXmlElement* bookElement) {
	TiXmlElement* bookNameElement = bookElement->FirstChild("Name")->ToElement();
	Book newBook(bookNameElement->GetText());

	TiXmlElement* bookIdElement = bookElement->FirstChild("Id")->ToElement();
	newBook.setId(atoi(bookIdElement->GetText()));

	TiXmlElement* bookReturnDateElement = bookElement->FirstChild("ReturnDate")->ToElement();
	if(bookReturnDateElement->GetText() != NULL) newBook.setReturnDate(bookReturnDateElement->GetText());

	TiXmlElement* bookReaderElement = bookElement->FirstChild("Reader")->ToElement();
	if (bookReaderElement->GetText() != NULL) newBook.setReader(bookReaderElement->GetText());

	return newBook;
}

TiXmlElement* BooksRepository::bookToXmlElement(Book book) {
	TiXmlElement* newBookElement = new TiXmlElement("Book");

	TiXmlElement* idElement = new TiXmlElement("Id");
	TiXmlText* id = (book.getId() == 0) ? new TiXmlText(to_string(getNewId()).c_str()) : new TiXmlText(to_string(book.getId()).c_str());
	idElement->LinkEndChild(id);
	newBookElement->LinkEndChild(idElement);

	TiXmlElement* nameElement = new TiXmlElement("Name");
	TiXmlText* name = new TiXmlText(book.getName().c_str());
	nameElement->LinkEndChild(name);
	newBookElement->LinkEndChild(nameElement);

	TiXmlElement* returnDateElement = new TiXmlElement("ReturnDate");
	TiXmlText* date = new TiXmlText(book.getReturnDate().c_str());
	returnDateElement->LinkEndChild(date);
	newBookElement->LinkEndChild(returnDateElement);

	TiXmlElement* readerElement = new TiXmlElement("Reader");
	TiXmlText* reader = new TiXmlText(book.getReader().c_str());
	readerElement->LinkEndChild(reader);
	newBookElement->LinkEndChild(readerElement);

	return newBookElement;
}

int BooksRepository::getNewId() {
	int newId = 0;
	int currentId;
	list<Book> books = this->getAll();
	for (list<Book>::iterator iterator = books.begin(), end = books.end(); iterator != end; ++iterator) {
		currentId = iterator->getId();
		if (currentId >= newId) newId = currentId;
	}
	return ++newId;
}
