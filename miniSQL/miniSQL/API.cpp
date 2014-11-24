#include "API.h"
#include "error.h"

//#define UNIQUE_TEST

API::API(catalog_manager* c, record_manager* r, index_manager* i, buffer_manager* b)
{
	m_catalog = c;
	m_record = r;
	m_index = i;
	m_buffer = b;
}


API::~API()
{
}


void API::create_table(table t)
{
	m_catalog->add_table(t);
	m_catalog->flush();
	m_buffer->create_file(t.get_name(), FILE_TYPE::RECORD);
}

void API::drop_table(string name)
{
	table t = m_catalog->get_table(name);
	m_buffer->delete_file(name, FILE_TYPE::RECORD);
	//如果存在index的话,删除之
	auto index = m_catalog->get_all_index();
	for (auto i = index.begin(); i != index.end(); i++)
	{
		if (i->second.m_table == name)
		{
			//m_buffer->delete_file(i->second.m_table, FILE_TYPE::INDEX);
			drop_index(i->first);
		}
	}

	m_catalog->drop_table(name);

}

void API::create_index(string index_name, string table_name, string attr_name)
{
	m_catalog->add_index(table_name, attr_name, index_name);
	//m_buffer->create_file(index_name, FILE_TYPE::INDEX);
}

void API::drop_index(string name)
{
	m_catalog->drop_index(name);
	m_buffer->delete_file(name, FILE_TYPE::INDEX);
}

selection API::select(string table_name, vector<comparison> vector_compare)
{
	table t = m_catalog->get_table(table_name);

#ifndef UNIQUE_TEST
	selection s;
	s.m_t = t;
	//如果vector.size==1并且是等值查找的话
	if (vector_compare.size() == 1 && vector_compare.begin()->m_op_code == OP_CODE::EQUAL)
	{
		//检测index是否存在
		auto all_index = m_catalog->get_all_index();
		for (auto i = all_index.begin(); i != all_index.end(); i++)
		{
			//index存在
			//index的table name 和 attribute name 都存在
			if (i->second.m_table == table_name && i->second.m_attribute == vector_compare.begin()->m_a.get_name())
			{
				int offset = m_index->search_item(i->second, vector_compare.begin()->m_e);
				if (offset == -1)
					return s;
				record r = m_record->select_item(t, vector_compare.begin()->m_a, vector_compare.begin()->m_e, offset);
				s.m_r.push_back(r);
				return s;
			}
		}
		//index不存在，老老实实干活吧
	}
#endif // !UNIQUE_TEST


	//直接输出
	return m_record->select(t, vector_compare);
}

//该函数不具备错误检测功能，如果table与record有出入不会报错
//语法检查留给解释器?
void API::insert(string table_name, record r)
{
	table t = m_catalog->get_table(table_name);
	auto v_attr = t.get_attribute_list();

	
#ifdef UNIQUE_TEST
	for (auto i = v_attr.begin(); i != v_attr.end(); i++)
	{
		if (i->is_unique() == true)
		{
			int attr_index = t.get_attribute_index(*i);
			element e = r.m_e[attr_index];
			comparison c(*i, OP_CODE::EQUAL, e);
			vector<comparison> v_comp;
			v_comp.push_back(c);
			selection sel = select(table_name, v_comp);
			if (sel.m_r.size() != 0)
				throw error(0, "API", "insert", "已经存在相同的项！该项为unique");
		}
	}
#endif // UNIQUE_TEST

	int offset = m_record->insert(t, r);

	//插入index索引
	map<string, index> all_index = m_catalog->get_all_index();
	for (auto i = all_index.begin(); i != all_index.end(); i++)
	{
		if (i->second.m_table == table_name)
		{
			attribute attr = t.get_attribute(i->second.m_attribute);
			int index = t.get_attribute_index(attr);
			element e = r.m_e[index];
			m_index->insert_item(i->second, e, offset);
		}
	}
}

void API::del(string table_name, vector<comparison> vector_compare)
{
	table t = m_catalog->get_table(table_name);
	//如果vector.size==1并且是等值查找的话
	if (vector_compare.size() == 1 && vector_compare.begin()->m_op_code == OP_CODE::EQUAL)
	{
		//检测index是否存在
		auto all_index = m_catalog->get_all_index();
		for (auto i = all_index.begin(); i != all_index.end(); i++)
		{
			//index存在
			//index的table name 和 attribute name 都存在
			if (i->second.m_table == table_name && i->second.m_attribute == vector_compare.begin()->m_a.get_name())
			{
				m_index->del_item(i->second, vector_compare.begin()->m_e);
			}
		}
		//index不存在，老老实实干活吧
	}
	m_record->del(t, vector_compare);
}
