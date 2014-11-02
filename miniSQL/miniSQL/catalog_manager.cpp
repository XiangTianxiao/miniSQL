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
		throw error(NO_SUCH_TABLE, "catalog manager", "get_table", "�޷��ҵ���������������ֵı�");
	return m_t[name];
}

attribute& catalog_manager::get_attribute(string table_name, string attr_name)
{
	if (m_t.find(table_name) == m_t.end())
		throw error(NO_SUCH_TABLE, "catalog manager", "get_attribute", "�޷��ҵ���������������ֵı�");
	return m_t[table_name].get_attribute(attr_name);
}

void catalog_manager::add_table(table t)
{
	if (m_t.find(t.get_name()) != m_t.end())
		throw error(ALREADY_EXIST_SUCH_TABLE, "catalog manager", "insert_table", "�޷����룬�Ѿ�������ͬ�ı���");
	m_t[t.get_name()] = t;
}

//����������
void catalog_manager::drop_table(string name)
{
	if (m_t.find(name) == m_t.end())
		throw error(NO_SUCH_TABLE, "catalog manager", "drop_table", "�޷��ҵ���������������ֵı�");
	m_t.erase(name);
}

void catalog_manager::add_index(string table_name, string attr_name, string index_name)
{
	if (m_i.find(index_name) != m_i.end())
		throw error(ALREADY_EXIST_SUCH_INDEX, "catalog manager", "add_index", "�޷����룬�Ѿ�������ͬ��������");
	attribute& attr = get_attribute(table_name, attr_name);
	attr.index_on();
	m_i[index_name] = index(index_name, table_name, attr_name);
}

void catalog_manager::drop_index(string name)
{
	if (m_i.find(name) == m_i.end())
		throw error(NO_SUCH_INDEX, "catalog manager", "drop_index", "�޷��ҵ���������������ֵ�����");
	string table = m_i[name].m_table;
	string attr = m_i[name].m_attribute;
	m_i.erase(name);
	get_attribute(table, attr).index_off();
}
