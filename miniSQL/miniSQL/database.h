#ifndef _database_
#define _database_

#include "table.h"
#include <map>
using namespace std;



//typedef map<string, attribute> table;



class database
{
private:
	map<string, table> t;
public:
	database();
	~database();
	bool get_attribute(attribute& target, string table_name, string attr_name);
};

#endif