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
	for (vector<attribute>::iterator i = m_attr.begin(); i != m_attr.end(); i++)
	{
		if (i->get_name()==attr_name)
			return *i;
	}
	throw error(NO_SUCH_ATTRIBUTE, "catalog manager", "get_attribute", "无法找到，不存在这个名字的列");
}

void table::insert_attribute(attribute attr)
{
	for (vector<attribute>::iterator i = m_attr.begin(); i != m_attr.end(); i++)
	{
		if (i->get_name() == attr.get_name())
			throw error(ALREADY_EXIST_SUCH_ATTRIBUTE, "catalog manager", "insert_attribute", "无法插入，已经存在相同的列了");
	}
	m_attr.push_back(attr);
}

ostream& operator<<(ostream& out, table t)
{
	for (vector<attribute>::iterator i = t.m_attr.begin(); i != t.m_attr.end(); i++)
		out << *i;
	return out;
}

int table::get_record_size()
{
	int size = 0;
	for (vector<attribute>::iterator i = m_attr.begin(); i != m_attr.end(); i++)
	{
		switch (i->get_type())
		{
		case ATTRIBUTE_TYPE::CHAR:
			size += i->get_char_num();
			break;
		case ATTRIBUTE_TYPE::FLOAT:
			size += 4;
			break;
		case ATTRIBUTE_TYPE::INT:
			size += 4;
			break;
		default:
			break;
		}
	}
	return size;
}

int table::get_record_size_before_a(attribute a)
{
	int size = 0;
	for (vector<attribute>::iterator i = m_attr.begin(); i != m_attr.end(); i++)
	{
		if (i->get_name() == a.get_name())
			break;
		switch (i->get_type())
		{
		case ATTRIBUTE_TYPE::CHAR:
			size += i->get_char_num();
			break;
		case ATTRIBUTE_TYPE::FLOAT:
			size += 4;
			break;
		case ATTRIBUTE_TYPE::INT:
			size += 4;
			break;
		default:
			break;
		}
	}
	return size;
}

const vector<attribute> table::get_attribute_list()
{
	return m_attr;
}

int table::get_attr_index(attribute a)
{
	int index = 0;
	for (vector<attribute>::iterator i = m_attr.begin(); i != m_attr.end(); i++)
	{
		if (i->get_name() == a.get_name())
			return index;
		index++;
	}
	throw error();
}

int table::get_attribute_index(attribute a)
{
	int index = 0;
	for (vector<attribute>::iterator i = m_attr.begin(); i != m_attr.end(); i++)
	{
		if (i->get_name() == a.get_name())
			return index;
		index++;
	}
	return -1;
}

void table::set_name(string name)
{
	m_name = name;
}