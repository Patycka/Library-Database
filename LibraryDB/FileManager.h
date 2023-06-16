#pragma once
using namespace std;

class FileManager
{
public:
	static bool fileExists(const string& filename);
	static void createEmptyRepositoryFile(const string& pathToFile, string rootName);
};

