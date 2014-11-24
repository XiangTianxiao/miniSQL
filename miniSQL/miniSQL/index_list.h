#pragma once
#include <map>
#include "typedef.h"
#include "element.h"
#include <string>
#include <vector>
using namespace std;

class index_list
{
public:
	index_list();
	~index_list();

	NODE_TYPE m_node_type;
	int m_parent_offset;
	int m_next_offset;
	
	ATTRIBUTE_TYPE m_attr_type;
	int m_char_num;
	
	map<string, int> m_list_char;
	map<int, int> m_list_int;
	map<float, int> m_list_float;
	
	
	string m_index_name;
	int offset;
	
	void clear();
	int size();
	bool is_full();
	void insert(element e, int offset);
	void del(element e);
	void del_char(string s);
	void del_int(int i);
	void del_float(float f);


	index_list right_part(int disk_offset);

	int find_offset(element e);
	int find_offset(string s);
	int find_offset(int i);
	int find_offset(float f);

	int find_offset_for_record(element e);
	int find_offset_for_record(string s);
	int find_offset_for_record(int i);
	int find_offset_for_record(float f);

	vector<int> get_sons_offset();
	vector<int> get_sons_offset_char();
	vector<int> get_sons_offset_int();
	vector<int> get_sons_offset_float();

	element first();
};

