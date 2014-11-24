#include "index_manager.h"
#include "global.h"
#include "error.h"
index_manager::index_manager(buffer_manager* bm)
{
	m_bm = bm;
}


index_manager::~index_manager()
{
}

BYTE& index_manager::node_type(block& b)
{
	BYTE* p = b.m_data;
	return *p;
}

int& index_manager::parent_offset(block& b)
{
	BYTE* p = b.m_data + 1;
	return *(int*)p;
}

int& index_manager::next_offset(block& b)
{
	BYTE* p = b.m_data + 5;
	return *(int*)p;
}

string index_manager::block_to_string(BYTE* p, int length)
{
	char* temp = new char[length + 1];
	temp[length] = 0;
	memcpy(temp, p, length);
	string s = temp;
	delete[] temp;
	return s;
}

int index_manager::block_to_int(BYTE* p)
{
	return *(int*)p;
}

float index_manager::block_to_float(BYTE* p)
{
	return *(float*)p;
}

void index_manager::unserialize(index_list& list, block b, index ind)
{
	//本函数非常冗余
	list.m_index_name = ind.m_index;
	list.offset = b.m_offset;
	if (ind.m_type == ATTRIBUTE_TYPE::CHAR)
	{
		int ofkey_length = 4 + ind.m_char_num;
		int total_number = (4096 - 9) / ofkey_length;

		list.m_attr_type = ATTRIBUTE_TYPE::CHAR;
		list.m_char_num = ind.m_char_num;
		BYTE t = node_type(b);
		list.m_node_type = NODE_TYPE(t);
		list.m_parent_offset = parent_offset(b);
		list.m_next_offset = next_offset(b);

		BYTE* p = b.m_data + 9;
		list.clear();
		for (int i = 0; i < total_number; i++)
		{
			int offset = *(int*)p;
			if (offset == -1)
				return;
			string s = block_to_string(p + 4, ind.m_char_num);
			list.m_list_char[s] = offset;
			p += ofkey_length;
		}
		return;
	}
	if (ind.m_type == ATTRIBUTE_TYPE::FLOAT)
	{
		int ofkey_length = 8;
		int total_number = (4096 - 9) / ofkey_length;

		list.m_attr_type = ATTRIBUTE_TYPE::FLOAT;
		BYTE t = node_type(b);
		list.m_node_type = NODE_TYPE(t);
		list.m_parent_offset = parent_offset(b);
		list.m_next_offset = next_offset(b);

		BYTE* p = b.m_data + 9;
		list.clear();
		for (int i = 0; i < total_number; i++)
		{
			int offset = *(int*)p;
			if (offset == -1)
				return;
			float key = block_to_float(p + 4);
			list.m_list_float[key] = offset;
			p += ofkey_length;
		}
		return;
	}
	if (ind.m_type == ATTRIBUTE_TYPE::INT)
	{
		int ofkey_length = 8;
		int total_number = (4096 - 9) / ofkey_length;

		list.m_attr_type = ATTRIBUTE_TYPE::INT;
		BYTE t = node_type(b);
		list.m_node_type = NODE_TYPE(t);
		list.m_parent_offset = parent_offset(b);
		list.m_next_offset = next_offset(b);

		BYTE* p = b.m_data + 9;
		list.clear();
		for (int i = 0; i < total_number; i++)
		{
			int offset = *(int*)p;
			if (offset == -1)
				return;
			int key = block_to_int(p + 4);
			list.m_list_int[key] = offset;
			p += ofkey_length;
		}
		return;
	}
	throw error();
}

void index_manager::string_to_block(BYTE* p, string s, int char_num)
{
	for (int i = 0; i < char_num; i++)
	{
		p[i] = s.c_str()[i];
		if (s.c_str()[i] == 0)
			return;
	}
}

void index_manager::int_to_block(BYTE* p, int i)
{
	*(int*)p = i;
}

void index_manager::float_to_block(BYTE* p, float f)
{
	*(float*)p = f;
}

