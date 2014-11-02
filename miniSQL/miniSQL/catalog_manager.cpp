#include "catalog_manager.h"
#include "error.h"

catalog_manager::catalog_manager()
{

}


catalog_manager::~catalog_manager()
{
}

table& catalog_manager::get_table(string name)
{
	if (m_t.find(name) == m_t.end())
		throw error(NO_SUCH_TABLE, "catalog manager", "get_table", "无法找到，不存在这个名字的表");
	return m_t[name];
}

attribute& catalog_manager::get_attribute(string table_name, string attr_name)
{
	if (m_t.find(table_name) == m_t.end())
		throw error(NO_SUCH_TABLE, "catalog manager", "get_attribute", "无法找到，不存在这个名字的表");
	return m_t[table_name].get_attribute(attr_name);
}

void catalog_manager::add_table(table t)
{
	if (m_t.find(t.get_name()) != m_t.end())
		throw error(ALREADY_EXIST_SUCH_TABLE, "catalog manager", "insert_table", "无法插入，已经存在相同的表了");
	m_t[t.get_name()] = t;
}

//索引的问题
void catalog_manager::drop_table(string name)
{
	if (m_t.find(name) == m_t.end())
		throw error(NO_SUCH_TABLE, "catalog manager", "drop_table", "无法找到，不存在这个名字的表");
	m_t.erase(name);
}

void catalog_manager::add_index(string table_name, string attr_name, string index_name)
{
	if (m_i.find(index_name) != m_i.end())
		throw error(ALREADY_EXIST_SUCH_INDEX, "catalog manager", "add_index", "无法插入，已经存在相同的索引了");
	attribute& attr = get_attribute(table_name, attr_name);
	attr.index_on();
	m_i[index_name] = index(index_name, table_name, attr_name);
}

void catalog_manager::drop_index(string name)
{
	if (m_i.find(name) == m_i.end())
		throw error(NO_SUCH_INDEX, "catalog manager", "drop_index", "无法找到，不存在这个名字的索引");
	string table = m_i[name].m_table;
	string attr = m_i[name].m_attribute;
	m_i.erase(name);
	get_attribute(table, attr).index_off();
}
