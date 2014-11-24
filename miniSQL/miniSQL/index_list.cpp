#include "index_list.h"
#include "error.h"
index_list::index_list()
{
	clear();
}

index_list::~index_list()
{
}

void index_list::clear()
{
	m_list_char.clear();
	m_list_int.clear();
	m_list_float.clear();
}

bool index_list::is_full()
{
	int total_number = 0;
	switch (m_attr_type)
	{
	case INT:
		total_number = (4096 - 9) / (4 + 4);
		if (size() == total_number)
			return true;
		else
			return false;
	case CHAR:
		total_number = (4096 - 9) / (4 + m_char_num);
		if (size() == total_number)
			return true;
		else
			return false;
		break;
	case FLOAT:
		total_number = (4096 - 9) / (4 + 4);
		if (size() == total_number)
			return true;
		else
			return false;
	default:
		throw error();
	}
}
int index_list::size()
{
	switch (m_attr_type)
	{
	case INT:
		return m_list_int.size();
	case CHAR:
		return m_list_char.size();
	case FLOAT:
		return m_list_float.size();
	default:
		throw error();
	}
}

void index_list::insert(element e, int offset)
{
	if (m_attr_type != e.m_type)
		throw error();
	if (m_attr_type == ATTRIBUTE_TYPE::CHAR && m_char_num != e.m_char_num)
	{
		throw error();
	}

	switch (m_attr_type)
	{
	case INT:
		if (m_list_int.find(e.m_int) != m_list_int.end())
		{
			if (m_list_int.find(e.m_int)->second == -1)
			{
				m_list_int.find(e.m_int)->second = offset;
				return;
			}
			throw error(0, "index_manager", "insert", "已经存在!");
		}
			m_list_int[e.m_int] = offset;
		break;
	case CHAR:
		if (m_list_char.find(e.m_char) != m_list_char.end())
		{
			if (m_list_char.find(e.m_char)->second == -1)
			{
				m_list_char.find(e.m_char)->second = offset;
				return;
			}
			throw error(0, "index_manager", "insert", "已经存在!");
		}
		m_list_char[e.m_char] = offset;
		break;
	case FLOAT:
		if (m_list_float.find(e.m_float) != m_list_float.end())
		{
			if (m_list_float.find(e.m_float)->second == -1)
			{
				m_list_float.find(e.m_float)->second = offset;
				return;
			}
			throw error(0, "index_manager", "insert", "已经存在!");
		}
		m_list_float[e.m_float] = offset;
		break;
	default:
		throw error();
	}
}

index_list index_list::right_part(int disk_offset)
{
	index_list right(*this);

	m_next_offset = disk_offset;
	right.offset = disk_offset;
	if (m_node_type == NODE_TYPE::ROOT)
	{
		right.m_node_type = NODE_TYPE::NONE;
		m_node_type = NODE_TYPE::NONE;
	}

	int total_number;
	if (m_attr_type == ATTRIBUTE_TYPE::CHAR)
		total_number = (4096 - 9) / (4 + m_char_num);
	else
		total_number = (4096 - 9) / 8;

	int count = 0;
	switch (m_attr_type)
	{
	case INT:
		for (auto i = m_list_int.begin(); i != m_list_int.end();)
		{
			if (count < (total_number + 1) / 2)
				++i;
			else
				m_list_int.erase(i++);
			count++;
		}
		count = 0;
		for (auto i = right.m_list_int.begin(); i != right.m_list_int.end();)
		{
			if (count < (total_number + 1) / 2)
				right.m_list_int.erase(i++);
			else
				++i;
			count++;
		}
		break;
	case CHAR:
		for (auto i = m_list_char.begin(); i != m_list_char.end();)
		{
			if (count < (total_number + 1) / 2)
				++i;
			else
				m_list_char.erase(i++);
			count++;
		}
		count = 0;
		for (auto i = right.m_list_char.begin(); i != right.m_list_char.end();)
		{
			if (count < (total_number + 1) / 2)
				right.m_list_char.erase(i++);
			else
				++i;
			count++;
		}
		break;
	case FLOAT:
		for (auto i = m_list_float.begin(); i != m_list_float.end();)
		{
			if (count < (total_number + 1) / 2)
				++i;
			else
				m_list_float.erase(i++);
			count++;
		}
		count = 0;
		for (auto i = right.m_list_float.begin(); i != right.m_list_float.end();)
		{
			if (count < (total_number + 1) / 2)
				right.m_list_float.erase(i++);
			else
				++i;
			count++;
		}
		break;
	default:
		break;
	}
	return right;
}

