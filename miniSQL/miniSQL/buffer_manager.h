#pragma once
#include "typedef.h"
#include <string>
#include "block.h"
using namespace std;

#define BUFFER
#define BUFFER_SIZE 200

class buffer_manager
{
public:
	buffer_manager();
	~buffer_manager();
	//��ȡblock

	block read(string table_name, FILE_TYPE t, int offset);
	block read_last(string table_name, FILE_TYPE t);
	int table_size(string table_name, FILE_TYPE t);
	//д��block
	void write(block b);
	//���ļ��н����µ�block��Ҳ�������ļ�ĩβ���һ���µĿ飬����offset
	int new_block(string table_name,FILE_TYPE t);
	//��ǰ������ȫ��д��
	void flush();
	bool file_exist(string table_name, FILE_TYPE t);
	void create_file(string table_name, FILE_TYPE t);
	void delete_file(string name, FILE_TYPE t);
#ifdef BUFFER
private:
	int m_block_age[BUFFER_SIZE];
	block m_block[BUFFER_SIZE];
	bool never_old[BUFFER_SIZE];
private:
	void age_plus_1();
	bool block_equal(int index, string table_name, FILE_TYPE t, int offset);
	void refresh_block_to_buffer(block b);
#endif // BUFFER

};
