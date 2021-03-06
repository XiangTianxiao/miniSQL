#ifndef _ATTRIBUTE_
#define _ATTRIBUTE_

#include <string>
#include <fstream>
#include "typedef.h"
using namespace std;



class attribute
{
private:
	string m_name;
	ATTRIBUTE_TYPE m_type;
	int m_char_num;
	bool m_primary;
	bool m_unique;
	bool m_index;
	string m_index_name;
public:
	attribute();
	attribute(string name, ATTRIBUTE_TYPE type, int char_num, bool primary, bool unique);
	attribute(string name, ATTRIBUTE_TYPE type, bool primary, bool unique);
	~attribute();

	string get_name();
	ATTRIBUTE_TYPE get_type();
	int get_char_num();
	bool is_primary();
	bool is_unique();
	bool is_index();
	string get_index();

	void index_off();
	void index_on(string index_name);

	friend ostream& operator<<(ostream& out, attribute attr);

	void set_name(string name);
	void set_type(ATTRIBUTE_TYPE type);
	void set_char_num(int num);
	void set_primary(bool p);
	void set_unique(bool u);

	void clear();
};


#endif