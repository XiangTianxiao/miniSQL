#include "database.h"


database::database()
{
}


database::~database()
{
}


bool database::get_attribute(attribute& target, string table_name, string attr_name)
{
	if (t.find(table_name) == t.end())
		return false;
	else
	{
		if (t[table_name].get_attribute(target, attr_name))
			return true;
		else
			return false;
	}
}
