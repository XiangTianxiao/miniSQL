#pragma once
#include <string>
#include "index.h"
#include <map>
#include <vector>
#include "elements_offset.h"
#include "comparison.h"
#include "buffer_manager.h"
#include "typedef.h"
#include "index_list.h"
using namespace std;
class index_manager
{
public:
	index_manager(buffer_manager* bm);
	~index_manager();
public:
	void insert_item(index i, element e, int offset);
	void del_item(index i, element e);

	//返回数据的 "record" 在 disk 中的 offset, 不是 "index" 的 offset !!!
	int search_item(index i, element e);
	//vector<elements_offset> search_range(index i, comparison c);

private:
	buffer_manager* m_bm;

	//这个element应该出现的那个offset，当然，这个element可以不在那个块中存在
	int index_in_disk_offset(index i, element e);

	////////////////////////重写
	void unserialize(index_list& list, block b, index i);
	void serialize(index_list list, block& b);

	////////////////////////old
	BYTE& node_type(block& b);
	int& parent_offset(block& b);
	int& next_offset(block& b);

	string	block_to_string(BYTE* p, int length);
	int		block_to_int(BYTE* p);
	float	block_to_float(BYTE* p);

	void set_null(BYTE* p, int total_number, int ofkey_length);
	////////////////////////////////new
	void string_to_block(BYTE* p, string s, int char_num);
	void int_to_block(BYTE* p, int i);
	void float_to_block(BYTE* p, float f);

	
	void insert_in_parent(index_list left, NODE_TYPE left_node_type, index_list right);

	void index_root_set(string index_name, int offset);
	int index_root_get(string index_name);
	/*
	void copy_element_to_block(element e, BYTE* b);
	void insert_in_leaf(BYTE* p, int total_number, int ofkey_length, int offset, element e);

	element block_to_element(BYTE* p, element elem);
	*/
};