void index_manager::serialize(index_list list, block& b)
{
	//本函数也很冗余
	if (list.m_attr_type == ATTRIBUTE_TYPE::CHAR)
	{
		b.m_table_name = list.m_index_name;
		b.m_file_type = FILE_TYPE::INDEX;
		b.m_offset = list.offset;

		node_type(b) = list.m_node_type;
		parent_offset(b) = list.m_parent_offset;
		next_offset(b) = list.m_next_offset;

		int ofkey_length = 4 + list.m_char_num;
		int total_number = (4096 - 9) / ofkey_length;
		BYTE*p = b.m_data + 9;
		int count = 0;
		for (map<string, int>::iterator i = list.m_list_char.begin(); i != list.m_list_char.end(); i++)
		{
			int_to_block(p, i->second);
			string_to_block(p + 4, i->first, list.m_char_num);
			p += ofkey_length;
			count++;
		}
		set_null(p, total_number - count, ofkey_length);
		return;
	}
	if (list.m_attr_type == ATTRIBUTE_TYPE::FLOAT)
	{
		b.m_table_name = list.m_index_name;
		b.m_file_type = FILE_TYPE::INDEX;
		b.m_offset = list.offset;

		node_type(b) = list.m_node_type;
		parent_offset(b) = list.m_parent_offset;
		next_offset(b) = list.m_next_offset;

		int ofkey_length = 8;
		int total_number = (4096 - 9) / ofkey_length;
		BYTE*p = b.m_data + 9;
		int count = 0;
		for (map<float, int>::iterator i = list.m_list_float.begin(); i != list.m_list_float.end(); i++)
		{
			int_to_block(p, i->second);
			float_to_block(p + 4, i->first);
			p += ofkey_length;
			count++;
		}
		set_null(p, total_number - count, ofkey_length);
		return;
	}
	if (list.m_attr_type == ATTRIBUTE_TYPE::INT)
	{
		b.m_table_name = list.m_index_name;
		b.m_file_type = FILE_TYPE::INDEX;
		b.m_offset = list.offset;

		node_type(b) = list.m_node_type;
		parent_offset(b) = list.m_parent_offset;
		next_offset(b) = list.m_next_offset;

		int ofkey_length = 8;
		int total_number = (4096 - 9) / ofkey_length;
		BYTE*p = b.m_data + 9;
		int count = 0;
		for (map<int, int>::iterator i = list.m_list_int.begin(); i != list.m_list_int.end(); i++)
		{
			int_to_block(p, i->second);
			int_to_block(p + 4, i->first);
			p += ofkey_length;
			count++;
		}
		set_null(p, total_number - count, ofkey_length);
		return;
	}
	throw error();

}


void index_manager::set_null(BYTE* p, int total_number, int ofkey_length)
{
	for (int i = 0; i < total_number; i++)
	{
		*(int*)p = -1;
		p += ofkey_length;
	}
}

void index_manager::insert_item(index i, element e, int offset)
{
	//一个offset和key加起来的长度
	int ofkey_length;
	int total_number;
	if (i.m_type == ATTRIBUTE_TYPE::CHAR)
		ofkey_length = 4 + i.m_char_num;
	else
		ofkey_length = 8;
	total_number = (4096 - 9) / ofkey_length;

	//如果文件不存在的话
	if (m_bm->file_exist(i.m_index, FILE_TYPE::INDEX) == false)
	{
		m_bm->create_file(i.m_index, FILE_TYPE::INDEX);
		m_bm->new_block(i.m_index, FILE_TYPE::INDEX);
		block b1(i.m_index, 0, FILE_TYPE::INDEX);
		BYTE* p = b1.m_data;
		*(int*)p = 1;
		m_bm->write(b1);

		block b;
		index_list list;
		list.m_index_name = i.m_index;
		list.m_attr_type = e.m_type;
		list.m_char_num = e.m_char_num;

		list.offset = 1;
		list.m_parent_offset = 1;
		list.m_next_offset = -1;
		list.m_node_type = NODE_TYPE::ROOT;
		list.insert(e, offset);
		serialize(list, b);
		m_bm->write(b);
		return;
	}
	//如果文件存在的话
	else
	{
		int offset_in_disk = index_in_disk_offset(i, e);
		block b = m_bm->read(i.m_index, FILE_TYPE::INDEX, offset_in_disk);
		index_list list;
		unserialize(list, b, i);
		//如果b没有满的话
		if (list.is_full() == false)
		{
			list.insert(e, offset);
			serialize(list, b);
			m_bm->write(b);
			return;
		}
		//如果b满了的话
		NODE_TYPE left_node_type = list.m_node_type;
		list.insert(e, offset);

		int disk_offset = m_bm->new_block(i.m_index, FILE_TYPE::INDEX);
		index_list right = list.right_part(disk_offset);
		right.m_node_type = NODE_TYPE::LEAF;
		list.m_node_type = NODE_TYPE::LEAF;

		block b_left;
		serialize(list, b_left);
		m_bm->write(b_left);

		block b_right;
		serialize(right, b_right);
		m_bm->write(b_right);
		insert_in_parent(list, left_node_type, right);
	}
}


