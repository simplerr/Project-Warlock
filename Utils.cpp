#include "Utils.h"

vector<string> SplitString(string text, char delim)
{
	vector<string> elems;
	stringstream ss(text);
	string item;

	while(getline(ss, item, delim)) {
		elems.push_back(item);
	}

	return elems;
}