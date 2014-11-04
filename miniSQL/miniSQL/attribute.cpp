#include "attribute.h"
#include "error.h"

attribute::attribute()
{
	m_index = false;
}

attribute::attribute(string name, ATTRIBUTE_TYPE type, int char_num, bool primary, bool unique)
{
	if (type != ATTRIBUTE_TYPE::CHAR)
		throw error(ONLY_CHAR_HAS_LENGTH, "catalog manager", "attribute���캯��", "ֻ��char���Ͳ����г��ȣ�int��floatû�г���");
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
		throw error(CHAR_MUST_HAVE_LENGTH, "catalog manager", "attribute���캯��", "char���ͱ����г���");
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
		throw error(ONLY_CHAR_HAS_LENGTH, "catalog manager", "get_char_num", "�޷����أ�ֻ��char���Ͳ����г��ȣ�int��floatû�г���");
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
		throw error(INDEX_NAME_CANT_BE_BLANK, "catalog manager", "index_on", "index�����ֲ��ܹ�Ϊ��");
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