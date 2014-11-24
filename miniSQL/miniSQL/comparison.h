#pragma once
#include "typedef.h"
#include <vector>
#include "table.h"
#include "attribute.h"
#include "element.h"
using namespace std;

class comparison
{
public:
	comparison();
	comparison(attribute a, OP_CODE op_code, element e);
	~comparison();
	//table m_t;
	attribute m_a;
	OP_CODE m_op_code;
	element m_e;
};

