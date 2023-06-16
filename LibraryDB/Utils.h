#pragma once
using namespace std;

class Utils
{
public:
	Utils();
	~Utils();

	static bool compare(unsigned char a, unsigned char b);
	static bool caseInvCompare(string const& a, string const& b);
	static tm dateStringToDate(string dateString);
	static double compareDates(tm date1, tm date2);
	static vector<string> splitDateString(const char *str, char c = ' ');
};

