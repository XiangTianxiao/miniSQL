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
	friend ofstream& operator<<(ofstream out, table t);
};

ofstream& operator<<(ofstream out, table t)
{
	for (map<string, attribute>::iterator i = t.m_attr.begin(); i != t.m_attr.end(); i++)
	{
		//out << i->second;
	}
		return out;
}

#endif