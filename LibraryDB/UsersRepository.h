#pragma once
using namespace std;

class UsersRepository
{
	string _pathToFile;

	TiXmlElement* getUsersRepositoryXmlElement(TiXmlHandle docHandle);
	TiXmlElement* getFirstUserXmlElement(TiXmlHandle docHandle);
	string getUserXmlElementNameProperty(TiXmlElement* userElement);
	TiXmlElement* libraryUserToXmlElement(LibraryUser user);
	LibraryUser xmlElementToLibraryUser(TiXmlElement* userElement);

public:
	UsersRepository(string pathToFile);
	~UsersRepository();

	//*********************************************
	// public API for managing users
	//*********************************************
	const list<LibraryUser> getAll();
	LibraryUser getByName(string name);
	void add(LibraryUser user);
	void remove(LibraryUser user);
	bool exists(LibraryUser user);
	void update(LibraryUser user);
};

