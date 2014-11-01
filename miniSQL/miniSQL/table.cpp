#include "table.h"
#include <map>

table::table()
{
}


table::~table()
{
}

bool table::get_attribute(attribute& target, string attr_name)
{
	if (attr.find(attr_name) == attr.end())
		return false;
	else
	{
		target = attr[attr_name];
		return true;
	}
}