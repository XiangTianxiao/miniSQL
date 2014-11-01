#ifndef _TABLE_
#define _TABLE_

#include "attribute.h"
#include <map>
#include <string>
using namespace std;
class table
{
private:
	map<string, attribute> attr;
public:
	bool get_attribute(attribute& target, string attr_name);
};

#endif