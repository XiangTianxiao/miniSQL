#pragma once
#include "typedef.h"
#include <string>
using namespace std;

class element
{
public:
	element();
	~element();

	int m_int;
	float m_float;
	string m_char;

	int m_char_num;

	ATTRIBUTE_TYPE m_type;

	bool operator<(element e);
	bool operator==(element e);
	bool operator>(element e);
	bool operator>=(element e);
	bool operator<=(element e);
	bool operator!=(element e);

	friend ostream& operator<<(ostream& out, element e);

	element(string s, int char_num);
	element(float f);
	element(int i);
};

