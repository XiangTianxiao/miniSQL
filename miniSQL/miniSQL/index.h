#pragma once
#include <string>
using namespace std;

class index
{
public:
	string m_index;
	string m_table;
	string m_attribute;
public:
	index();
	index(string index_name, string table_name, string attr_name);
	~index();
};
