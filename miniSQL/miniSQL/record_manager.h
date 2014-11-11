#ifndef _RECORD_MANAGER_DEF_
#define _RECOED_MANAGER_DEF_

#include "table.h";
#include "index.h";
#include "fitter.h"
#include "element.h"
#include "catalog_manager.h";
#include <map>;
#include <string>
#include <vector>
#include <set>
using namespace std;

class record_manager
{
private:
	
public:
	record_manager();
	~record_manager();

	int rmInsertRecord(const string &fileName, const vector<element> &entry, const table &datatable);
	void rmDeleteWithIndex(const string fileName, int offset, const Fitter &fitter, const table &datatable);
	void rmDeleteWithoutIndex(const string fileName, const Fitter &fitter, const table &datatable);
	vector <vector <element> > rmSelectWithIndex(const string fileName, int offset, const Fitter &fitter, const table &datatable);
	vector <vector <element> > rmSelectWithoutIndex(const string fileName, const Fitter &fitter, const table &datatable);
	void rmAddIndex(const string dbName, const string BTreeName, const table &datatable, int itemIndex);
	set<int> rmGetAllOffsets(const string &fileName);
	void rmClear(const string fileName);

};


#endif