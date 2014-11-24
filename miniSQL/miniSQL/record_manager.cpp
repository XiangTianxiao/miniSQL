#include "record_manager.h"
#include "buffer_manager.h"
#include "typedef.h"
#include "error.h"
//规定 0 为没有数据，1为有数据

//record_manager::record_manager()
//{
//}


record_manager::~record_manager()
{
}

record_manager::record_manager(buffer_manager* bm)
{
	m_bm = bm;
}

int record_manager::insert(table t, record r)
{
	if (m_bm->file_exist(t.get_name(), FILE_TYPE::RECORD) == false)
	{
		m_bm->create_file(t.get_name(), FILE_TYPE::RECORD);
		m_bm->new_block(t.get_name(), FILE_TYPE::RECORD);
	}
	block b = m_bm->read_last(t.get_name(), FILE_TYPE::RECORD);
	BYTE* p_data = b.m_data;
	int record_size = t.get_record_size() + 1;
	int total_number = 4096 / record_size;
	for (int i = 0; i < total_number; i++)
	{
		BYTE* p_record = p_data + i*record_size;
		if (p_record[0] == 0)
		{
			p_record[0] = 1;
			copy_record_to_block(p_record + 1, r);
			m_bm->write(b);
			return b.m_offset;
		}
	}
	int offset = m_bm->new_block(t.get_name(), FILE_TYPE::RECORD);
	b = m_bm->read(t.get_name(), FILE_TYPE::RECORD, offset);
	BYTE* p_record = b.m_data;
	p_record[0] = 1;
	copy_record_to_block(p_record + 1, r);
	m_bm->write(b);
	return b.m_offset;
}

void record_manager::copy_record_to_block(BYTE* p, record r)
{
	for (vector<element>::iterator i = r.m_e.begin(); i != r.m_e.end(); i++)
	{
		switch (i->m_type)
		{
		case ATTRIBUTE_TYPE::CHAR:
			memcpy(p, i->m_char.c_str(), i->m_char_num);
			p += i->m_char_num;
			break;
		case ATTRIBUTE_TYPE::FLOAT:
			memcpy(p, &i->m_float, sizeof(float));
			p += sizeof(float);
			break;
		case ATTRIBUTE_TYPE::INT:
			memcpy(p, &i->m_int, sizeof(int));
			p += sizeof(int);
			break;
		default:
			break;
		}
	}
}

selection record_manager::select(table t, vector<comparison> c)
{
	selection sel;
	sel.m_t = t.get_name();

	int record_size = t.get_record_size() + 1;
	int total_number = 4096 / record_size;

	int table_size = m_bm->table_size(t.get_name(), FILE_TYPE::RECORD);

	for (int offset = 0; offset < table_size; offset++)
	{
		block b = m_bm->read(t.get_name(), FILE_TYPE::RECORD, offset);
		BYTE* p = b.m_data;

		for (int i = 0; i < total_number; i++)
		{
			if (*p == 1)
			{
				bool match = true;
				record r = copy_block_to_record(p + 1, t);
				for (auto i_c = c.begin(); i_c != c.end(); i_c++)
				{
					int index = t.get_attribute_index(i_c->m_a);
					element e = r.m_e[index];
					switch (i_c->m_op_code)
					{
					case OP_CODE::EQUAL:
						if (e == i_c->m_e)
							;
						else
							match = false;
						break;
					case OP_CODE::LESS_THAN:
						if (e < i_c->m_e)
							;
						else
							match = false;
						break;
					case OP_CODE::MORE_THAN:
						if (e > i_c->m_e)
							;
						else
							match = false;
						break;
					case OP_CODE::MORE_AND_EQUAL:
						if (e >= i_c->m_e)
							;
						else
							match = false;
						break;
					case OP_CODE::LESS_AND_EQUAL:
						if (e <= i_c->m_e)
							;
						else
							match = false;
						break;
					case OP_CODE::NOT_EQUAL:
						if (e != i_c->m_e)
							;
						else
							match = false;
						break;
					default:
						break;
					}
					if (match == false)
						break;
				}
				if (match == true)
					sel.m_r.push_back(r);
			}
			
			p += record_size;
		}
	}
	return sel;
}

