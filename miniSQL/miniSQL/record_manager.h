#pragma once
#include "table.h"
#include "record.h"
#include "comparison.h"
#include "selection.h"
#include "buffer_manager.h"
using namespace std;
class record_manager
{
public:
	//record_manager();
	~record_manager();
	record_manager(buffer_manager* bm);
	//该函数不具备错误检测功能，如果table与record有出入不会报错
	//返回值为这条记录在磁盘上位于block 的 offset，也就是位于第一块block还是第n块block
	int insert(table t, record r);

	selection select(table t, vector<comparison> c);
	void del(table t, vector<comparison> c);
	//单选
	record select_item(table t, attribute a, element e, int offset);
private:
	//将一条记录按顺序拷贝到p的位置
	void copy_record_to_block(BYTE* p, record r);
	record copy_block_to_record(BYTE* p, table t);
	buffer_manager* m_bm;
};

