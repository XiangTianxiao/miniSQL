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
	//读取block

	block read(string table_name, FILE_TYPE t, int offset);
	block read_last(string table_name, FILE_TYPE t);
	int table_size(string table_name, FILE_TYPE t);
	//写入block
	void write(block b);
	//在文件中建立新的block，也就是在文件末尾添加一块新的块，返回offset
	int new_block(string table_name,FILE_TYPE t);
	//当前缓冲区全部写入
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
