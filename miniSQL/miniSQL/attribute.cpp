#include "attribute.h"
#include "error.h"

attribute::attribute()
{
	clear();
}

attribute::attribute(string name, ATTRIBUTE_TYPE type, int char_num, bool primary, bool unique)
{
	if (type != ATTRIBUTE_TYPE::CHAR)
		throw error(ONLY_CHAR_HAS_LENGTH, "catalog manager", "attribute构造函数", "只有char类型才能有长度，int和float没有长度");
	m_name = name;
	m_type = type;
	m_char_num = char_num;
	m_primary = primary;
	m_unique = unique;
	m_index = false;
}

attribute::attribute(string name, ATTRIBUTE_TYPE type, bool primary, bool unique)
{
	if (type == ATTRIBUTE_TYPE::CHAR)
		throw error(CHAR_MUST_HAVE_LENGTH, "catalog manager", "attribute构造函数", "char类型必须有长度");
	m_name = name;
	m_type = type;
	m_primary = primary;
	m_unique = unique;
	m_index = false;
}

attribute::~attribute()
{
}

string attribute::get_name()
{
	return m_name;
}

ATTRIBUTE_TYPE attribute::get_type()
{
	return m_type;
}

int attribute::get_char_num()
{
	if (m_type != ATTRIBUTE_TYPE::CHAR)
		//throw error(ONLY_CHAR_HAS_LENGTH, "catalog manager", "get_char_num", "无法返回，只有char类型才能有长度，int和float没有长度");
		return 0;
	return m_char_num;
}

bool attribute::is_primary()
{
	return m_primary;
}

bool attribute::is_unique()
{
	return m_unique;
}

bool attribute::is_index()
{
	return m_index;
}
string attribute::get_index()
{
	return m_index_name;
}
void attribute::index_off()
{
	m_index = false;
	m_index_name.clear();
}

void attribute::index_on(string index_name)
{
	m_index = true;
	if (index_name.size() == 0)
		throw error(INDEX_NAME_CANT_BE_BLANK, "catalog manager", "index_on", "index的名字不能够为空");
	m_index_name = index_name;
}

ostream& operator<<(ostream& out, attribute attr)
{
	out << attr.m_name << " ";
	if (ATTRIBUTE_TYPE::CHAR == attr.m_type)
		out << "CHAR " << attr.m_char_num << " ";
	else if (ATTRIBUTE_TYPE::INT == attr.m_type)
		out << "INT ";
	else if (ATTRIBUTE_TYPE::FLOAT == attr.m_type)
		out << "FLOAT ";
	if (attr.m_primary)
		out << "P ";
	else
		out << "N ";
	if (attr.m_unique)
		out << "U ";
	else
		out << "N ";
	if (attr.m_index)
		out << "I " << attr.m_index_name;
	else
		out << "N ";
	out << endl;

	return out;
}

void attribute::set_name(string name)
{
	m_name = name;
}
void attribute::set_type(ATTRIBUTE_TYPE type)
{
	m_type = type;
}
void attribute::set_char_num(int num)
{
	if (m_type == CHAR)
	{
		if (num > 0 && num <= 255)
			m_char_num = num;
		else
			throw error(0, "catalog manager", "set_char_num", "char 类型的大小应该大于等于1小于等于255");
	}
	else
		throw error(0, "catalog manager", "set_char_num", "该属性不是char类型,无法设置char_num的大小");
}
void attribute::set_primary(bool p)
{
	m_primary = p;
}
void attribute::set_unique(bool u)
{
	m_unique = u;
}

void attribute::clear()
{
	m_name.clear();
	m_type = INT;
	m_char_num = 0;
	m_primary = false;
	m_unique = false;
	m_index = false;
	m_index_name.clear();
}