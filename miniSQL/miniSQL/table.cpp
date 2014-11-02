#include "table.h"
#include "error.h"

table::table()
{
}


table::table(string name)
{
	m_name = name;
}


table::~table()
{
}

string table::get_name()
{
	return m_name;
}
attribute& table::get_attribute(string attr_name)
{
	if (m_attr.find(attr_name) == m_attr.end())
		throw error(NO_SUCH_ATTRIBUTE, "catalog manager", "get_attribute", "无法找到，不存在这个名字的列");
	else
		return m_attr[attr_name];
}

void table::insert_attribute(attribute attr)
{
	if (m_attr.find(attr.get_name()) != m_attr.end())
		throw error(ALREADY_EXIST_SUCH_ATTRIBUTE, "catalog manager", "insert_attribute", "无法插入，已经存在相同的列了");
	m_attr[attr.get_name()] = attr;
}