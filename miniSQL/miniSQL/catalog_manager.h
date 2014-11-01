#ifndef _CATALOG_MANAGER_
#define _CATALOG_MANAGER_

#include <map>
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

typedef map<string, attribute> table;
map<string, table> t;



#endif /*_CATALOG_MANAGER_*/