#ifndef _ATTRIBUTE_
#define _ATTRIBUTE_

#include <string>
using namespace std;

enum ATTRIBUTE_TYPE
{
	INT,
	CHAR,
	FLOAT
};

class attribute
{
	string name;
	ATTRIBUTE_TYPE type;
	int char_num;
	bool primary;
	bool unique;
	bool index;
};

#endif