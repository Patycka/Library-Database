#include "stdafx.h"
#include "Utils.h"


Utils::Utils()
{
}


Utils::~Utils()
{
}

bool Utils::compare(unsigned char a, unsigned char b)
{
	return tolower(a) == tolower(b);
}

bool Utils::caseInvCompare(string const& a, string const& b)
{
	if (a.length() == b.length()) {
		return equal(b.begin(), b.end(),
			a.begin(), compare);
	}
	else {
		return false;
	}
}

tm Utils::dateStringToDate(string dateString) {
	if (dateString == "") throw runtime_error(EXCEPTION_DATE_EMPTY);
	vector<string> splittedDateString = splitDateString(dateString.c_str(), '-');
	int day = atoi(splittedDateString[0].c_str());
	int month = atoi(splittedDateString[1].c_str());
	int year = atoi(splittedDateString[2].c_str());
	// current date/time based on current system
	//time_t now = time(0);

	//cout << "Number of sec since January 1,1970:" << now << endl;

	tm date;
	date.tm_mday = day;
	date.tm_mon = month - 1;
	date.tm_year = year - 1900;
	date.tm_hour = 0;
	date.tm_min = 0;
	date.tm_sec = 0;
	
	// print various components of tm structure.
	/*cout << "Year" << 1900 + ltm->tm_year << endl;
	cout << "Month: " << 1 + ltm->tm_mon << endl;
	cout << "Day: " << ltm->tm_mday << endl;
	cout << "Time: " << 1 + ltm->tm_hour << ":";
	cout << 1 + ltm->tm_min << ":";
	cout << 1 + ltm->tm_sec << endl;*/
	return date;
}

double Utils::compareDates(tm date1, tm date2) {
	return difftime(mktime(&date1), mktime(&date2));
}

vector<string> Utils::splitDateString(const char *str, char c)
{
	vector<string> result;

	do
	{
		const char *begin = str;

		while (*str != c && *str)
			str++;

		result.push_back(string(begin, str));
	} while (0 != *str++);

	return result;
}