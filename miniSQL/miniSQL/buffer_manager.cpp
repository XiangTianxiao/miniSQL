#define _CRT_SECURE_NO_WARNINGS

#include "buffer_manager.h"
#include <cstdio>
#include "error.h"
buffer_manager::buffer_manager()
{
#ifdef BUFFER

	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		m_block_age[i] = -1;
		never_old[i] = false;
	}
#endif
}

buffer_manager::~buffer_manager()
{
}
#ifndef BUFFER

block buffer_manager::read(string table_name, FILE_TYPE t, int offset)
{
	block b;
	FILE* fp = NULL;
	if (t == FILE_TYPE::RECORD)
		fp = fopen((table_name + ".blo").c_str(), "rb");
	else if (t == FILE_TYPE::INDEX)
		fp = fopen((table_name + ".ind").c_str(), "rb");
	if (fp == NULL)
	{
		error e(1, "", "", "");
		throw e;
	}
	fseek(fp, offset * 4096, SEEK_SET);
	fread(b.m_data, sizeof(BYTE), 4096, fp);
	fclose(fp);
	b.m_offset = offset;
	b.m_table_name = table_name;
	b.m_file_type = t;
	return b;
}

block buffer_manager::read_last(string table_name, FILE_TYPE t)
{
	block b;
	FILE* fp = NULL;
	if (t == FILE_TYPE::RECORD)
		fp = fopen((table_name + ".blo").c_str(), "rb");
	else if (t == FILE_TYPE::INDEX)
		fp = fopen((table_name + ".ind").c_str(), "rb");
	if (fp == NULL)
	{
		error e;
		throw e;
	}

	fseek(fp, 0, SEEK_END);
	//if (ftell(fp) == 0)
	//	throw error();

	fseek(fp, -4096, SEEK_END);
	int offset = ftell(fp) / 4096;
	fread(b.m_data, sizeof(BYTE), 4096, fp);
	fclose(fp);
	b.m_offset = offset;
	b.m_table_name = table_name;
	b.m_file_type = t;
	return b;
}

void buffer_manager::write(block b)
{
	FILE* fp = NULL;
	if (b.m_file_type == FILE_TYPE::RECORD)
		fp = fopen((b.m_table_name + ".blo").c_str(), "rb+");
	else if (b.m_file_type == FILE_TYPE::INDEX)
		fp = fopen((b.m_table_name + ".ind").c_str(), "rb+");
	if (fp == NULL)
	{
		error e;
		throw e;
	}
	fseek(fp, b.m_offset * 4096, SEEK_SET);
	fwrite(b.m_data, sizeof(BYTE), 4096, fp);
	fclose(fp);
}

int buffer_manager::new_block(string table_name, FILE_TYPE t)
{
	FILE* fp = NULL;
	if (t == FILE_TYPE::RECORD)
		fp = fopen((table_name + ".blo").c_str(), "rb+");
	else if (t == FILE_TYPE::INDEX)
		fp = fopen((table_name + ".ind").c_str(), "rb+");
	if (fp == NULL)
	{
		error e;
		throw e;
	}
	fseek(fp, 0, SEEK_END);
	int offset = ftell(fp) / 4096;
	block b;
	fwrite(b.m_data, sizeof(BYTE), 4096, fp);
	fclose(fp);
	return offset;
}

void buffer_manager::flush()
{

}

int buffer_manager::table_size(string table_name, FILE_TYPE t)
{
	FILE* fp = NULL;
	if (t == FILE_TYPE::RECORD)
		fp = fopen((table_name + ".blo").c_str(), "rb");
	else if (t == FILE_TYPE::INDEX)
		fp = fopen((table_name + ".ind").c_str(), "rb");
	if (fp == NULL)
		throw error();
	fseek(fp, 0, SEEK_END);
	int offset = ftell(fp) / 4096;
	fclose(fp);
	return offset;
}

bool buffer_manager::file_exist(string table_name, FILE_TYPE t)
{
	FILE* fp = NULL;
	if (t == FILE_TYPE::RECORD)
		fp = fopen((table_name + ".blo").c_str(), "rb");
	else if (t == FILE_TYPE::INDEX)
		fp = fopen((table_name + ".ind").c_str(), "rb");
	if (fp == NULL)
		return false;
	else
	{
		fclose(fp);
		return true;
	}
}
void buffer_manager::create_file(string table_name, FILE_TYPE t)
{
	FILE* fp = NULL;
	if (t == FILE_TYPE::RECORD)
		fp = fopen((table_name + ".blo").c_str(), "wb");
	else if (t == FILE_TYPE::INDEX)
		fp = fopen((table_name + ".ind").c_str(), "wb");
	if (fp == NULL)
		throw error();
	fclose(fp);

}

