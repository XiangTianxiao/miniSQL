#pragma once
#include <string>
#include "attribute.h"

using namespace std;

class index
{
public:
	string m_index;
	string m_table;
	string m_attribute;
	int m_char_num;
	ATTRIBUTE_TYPE m_type;
public:
	index();
	index(string index_name, string table_name, string attr_name, ATTRIBUTE_TYPE type, int char_num);
	~index();
};
