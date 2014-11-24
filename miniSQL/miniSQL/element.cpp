#include "element.h"
#include "error.h"

element::element()
{
}


element::~element()
{
}

bool element::operator<(element e)
{
	if (m_type != e.m_type)
		throw error();
	switch (m_type)
	{
	case ATTRIBUTE_TYPE::CHAR:
		if (m_char < e.m_char)
			return true;
		else
			return false;
		break;
	case ATTRIBUTE_TYPE::FLOAT:
		if (m_float < e.m_float)
			return true;
		else
			return false;
		break;
	case ATTRIBUTE_TYPE::INT:
		if (m_int < e.m_int)
			return true;
		else
			return false;
		break;
	default:
		break;
	}
	throw error();
}
bool element::operator==(element e)
{
	if (m_type != e.m_type)
		throw error();
	switch (m_type)
	{
	case ATTRIBUTE_TYPE::CHAR:
		if (m_char == e.m_char)
			return true;
		else
			return false;
		break;
	case ATTRIBUTE_TYPE::FLOAT:
		if (m_float == e.m_float)
			return true;
		else
			return false;
		break;
	case ATTRIBUTE_TYPE::INT:
		if (m_int == e.m_int)
			return true;
		else
			return false;
		break;
	default:
		break;
	}
	throw error();
}
bool element::operator>(element e)
{
	if (m_type != e.m_type)
		throw error();
	switch (m_type)
	{
	case ATTRIBUTE_TYPE::CHAR:
		if (m_char > e.m_char)
			return true;
		else
			return false;
		break;
	case ATTRIBUTE_TYPE::FLOAT:
		if (m_float > e.m_float)
			return true;
		else
			return false;
		break;
	case ATTRIBUTE_TYPE::INT:
		if (m_int > e.m_int)
			return true;
		else
			return false;
		break;
	default:
		break;
	}
	throw error();
}

bool element::operator!=(element e)
{
	if (m_type != e.m_type)
		throw error();
	switch (m_type)
	{
	case ATTRIBUTE_TYPE::CHAR:
		if (m_char != e.m_char)
			return true;
		else
			return false;
		break;
	case ATTRIBUTE_TYPE::FLOAT:
		if (m_float != e.m_float)
			return true;
		else
			return false;
		break;
	case ATTRIBUTE_TYPE::INT:
		if (m_int != e.m_int)
			return true;
		else
			return false;
		break;
	default:
		break;
	}
	throw error();
}

bool element::operator>=(element e)
{
	if (m_type != e.m_type)
		throw error();
	switch (m_type)
	{
	case ATTRIBUTE_TYPE::CHAR:
		if (m_char >= e.m_char)
			return true;
		else
			return false;
		break;
	case ATTRIBUTE_TYPE::FLOAT:
		if (m_float >= e.m_float)
			return true;
		else
			return false;
		break;
	case ATTRIBUTE_TYPE::INT:
		if (m_int >= e.m_int)
			return true;
		else
			return false;
		break;
	default:
		break;
	}
	throw error();
}
bool element::operator<=(element e)
{
	if (m_type != e.m_type)
	throw error();
	switch (m_type)
	{
	case ATTRIBUTE_TYPE::CHAR:
		if (m_char <= e.m_char)
			return true;
		else
			return false;
		break;
	case ATTRIBUTE_TYPE::FLOAT:
		if (m_float <= e.m_float)
			return true;
		else
			return false;
		break;
	case ATTRIBUTE_TYPE::INT:
		if (m_int <= e.m_int)
			return true;
		else
			return false;
		break;
	default:
		break;
	}
	throw error();
}

element::element(string s, int char_num)
{
	m_type = ATTRIBUTE_TYPE::CHAR;
	m_char = s;
	m_char_num = char_num;
}
element::element(float f)
{
	m_type = ATTRIBUTE_TYPE::FLOAT;
	m_float = f;
}
element::element(int i)
{
	m_type = ATTRIBUTE_TYPE::INT;
	m_int = i;
}

ostream& operator<<(ostream& out, element e)
{
	switch (e.m_type)
	{
	case CHAR:
		out << e.m_char;
		break;
	case FLOAT:
		out << e.m_float;
		break;
	case INT:
		out << e.m_int;
		break;
	default:
		break;
	}
	return out;
}