void record_manager::del(table t, vector<comparison> c)
{
	int record_size = t.get_record_size() + 1;
	int total_number = 4096 / record_size;

	int table_size = m_bm->table_size(t.get_name(), FILE_TYPE::RECORD);

	for (int offset = 0; offset < table_size; offset++)
	{
		block b = m_bm->read(t.get_name(), FILE_TYPE::RECORD, offset);
		BYTE* p = b.m_data;

		for (int i = 0; i < total_number; i++)
		{
			if (*p == 1)
			{
				bool match = true;
				record r = copy_block_to_record(p + 1, t);
				for (auto i_c = c.begin(); i_c != c.end(); i_c++)
				{
					int index = t.get_attribute_index(i_c->m_a);
					element e = r.m_e[index];
					switch (i_c->m_op_code)
					{
					case OP_CODE::EQUAL:
						if (e == i_c->m_e)
							;
						else
							match = false;
						break;
					case OP_CODE::LESS_THAN:
						if (e < i_c->m_e)
							;
						else
							match = false;
						break;
					case OP_CODE::MORE_THAN:
						if (e > i_c->m_e)
							;
						else
							match = false;
						break;
					case OP_CODE::MORE_AND_EQUAL:
						if (e >= i_c->m_e)
							;
						else
							match = false;
						break;
					case OP_CODE::LESS_AND_EQUAL:
						if (e <= i_c->m_e)
							;
						else
							match = false;
						break;
					case OP_CODE::NOT_EQUAL:
						if (e != i_c->m_e)
							;
						else
							match = false;
						break;
					default:
						break;
					}
					if (match == false)
						break;
				}
				if (match == true)
					*p = 0;
			}
			p += record_size;
		}
		m_bm->write(b);
	}
}

record record_manager::copy_block_to_record(BYTE* p, table t)
{
	vector<attribute> attr = t.get_attribute_list();
	record r;
	int target_int;
	float target_float;
	char target_char[255];
	for (vector<attribute>::iterator i = attr.begin(); i != attr.end(); i++)
	{
		element e;
		target_int = *((int*)p);
		target_float = *((float*)p);
		memset(target_char, 0, 255);
		
		switch (i->get_type())
		{
		case ATTRIBUTE_TYPE::CHAR:
			memcpy(target_char, p, i->get_char_num());
			e.m_type = ATTRIBUTE_TYPE::CHAR;
			e.m_char_num = i->get_char_num();
			e.m_char = target_char;
			p += i->get_char_num();
			break;
		case ATTRIBUTE_TYPE::FLOAT:
			e.m_type = ATTRIBUTE_TYPE::FLOAT;
			e.m_float = target_float;
			p += 4;
			break;
		case ATTRIBUTE_TYPE::INT:
			e.m_type = ATTRIBUTE_TYPE::INT;
			e.m_int = target_int;
			p += 4;
			break;
		default:
			break;
		}
		r.m_e.push_back(e);
	}
	return r;
}

record record_manager::select_item(table t, attribute a, element e, int offset)
{
	block b = m_bm->read(t.get_name(), FILE_TYPE::RECORD, offset);
	int record_size = t.get_record_size() + 1;
	int total_number = 4096 / record_size;
	BYTE* p = b.m_data;
	record r;
	int attr_index = t.get_attr_index(a);
	for (int i = 0; i < total_number; i++)
	{
		if (*p == 1)
		{
			r = copy_block_to_record(p + 1, t);
			element target_element = r.m_e[attr_index];
			if (target_element.m_type == e.m_type)
			{
				switch (e.m_type)
				{
				case ATTRIBUTE_TYPE::CHAR:
					if (target_element.m_char == e.m_char)
						return r;
					break;
				case ATTRIBUTE_TYPE::FLOAT:
					if (target_element.m_float == e.m_float)
						return r;
					break;
				case ATTRIBUTE_TYPE::INT:
					if (target_element.m_int == e.m_int)
						return r;
					break;
				default:
					break;
				}
			}
		}
		p += record_size;
	}
	throw error(0, "r_m", "sel_item", "不存在所要找的记录\n如果出现了本信息的话，应该是因为曾经进行了区间删除操作，导致index没有删除");
}


