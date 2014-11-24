#include "comparison.h"


comparison::comparison()
{
}


comparison::~comparison()
{
}

comparison::comparison(attribute attr, OP_CODE op_code, element e)
{
	m_a = attr;
	m_op_code = op_code;
	m_e = e;
}