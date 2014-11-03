#ifndef _CATALOG_MANAGER_DEF_
#define _CATALOG_MANAGER_DEF_

#include <map>
#include "table.h"
#include "index.h"
using namespace std;



class catalog_manager
{
private:
	map<string, table> m_t;
	map<string, index> m_i;
	void open_table(string table_name, string file_path);
public:
	catalog_manager();
	~catalog_manager();
	table& get_table(string name);
	attribute& get_attribute(string table_name, string attr_name);
	void add_table(table t);
	void drop_table(string name);
	void add_index(string table_name, string attr_name, string index_name);
	void drop_index(string name);
	void load(string file_name);
	void flush(string file_name);
};

#endif