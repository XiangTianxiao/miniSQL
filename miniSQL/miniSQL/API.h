#pragma once
#include "catalog_manager.h"
#include "record_manager.h"
#include "index_manager.h"
#include <string>
using namespace std;

class API
{
public:
	API(catalog_manager*, record_manager*, index_manager*, buffer_manager*);
	~API();

private:
	catalog_manager* m_catalog;
	record_manager* m_record;
	index_manager* m_index;
	buffer_manager* m_buffer;
public:
	void create_table(table t);
	void drop_table(string name);

	void create_index(string index_name, string table_name, string attr_name);
	void drop_index(string name);
	selection select(string table_name, vector<comparison> vector_compare);
	void insert(string table_name, record r);
	//�����Ҫɾ��ȫ�����ݵĻ���ֻ��Ҫ����һ���յ�vector�Ϳ�����
	void del(string table_name, vector<comparison> vector_compare);
};

