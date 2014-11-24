#pragma once
#include "typedef.h"
#include <string>
using namespace std;

class block
{
public:
	block();
	block(string table_name, int offset, FILE_TYPE t);
	~block();

	string m_table_name;
	BYTE m_data[4096];
	int m_offset;
	FILE_TYPE m_file_type;
};

