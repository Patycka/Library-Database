#include "stdafx.h"
#include "FileManager.h"

bool FileManager::fileExists(const string& pathToFile) {
	ifstream ifile(pathToFile.c_str());
	return (bool)ifile;
}

void FileManager::createEmptyRepositoryFile(const string& pathToFile, string rootName) {
	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "", "");
	TiXmlElement * element = new TiXmlElement(rootName.c_str());
	doc.LinkEndChild(decl);
	doc.LinkEndChild(element);
	doc.SaveFile(pathToFile.c_str());
}