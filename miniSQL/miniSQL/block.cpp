#include "block.h"

block::block()
{
	memset(m_data, 0, 4096);
}


block::~block()
{
}

block::block(string table_name, int offset, FILE_TYPE t)
{
	m_table_name = table_name;
	m_offset = offset;
	m_file_type = t;
}