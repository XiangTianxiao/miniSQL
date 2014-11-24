#ifndef _TABLE_
#define _TABLE_

#include "attribute.h"
#include <vector>
#include <string>
#include <fstream>
using namespace std;
class table
{
private:
	string m_name;
	vector<attribute> m_attr;
public:
	table();
	table(string name);
	~table();
	string get_name();
	attribute& get_attribute(string attr_name);
	void insert_attribute(attribute attr);
	friend ostream& operator<<(ostream& out, table t);
	//返回这个table的一条记录的大小（字节）
	int get_record_size();
	int get_record_size_before_a(attribute a);
	int get_attribute_index(attribute a);
	const vector<attribute> get_attribute_list();
	//得到attribute在一个table中的位置，最左侧为0
	int get_attr_index(attribute a);
	void set_name(string name);
};

#endif