#include "attribute.h"
#include "error.h"

attribute::attribute()
{
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

void attribute::index_off()
{
	m_index = false;
}

void attribute::index_on()
{
	m_index = true;
}