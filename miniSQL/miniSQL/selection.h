#pragma once
#include "record.h"
#include <vector>
#include "table.h"

using namespace std;

class selection
{
public:
	selection();
	~selection();
	vector<record> m_r;
	table m_t;
};

