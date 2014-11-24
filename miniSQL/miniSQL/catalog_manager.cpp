#include "catalog_manager.h"
#include "error.h"
#include <fstream>

catalog_manager::catalog_manager()
{
	load();
}


catalog_manager::~catalog_manager()
{
	flush();
}

table& catalog_manager::get_table(string name)
{
	if (m_t.find(name) == m_t.end())
		throw error(NO_SUCH_TABLE, "catalog manager", "get_table", "无法找到，不存在这个名字的表");
	return m_t[name];
}

attribute& catalog_manager::get_attribute(string table_name, string attr_name)
{
	if (m_t.find(table_name) == m_t.end())
		throw error(NO_SUCH_TABLE, "catalog manager", "get_attribute", "无法找到，不存在这个名字的表");
	return m_t[table_name].get_attribute(attr_name);
}

void catalog_manager::add_table(table t)
{
	if (m_t.find(t.get_name()) != m_t.end())
		throw error(ALREADY_EXIST_SUCH_TABLE, "catalog manager", "insert_table", "无法插入，已经存在相同的表了");
	m_t[t.get_name()] = t;
}

//索引的问题，没有删除
void catalog_manager::drop_table(string name)
{
	if (m_t.find(name) == m_t.end())
		throw error(NO_SUCH_TABLE, "catalog manager", "drop_table", "无法找到，不存在这个名字的表");
	m_t.erase(name);
	remove((name + ".def").c_str());
	
}

void catalog_manager::add_index(string table_name, string attr_name, string index_name)
{
	if (m_i.find(index_name) != m_i.end())
		throw error(ALREADY_EXIST_SUCH_INDEX, "catalog manager", "add_index", "无法插入，已经存在相同的索引了");
	attribute& attr = get_attribute(table_name, attr_name);
	if (attr.is_unique() == false)
		throw error(0, "catalog manager", "add_index", "无法建立索引，因为目标属性不是unique的");
	attr.index_on(index_name);
	m_i[index_name] = index(index_name, table_name, attr_name, attr.get_type(), attr.get_char_num());
}

void catalog_manager::drop_index(string name)
{
	if (m_i.find(name) == m_i.end())
		throw error(NO_SUCH_INDEX, "catalog manager", "drop_index", "无法找到，不存在这个名字的索引");
	string table = m_i[name].m_table;
	string attr = m_i[name].m_attribute;
	m_i.erase(name);
	get_attribute(table, attr).index_off();
}

void catalog_manager::load()
{
	string file_name = "database";
	m_t.clear();
	m_i.clear();
	ifstream infile;
	infile.open(file_name);
	if (!infile)
		throw error(CAN_NOT_OPEN_TABLE_FILE, "catalog manager", "load", "无法打开table文件");
	string table_name;
	string file_path;
	while (!(infile >> table_name).eof())
	{
		infile >> file_path;
		if (table_name.size() == 0)
			throw error(TAB_FILE_SYNTAX_ERROR, "catalog manager", "load", "table文件语法错误");
		open_table(table_name, file_path);
	}
	infile.close();
}

void catalog_manager::open_table(string table_name, string file_path)
{
	ifstream infile;
	infile.open(file_path);
	if (!infile)
		throw error(CAN_NOT_OPEN_DEF_FILE, "catalog manager", "open_table", "无法打开def文件");

	string name;
	string type;
	int char_length;
	char c_primary;
	char c_unique;
	char c_index;
	string index_name;
	ATTRIBUTE_TYPE a_type;
	bool b_primary;
	bool b_unique;
	bool b_index;
	table t(table_name);
	while (infile >> name)
	{
		//读入类型
		infile >> type;
		if (type == "CHAR" || type == "char")
		{
			infile >> char_length;
			a_type = ATTRIBUTE_TYPE::CHAR;
		}
		else if (type == "INT" || type == "int")
			a_type = ATTRIBUTE_TYPE::INT;
		else if (type == "FLOAT" || type == "float")
			a_type = ATTRIBUTE_TYPE::FLOAT;
		else
			throw error(DEF_FILE_SYNTAX_ERROR, "catalog manager", "open_table", "def文件语法错误，type应为存在的int,char或者float类型");

		//读入primary unique index状态
		infile >> c_primary >> c_unique >> c_index;
		switch (c_primary)
		{
		case 'P':
		case 'p':
			b_primary = true;
			break;
		case 'N':
		case 'n':
			b_primary = false;
			break;
		default:
			throw error(DEF_FILE_SYNTAX_ERROR, "catalog manager", "open_table", "def文件语法错误，primary应该为p或者n");
			break;
		}

		switch (c_unique)
		{
		case 'U':
		case 'u':
			b_unique= true;
			break;
		case 'N':
		case 'n':
			b_unique = false;
			break;
		default:
			throw error(DEF_FILE_SYNTAX_ERROR, "catalog manager", "open_table", "def文件语法错误，unique应该为p或者n");
			break;
		}

		switch (c_index)
		{
		case 'I':
		case 'i':
			b_index = true;
			break;
		case 'N':
		case 'n':
			b_index = false;
			break;
		default:
			throw error(DEF_FILE_SYNTAX_ERROR, "catalog manager", "open_table", "def文件语法错误，index应该为p或者n");
			break;
		}
		//向表中插入一条属性
		if (a_type == ATTRIBUTE_TYPE::CHAR)
			t.insert_attribute(attribute(name, a_type, char_length, b_primary, b_unique));
		else
			t.insert_attribute(attribute(name, a_type, b_primary, b_unique));
		if (b_index)
		{
			infile >> index_name;
			t.get_attribute(name).index_on(index_name);
			//这样直接插入不太好
			if (m_i.find(index_name) != m_i.end())
				throw error(ALREADY_EXIST_SUCH_INDEX, "catalog manager", "add_index", "无法插入，已经存在相同的索引了");
			m_i[index_name] = index(index_name, table_name, name, t.get_attribute(name).get_type(), t.get_attribute(name).get_char_num());
		}
	}
	add_table(t);
	infile.close();
}

void catalog_manager::flush()
{
	string file_name = "database";
	//写入
	ofstream tab_file;
	tab_file.open(file_name);
	if (!tab_file)
		throw error(CAN_NOT_OPEN_TABLE_FILE, "catalog manager", "flush", "无法打开table文件进行写入！");
	for (map<string, table>::iterator i = m_t.begin(); i != m_t.end(); i++)
	{
		string table_name = i->first;
		tab_file << table_name << " " << table_name << ".def" << endl;
		ofstream def_file;
		def_file.open(i->first + ".def");
		if (!def_file)
			throw error(CAN_NOT_OPEN_DEF_FILE, "catalog manager", "flush", "无法打开def文件进行写如！");
		def_file << i->second;
		def_file.close();
	}
	tab_file.close();
}

map<string, index> catalog_manager::get_all_index()
{
	return m_i;
}