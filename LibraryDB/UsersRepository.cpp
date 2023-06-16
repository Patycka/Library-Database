#include "stdafx.h"
#include "UsersRepository.h"

UsersRepository::UsersRepository(string pathToFile){

	_pathToFile = pathToFile;

	if (!FileManager::fileExists(_pathToFile)) {
		FileManager::createEmptyRepositoryFile(_pathToFile, "Users");
	}
}

UsersRepository::~UsersRepository(){

}

const list<LibraryUser> UsersRepository::getAll() {
	list<LibraryUser> users;
	TiXmlDocument doc(_pathToFile.c_str());
	bool fileLoaded = doc.LoadFile();
	if (fileLoaded)
	{
		TiXmlHandle docHandle(&doc);
		TiXmlElement* userElement = getFirstUserXmlElement(docHandle);

		for (userElement; userElement; userElement = userElement->NextSiblingElement())
		{
			users.push_back(xmlElementToLibraryUser(userElement));
		}
	}
	else
	{
		throw runtime_error(EXCEPTION_COULD_NOT_LOAD_USERS_XML);
	}
	return users;
}

LibraryUser UsersRepository::getByName(string name) {
	try {
		list<LibraryUser> users = getAll();
		for (list<LibraryUser>::iterator iterator = users.begin(), end = users.end(); iterator != end; ++iterator) {
			if (Utils::caseInvCompare(iterator->getName(), name)) return *iterator;
		}
		throw runtime_error(EXCEPTION_USER_NOT_FOUND);
	}
	catch (runtime_error& e) {
		throw e;
	}
}

void UsersRepository::add(LibraryUser user) {

	if (exists(user)) throw runtime_error(EXCEPTION_USER_EXISTS);

	TiXmlDocument doc(_pathToFile.c_str());
	bool fileLoaded = doc.LoadFile();
	if (fileLoaded)
	{
		TiXmlHandle docHandle(&doc);
		TiXmlElement*  repositoryRoot = getUsersRepositoryXmlElement(docHandle);
		TiXmlElement* newUserElement = libraryUserToXmlElement(user);
		repositoryRoot->LinkEndChild(newUserElement);
		doc.SaveFile(_pathToFile.c_str());
	}
	else
	{
		throw runtime_error(EXCEPTION_COULD_NOT_LOAD_USERS_XML);
	}
}

void UsersRepository::remove(LibraryUser user) {
	TiXmlDocument doc(_pathToFile.c_str());
	bool fileLoaded = doc.LoadFile();
	if (fileLoaded)
	{
		TiXmlHandle docHandle(&doc);
		TiXmlElement* userElement = getFirstUserXmlElement(docHandle);

		for (userElement; userElement; userElement = userElement->NextSiblingElement())
		{
			string name = getUserXmlElementNameProperty(userElement);
			if (name.compare(user.getName()) == 0) {
				getUsersRepositoryXmlElement(docHandle)->RemoveChild(userElement);
				doc.SaveFile(_pathToFile.c_str());
				return;
			}
		}

		throw runtime_error(EXCEPTION_USER_NOT_FOUND);
	}
	else
	{
		throw runtime_error(EXCEPTION_COULD_NOT_LOAD_USERS_XML);
	}
}

bool UsersRepository::exists(LibraryUser user) {
	try {
		list<LibraryUser> users = getAll();
		for (list<LibraryUser>::iterator iterator = users.begin(), end = users.end(); iterator != end; ++iterator) {
			if (iterator->getName().compare(user.getName()) == 0) return true;
		}
		return false;
	}
	catch (runtime_error& e) {
		throw e;
	}
}
	

void UsersRepository::update(LibraryUser user) {
	try {
		remove(user);
		add(user);
	}
	catch(runtime_error& e) {
		throw e;
	}
}

TiXmlElement* UsersRepository::getUsersRepositoryXmlElement(TiXmlHandle docHandle) {
	TiXmlElement* repositoryElement = docHandle.FirstChild("Users").ToElement();
	return repositoryElement;
}

TiXmlElement* UsersRepository::getFirstUserXmlElement(TiXmlHandle docHandle) {
	TiXmlElement* repositoryRoot = getUsersRepositoryXmlElement(docHandle);
	TiXmlElement* userElement = repositoryRoot->FirstChildElement("User");
	return userElement;
}

string UsersRepository::getUserXmlElementNameProperty(TiXmlElement* userElement) {
	return userElement->FirstChildElement("Name")->GetText();
}

LibraryUser UsersRepository::xmlElementToLibraryUser(TiXmlElement* userElement) {
	TiXmlElement* userNameElement = userElement->FirstChild("Name")->ToElement();
	LibraryUser newUser(userNameElement->GetText());

	if (userElement->FirstChild("BorrowedBooks")->FirstChild("Book") != NULL) {
		TiXmlElement* userBookElement = userElement->FirstChild("BorrowedBooks")->FirstChild("Book")->ToElement();
		for (userBookElement; userBookElement; userBookElement = userBookElement->NextSiblingElement()) {
			newUser.addBook(atoi(userBookElement->GetText()));
		}
	}

	return newUser;
}

TiXmlElement* UsersRepository::libraryUserToXmlElement(LibraryUser user) {
	TiXmlElement* newUserElement = new TiXmlElement("User");
	TiXmlElement* nameElement = new TiXmlElement("Name");
	TiXmlText* name = new TiXmlText(user.getName().c_str());
	nameElement->LinkEndChild(name);
	newUserElement->LinkEndChild(nameElement);
	TiXmlElement* borrowedBooksElement = new TiXmlElement("BorrowedBooks");
	list<int> userBooks = user.getBooks();
	for (list<int>::iterator iterator = userBooks.begin(), end = userBooks.end(); iterator != end; ++iterator) {
		TiXmlElement* bookElement = new TiXmlElement("Book");
		TiXmlText* book = new TiXmlText(to_string(*iterator).c_str());
		bookElement->LinkEndChild(book);
		borrowedBooksElement->LinkEndChild(bookElement);
	}
	newUserElement->LinkEndChild(borrowedBooksElement);

	return newUserElement;
}