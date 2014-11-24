#include "index.h"


index::index()
{
}

index::index(string index_name, string table_name, string attr_name, ATTRIBUTE_TYPE type, int char_num)
{
	m_index = index_name;
	m_table = table_name;
	m_attribute = attr_name;
	m_type = type;
	m_char_num = char_num;
}

index::~index()
{
}
