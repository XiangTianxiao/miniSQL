
#ifndef _CATALOG_MANAGER_
#define _CATALOG_MANAGER_

#include "database.h"
#include <string>
using namespace std;

bool add_table(database& d, string table_name, table t);
bool drop_table(database& d, string table_name);
bool add_index(database& d, string table_name, string attribute_name, string index_name);
bool drop_index(database&d, string index_name);
void catalog_init();
void catalog_flush();

#endif /*_CATALOG_MANAGER_*/