#include "index.h"


index::index()
{
}

index::index(string index_name, string table_name, string attr_name)
{
	m_index = index_name;
	m_table = table_name;
	m_attribute = attr_name;
}

index::~index()
{
}