element index_list::first()
{
	if (m_attr_type == ATTRIBUTE_TYPE::CHAR)
	{
		return element(m_list_char.begin()->first, m_char_num);;
	}

	if (m_attr_type == ATTRIBUTE_TYPE::FLOAT)
	{
		return element(m_list_float.begin()->first);
	}

	if (m_attr_type == ATTRIBUTE_TYPE::INT)
	{
		return element(m_list_int.begin()->first);
	}
	throw error();
}

int index_list::find_offset(element e)
{
	switch (e.m_type)
	{
	case ATTRIBUTE_TYPE::CHAR:
		return find_offset(e.m_char);
	case ATTRIBUTE_TYPE::FLOAT:
		return find_offset(e.m_float);
	case ATTRIBUTE_TYPE::INT:
		return find_offset(e.m_int);
	default:
		break;
	}
	throw error();
}

int index_list::find_offset(string s)
{
	auto i = m_list_char.begin();
	i++;
	for (; i != m_list_char.end(); i++)
	{
		if (i->first > s)
			return (--i)->second;
	}
	return (--i)->second;
}
int index_list::find_offset(int a)
{
	auto i = m_list_int.begin();
	i++;
	for (; i != m_list_int.end(); i++)
	{
		if (i->first > a)
			return (--i)->second;
	}
	return (--i)->second;
}
int index_list::find_offset(float f)
{
	auto i = m_list_float.begin();
	i++;
	for (; i != m_list_float.end(); i++)
	{
		if (i->first > f)
			return (--i)->second;
	}
	return (--i)->second;
}


//////////////////////////////////////

int index_list::find_offset_for_record(element e)
{
	switch (e.m_type)
	{
	case ATTRIBUTE_TYPE::CHAR:
		return find_offset_for_record(e.m_char);
	case ATTRIBUTE_TYPE::FLOAT:
		return find_offset_for_record(e.m_float);
	case ATTRIBUTE_TYPE::INT:
		return find_offset_for_record(e.m_int);
	default:
		break;
	}
	throw error();
}

int index_list::find_offset_for_record(string s)
{
	for (auto i = m_list_char.begin(); i != m_list_char.end(); i++)
	{
		if (i->first == s)
			return i->second;
	}
	return -1;
}
int index_list::find_offset_for_record(int a)
{
	for (auto i = m_list_int.begin(); i != m_list_int.end(); i++)
	{
		if (i->first == a)
			return i->second;
	}
	return -1;
}
int index_list::find_offset_for_record(float f)
{
	for (auto i = m_list_float.begin(); i != m_list_float.end(); i++)
	{
		if (i->first == f)
			return i->second;
	}
	return -1;
}

vector<int> index_list::get_sons_offset()
{
	switch (m_attr_type)
	{
	case INT:
		return get_sons_offset_int();
		break;
	case CHAR:
		return get_sons_offset_char();
		break;
	case FLOAT:
		return get_sons_offset_float();
		break;
	default:
		break;
	}
	throw error();
}

vector<int> index_list::get_sons_offset_char()
{
	vector<int> v;
	for (auto i = m_list_char.begin(); i != m_list_char.end(); i++)
	{
		v.push_back(i->second);
	}
	return v;
}
vector<int> index_list::get_sons_offset_int()
{
	vector<int> v;
	for (auto i = m_list_int.begin(); i != m_list_int.end(); i++)
	{
		v.push_back(i->second);
	}
	return v;
}
vector<int> index_list::get_sons_offset_float()
{
	vector<int> v;
	for (auto i = m_list_float.begin(); i != m_list_float.end(); i++)
	{
		v.push_back(i->second);
	}
	return v;
}

void index_list::del(element e)
{
	switch (e.m_type)
	{
	case ATTRIBUTE_TYPE::CHAR:
		del_char(e.m_char);
		break;
	case ATTRIBUTE_TYPE::FLOAT:
		del_float(e.m_float);
		break;
	case ATTRIBUTE_TYPE::INT:
		del_int(e.m_int);
		break;
	default:
		break;
	}
}

void index_list::del_char(string s)
{
	auto iterator = m_list_char.find(s);
	if (iterator != m_list_char.end())
		iterator->second = -1;
	//else
	//	throw error();
}
void index_list::del_int(int i)
{
	auto iterator = m_list_int.find(i);
	if (iterator != m_list_int.end())
		iterator->second = -1;
	//else
	//	throw error();
}
void index_list::del_float(float f)
{
	auto iterator = m_list_float.find(f);
	if (iterator != m_list_float.end())
		iterator->second = -1;
	//else
	//	throw error();
}