void index_manager::insert_in_parent(index_list left, NODE_TYPE left_node_type, index_list right)
{
	if (left_node_type == NODE_TYPE::ROOT)
	{
		int disk_offset = m_bm->new_block(left.m_index_name, FILE_TYPE::INDEX);
		
		index_list new_root(left);
		new_root.offset = disk_offset;
		new_root.clear();
		new_root.m_next_offset = -1;
		new_root.m_node_type = NODE_TYPE::ROOT;
		new_root.m_parent_offset = 1;
		element e1 = left.first();
		element e2 = right.first();
		
		new_root.insert(e1, left.offset);
		new_root.insert(e2, right.offset);

		block b;
		serialize(new_root, b);
		m_bm->write(b);
		index_root_set(left.m_index_name, disk_offset);

		block b1;
		left.m_parent_offset = disk_offset;
		serialize(left, b1);
		m_bm->write(b1);

		block b2;
		right.m_parent_offset = disk_offset;
		serialize(right, b2);
		m_bm->write(b2);

		return;
	}

	//得到parent这个index_list
	block b = m_bm->read(left.m_index_name, FILE_TYPE::INDEX, left.m_parent_offset);
	index_list parent;
	index ind(left.m_index_name, "", "", left.m_attr_type, left.m_char_num);
	unserialize(parent, b, ind);

	//如果parent没有满的话
	if (parent.is_full() == false)
	{
		parent.insert(right.first(), right.offset);
		serialize(parent, b);
		m_bm->write(b);
		return;
	}
	//如果parent满了的话
	NODE_TYPE parent_node_type = parent.m_node_type;
	parent.insert(right.first(), right.offset);

	int disk_offset = m_bm->new_block(parent.m_index_name, FILE_TYPE::INDEX);
	index_list parent_right = parent.right_part(disk_offset);

	block b_left;
	serialize(parent, b_left);
	m_bm->write(b_left);

	block b_right;
	serialize(parent_right, b_right);
	m_bm->write(b_right);

	vector<int> sons_offset = parent_right.get_sons_offset();
	for (auto i = sons_offset.begin(); i != sons_offset.end(); i++)
	{
		block son = m_bm->read(parent_right.m_index_name, FILE_TYPE::INDEX, *i);
		parent_offset(son) = disk_offset;
		m_bm->write(son);
	}

	insert_in_parent(parent, parent_node_type, parent_right);
}

int index_manager::index_in_disk_offset(index i, element e)
{

	int offset = index_root_get(i.m_index);
	//只有根节点的情况
	if (offset == 1)
		return 1;
	block b = m_bm->read(i.m_index, FILE_TYPE::INDEX, offset);
	index_list l;
	unserialize(l, b, i);
	while (l.m_node_type != NODE_TYPE::LEAF)
	{
		offset = l.find_offset(e);
		b = m_bm->read(i.m_index, FILE_TYPE::INDEX, offset);
		unserialize(l, b, i);
	}
	return offset;
}

void index_manager::index_root_set(string index_name, int offset)
{
	if (m_bm->file_exist(index_name, FILE_TYPE::INDEX) == false)
		throw error();
	block start = m_bm->read(index_name, FILE_TYPE::INDEX, 0);
	*(int*)start.m_data = offset;
	m_bm->write(start);
}

int index_manager::index_root_get(string index_name)
{
	if (m_bm->file_exist(index_name, FILE_TYPE::INDEX) == false)
		throw error();
	block start = m_bm->read(index_name, FILE_TYPE::INDEX, 0);
	return *(int*)start.m_data;
}

int index_manager::search_item(index i, element e)
{
	int index_offset = index_in_disk_offset(i, e);
	block b = m_bm->read(i.m_index, FILE_TYPE::INDEX, index_offset);
	index_list l;
	unserialize(l, b, i);
	return l.find_offset_for_record(e);
}

void index_manager::del_item(index i, element e)
{
	int offset = index_in_disk_offset(i, e);
	block b = m_bm->read(i.m_index, FILE_TYPE::INDEX, offset);
	index_list list;
	unserialize(list, b, i);
	list.del(e);
	serialize(list, b);
	m_bm->write(b);
}