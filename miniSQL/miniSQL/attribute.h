#ifndef _ATTRIBUTE_
#define _ATTRIBUTE_

#include <string>
using namespace std;

enum ATTRIBUTE_TYPE
{
	INT,
	CHAR,
	FLOAT
};

class attribute
{
private:
	string m_name;
	ATTRIBUTE_TYPE m_type;
	int m_char_num;
	bool m_primary;
	bool m_unique;
	bool m_index;
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

	void index_off();
	void index_on();
};

#endif