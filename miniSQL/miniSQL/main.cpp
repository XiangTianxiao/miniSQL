#include <iostream>
#include "error.h"
#include "catalog_manager.h"
using namespace std;
int main()
{
	try{
		catalog_manager cm;
		//cm.load("t.tab");
		table t("t");
		t.insert_attribute(attribute("a", ATTRIBUTE_TYPE::INT, false, false));
		cm.add_table(t);
		cm.add_index("t", "a", "i");
		cm.flush("c.tab");
	}
	catch (error e)
	{
		cout << "·¢Éú´íÎó" << endl
			<< "error code = " << e.get_error_code() << endl
			<< "error module = " << e.get_module() << endl
			<< "error function = " << e.get_function() << endl
			<< "error description = " << e.get_description() << endl;
		
	}
	catch (...)
	{
	}

	system("pause");
	return 0;
}