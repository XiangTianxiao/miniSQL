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
	//�ú������߱������⹦�ܣ����table��record�г��벻�ᱨ��
	//����ֵΪ������¼�ڴ�����λ��block �� offset��Ҳ����λ�ڵ�һ��block���ǵ�n��block
	int insert(table t, record r);

	selection select(table t, vector<comparison> c);
	void del(table t, vector<comparison> c);
	//��ѡ
	record select_item(table t, attribute a, element e, int offset);
private:
	//��һ����¼��˳�򿽱���p��λ��
	void copy_record_to_block(BYTE* p, record r);
	record copy_block_to_record(BYTE* p, table t);
	buffer_manager* m_bm;
};

