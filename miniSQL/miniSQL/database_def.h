#ifndef _DATABASE_DEF_
#define _DATABASE_DEF_
#include "table.h"
#include <map>
using namespace std;



//typedef map<string, attribute> table;



class database_def
{
private:
	map<string, table> t;
public:
	database_def();
	~database_def();
	bool get_attribute(attribute& target, string table_name, string attr_name);
};

#endif