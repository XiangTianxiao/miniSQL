#ifndef _TABLE_
#define _TABLE_

#include "attribute.h"
#include <map>
#include <string>
#include <fstream>
using namespace std;
class table
{
private:
	string m_name;
	map<string, attribute> m_attr;
public:
	table();
	table(string name);
	~table();
	string get_name();
	attribute& get_attribute(string attr_name);
	void insert_attribute(attribute attr);
	friend ostream& operator<<(ostream& out, table t);
};

//ostream& operator<<(ostream& out, table t);

#endif