#pragma once
#include "API.h"
#include "catalog_manager.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class interpreter
{
public:
	interpreter(catalog_manager*, API*);
	~interpreter();
private:
	API* m_api;
	catalog_manager* m_catalog;
public:
	void welcome();
	void loop();
private:
	bool is_null(vector<string> in);
	bool is_quit(vector<string> in);
	bool is_execfile(vector<string> in);
	bool is_create_table(vector<string> in);
	bool is_drop_table(vector<string> in);
	bool is_create_index(vector<string> in);
	bool is_drop_index(vector<string> in);
	bool is_select(vector<string> in);
	bool is_insert(vector<string> in);
	bool is_delete(vector<string> in);

	void execfile(vector<string> in);

	void execute_statement(vector<string> in);

	void create_table(vector<string> in);
	void drop_table(vector<string> in);
	void create_index(vector<string> in);
	void drop_index(vector<string> in);
	void select(vector<string> in);
	void insert(vector<string> in);
	void del(vector<string> in);

	ATTRIBUTE_TYPE string_to_type(string type);
	int string_to_int(string s);
	float string_to_float(string s);

	comparison create_compare(string table_name, string attr_name, string opcode, string elem);
	void print_selection(selection);
};