void buffer_manager::delete_file(string name, FILE_TYPE t)
{
	if (t == FILE_TYPE::INDEX)
	{

		if (remove((name + ".ind").c_str()) == -1)
			return;// throw error(0, "buffer manager", "delete_file", "无法删除 " + name + ".ind 文件!");
	}
	if (t == FILE_TYPE::RECORD)
	{
		if (remove((name + ".blo").c_str()) == -1)
			return;// throw error(0, "buffer manager", "delete_file", "无法删除 " + name + ".blo 文件!");
	}
	return;
}

#endif // !BUFFER

#ifdef BUFFER

void buffer_manager::age_plus_1()
{
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		if (m_block_age[i] >= 0 && never_old[i] == false)
			m_block_age[i]++;
	}
}

bool buffer_manager::block_equal(int index, string table_name, FILE_TYPE t, int offset)
{
	if (m_block[index].m_table_name == table_name
		&& m_block[index].m_file_type == t
		&& m_block[index].m_offset == offset)
		return true;
	else
		return false;
}

void buffer_manager::refresh_block_to_buffer(block b)
{
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		if (m_block_age[i] >= 0)
		{
			if (block_equal(i, b.m_table_name, b.m_file_type, b.m_offset) == true)
			{
				age_plus_1();
				m_block_age[i] = 0;
				m_block[i] = b;
				return;
			}
		}
	}
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		if (m_block_age[i] < 0)
		{
			age_plus_1();
			m_block_age[i] = 0;
			m_block[i] = b;
			return;
		}
	}

	int max_age = 0;
	int max_index = 0;
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		if (m_block_age[i]>max_age)
		{
			max_age = m_block_age[i];
			max_index = i;
		}
	}

	FILE* fp = NULL;
	if (m_block[max_index].m_file_type == FILE_TYPE::RECORD)
		fp = fopen((m_block[max_index].m_table_name + ".blo").c_str(), "rb+");
	else if (m_block[max_index].m_file_type == FILE_TYPE::INDEX)
		fp = fopen((m_block[max_index].m_table_name + ".ind").c_str(), "rb+");
	if (fp == NULL)
	{
		error e;
		throw e;
	}
	fseek(fp, m_block[max_index].m_offset * 4096, SEEK_SET);
	fwrite(m_block[max_index].m_data, sizeof(BYTE), 4096, fp);
	fclose(fp);

	age_plus_1();
	m_block_age[max_index] = 0;
	m_block[max_index] = b;
	return;

}

block buffer_manager::read(string table_name, FILE_TYPE t, int offset)
{
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		if (m_block_age[i] >= 0)
		{
			if (block_equal(i, table_name, t, offset) == true)
			{
				age_plus_1();
				m_block_age[i] = 0;
				return m_block[i];
			}
		}
	}
	block b;
	FILE* fp = NULL;
	if (t == FILE_TYPE::RECORD)
		fp = fopen((table_name + ".blo").c_str(), "rb");
	else if (t == FILE_TYPE::INDEX)
		fp = fopen((table_name + ".ind").c_str(), "rb");
	if (fp == NULL)
	{
		error e(1, "", "", "");
		throw e;
	}
	fseek(fp, offset * 4096, SEEK_SET);
	fread(b.m_data, sizeof(BYTE), 4096, fp);
	fclose(fp);
	b.m_offset = offset;
	b.m_table_name = table_name;
	b.m_file_type = t;

	refresh_block_to_buffer(b);
	return b;
}

block buffer_manager::read_last(string table_name, FILE_TYPE t)
{
	block b;
	FILE* fp = NULL;
	if (t == FILE_TYPE::RECORD)
		fp = fopen((table_name + ".blo").c_str(), "rb");
	else if (t == FILE_TYPE::INDEX)
		fp = fopen((table_name + ".ind").c_str(), "rb");
	if (fp == NULL)
	{
		error e;
		throw e;
	}

	fseek(fp, 0, SEEK_END);
	//if (ftell(fp) == 0)
	//	throw error();

	fseek(fp, -4096, SEEK_END);
	int offset = ftell(fp) / 4096;
	fclose(fp);
	b = read(table_name, t, offset);
	/*
	fread(b.m_data, sizeof(BYTE), 4096, fp);
	fclose(fp);
	b.m_offset = offset;
	b.m_table_name = table_name;
	b.m_file_type = t;

	refresh_block_to_buffer(b);
	*/
	return b;
}

void buffer_manager::write(block b)
{
	refresh_block_to_buffer(b);
}

int buffer_manager::new_block(string table_name, FILE_TYPE t)
{
	flush();
	FILE* fp = NULL;
	if (t == FILE_TYPE::RECORD)
		fp = fopen((table_name + ".blo").c_str(), "rb+");
	else if (t == FILE_TYPE::INDEX)
		fp = fopen((table_name + ".ind").c_str(), "rb+");
	if (fp == NULL)
	{
		error e;
		throw e;
	}
	fseek(fp, 0, SEEK_END);
	int offset = ftell(fp) / 4096;
	block b;
	b.m_table_name = table_name;
	b.m_file_type = t;
	b.m_offset = offset;
	fwrite(b.m_data, sizeof(BYTE), 4096, fp);
	fclose(fp);

	refresh_block_to_buffer(b);
	return offset;
}

void buffer_manager::flush()
{
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		if (m_block_age[i] >= 0)
		{
			FILE* fp = NULL;
			if (m_block[i].m_file_type == FILE_TYPE::RECORD)
				fp = fopen((m_block[i].m_table_name + ".blo").c_str(), "rb+");
			else if (m_block[i].m_file_type == FILE_TYPE::INDEX)
				fp = fopen((m_block[i].m_table_name + ".ind").c_str(), "rb+");
			if (fp == NULL)
			{
				error e;
				throw e;
			}
			fseek(fp, m_block[i].m_offset * 4096, SEEK_SET);
			fwrite(m_block[i].m_data, sizeof(BYTE), 4096, fp);
			fclose(fp);
		}
	}
}

int buffer_manager::table_size(string table_name, FILE_TYPE t)
{
	flush();
	FILE* fp = NULL;
	if (t == FILE_TYPE::RECORD)
		fp = fopen((table_name + ".blo").c_str(), "rb");
	else if (t == FILE_TYPE::INDEX)
		fp = fopen((table_name + ".ind").c_str(), "rb");
	if (fp == NULL)
		throw error();
	fseek(fp, 0, SEEK_END);
	int offset = ftell(fp) / 4096;
	fclose(fp);
	return offset;
}

bool buffer_manager::file_exist(string table_name, FILE_TYPE t)
{
	FILE* fp = NULL;
	if (t == FILE_TYPE::RECORD)
		fp = fopen((table_name + ".blo").c_str(), "rb");
	else if (t == FILE_TYPE::INDEX)
		fp = fopen((table_name + ".ind").c_str(), "rb");
	if (fp == NULL)
		return false;
	else
	{
		fclose(fp);
		return true;
	}
}
void buffer_manager::create_file(string table_name, FILE_TYPE t)
{
	FILE* fp = NULL;
	if (t == FILE_TYPE::RECORD)
		fp = fopen((table_name + ".blo").c_str(), "wb");
	else if (t == FILE_TYPE::INDEX)
		fp = fopen((table_name + ".ind").c_str(), "wb");
	if (fp == NULL)
		throw error();
	fclose(fp);

}

void buffer_manager::delete_file(string name, FILE_TYPE t)
{
	if (t == FILE_TYPE::INDEX)
	{
		remove((name + ".ind").c_str());
		//if (remove((name + ".ind").c_str()) == -1)
		//	return;// throw error(0, "buffer manager", "delete_file", "无法删除 " + name + ".ind 文件!");
	}
	if (t == FILE_TYPE::RECORD)
	{
		remove((name + ".blo").c_str());
		//if (remove((name + ".blo").c_str()) == -1)
		//	return;// throw error(0, "buffer manager", "delete_file", "无法删除 " + name + ".blo 文件!");
	}

	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		if (m_block_age[i] >= 0)
		{
			if (m_block[i].m_table_name == name&&m_block[i].m_file_type == t)
			{
				m_block_age[i] = -1;
			}
		}
	}

	return;
}
#endif // BUFFER
