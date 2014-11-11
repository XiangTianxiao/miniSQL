#include "error.h";
#include "record_manager.h";
#include "table.h";
#include "index.h";
#include "fitter.h";
#include "element.h";
#include "Buffer.h";
#include "block.h";
#include <map>;
#include <string>;
#include <vector>;
#include <set>;
#include <list>;
#include <array>
#include <stdarg.h>
#include <sstream>
#include "assert.h";
using namespace std;

map <string, map <int, int> >  blockStatus;
enum Comparison{ Lt, Le, Gt, Ge, Eq, Ne };
//stants for less than, less equal, greater than, greater equal, equal, not equal respectivly
class Condition{
public:
	Comparison op;
	int columnNum;
	string value;
};
class blockStatusSaver{
public:
	~blockStatusSaver() {
		for (map <string, map<int, int> >::iterator it = blockStatus.begin(); it != blockStatus.end(); it++) {
			string fileName = it->first;
			FILE *fp = fopen((fileName + ".blo").c_str(), "w");
			assert(fp);
			for (map<int, int>::iterator i = blockStatus[fileName].begin(); i != blockStatus[fileName].end(); i++) {
				fprintf(fp, "%d %d\n", i->first, i->second);
			}
			fclose(fp);
		}
	}
}bss;





set<int> rmGetAllOffsets(const string &fileName) {
	loadBlockStatus(fileName);
	set <int> ret;
	for (map<int, int>::iterator it = blockStatus[fileName].begin(); it != blockStatus[fileName].end(); it++) {
		ret.insert(it->first);
	}
	return ret;

}

void rmClear(const string fileName) {
	if (blockStatus.find(fileName) != blockStatus.end())
		blockStatus.erase(blockStatus.find(fileName));
	bClear(fileName);
	remove((fileName + ".blo").c_str());
}
enum ATTRIBUTE_TYPE
{
	INT,
	CHAR,
	FLOAT
};
class blocktolist
{
public:
	blocktolist(const block &bl) ;
	~blocktolist();
	bool insert_table(int insertnum, ...);
	bool delete_table();
	bool delete_value(string rowname, vector<Condition>conditions);
	bool compare_value(ATTRIBUTE_TYPE attrtype, string value, vector< Condition >conditions);
	list<list<string>> select_all();
	list<list<string>> select_value(string rowname, vector<Condition>conditions);
private:
	block myblock;
	table mytable;
	//map<string,list> mylist;
	char tablename[256];
	list<string> attrs[32];
	int num_of_attrs;
};

blocktolist::blocktolist(const block &bl)
{
	myblock = bl;
	num_of_attrs = 0;
	string mytablename;

	catalog_manager cmanager;// = new catalog_manager();
	strncpy(tablename, bl.binblock, 256);
	mytablename = string(tablename);
	mytable = cmanager.get_table(mytablename);


}

list<list<string>> blocktolist::select_value(string rowname, vector<Condition>conditions)
{
	list<list<string>> selectresult;
	int rownum = mytable.get_table().size();
	list<string> tempattrs[32];
	list<string>::iterator ite[32];
	for (ite[0] = attrs[0].begin(); ite[0] != attrs[0].end(); ite[0]++)
	{
		if (compare_value(mytable.get_table().find(rowname)->second.get_type(), *ite[mytable.get_table().find(rowname)->second.get_index], conditions))
			//这个的rownum似乎定义错了,需要从另外的地方传入
		{
			for (int i = 0; i < rownum; i++)
			{
				tempattrs[i].push_front(*ite[i]);
			}
		}
		for (int i = 1; i < rownum; i++)
		{
			ite[i]++;
		}
	}
	for (int i = 0; i < rownum; i++)
	{
		selectresult.push_front(tempattrs[i]);
	}
	return selectresult;
}

list<list<string>> blocktolist::select_all()
{
	list<list<string>> selectresult;
	int rownum = mytable.get_table().size();
	for (int i = 0; i < rownum; i++)
	{
		selectresult.push_front(attrs[i]);
	}
	return selectresult;
}

bool blocktolist::delete_value(string rowname,vector<Condition>conditions)
{
	int rownum = mytable.get_table().size();
	list<string>::iterator ite[32];
	for (ite[0]= attrs[0].begin(); ite[0] != attrs[0].end(); ite[0]++)
	{
		if (compare_value(mytable.get_table().find(rowname)->second.get_type(), *ite[rownum], conditions))
		{
			for (int i = 0; i < rownum; i++)
			{
				attrs[i].erase(ite[i]);
			}
		}
		for (int i = 1; i < rownum; i++)
		{
			ite[i]++;
		}
	}
	return true;
}



bool blocktolist::insert_table(int insertnum, ...)
{
	string s;
	va_list vl;
	va_start(vl, insertnum);
	for (int i = 0; i < insertnum; i++)
	{
		string st = va_arg(vl, string);
		attrs[i].push_front(st);
	}
	va_end(vl);
	return true;
}
bool blocktolist::delete_table()
{
	for (int i = 0; i < 32; i++)
	{
		attrs[i].clear();
	}
}

blocktolist::~blocktolist()
{
}



bool blocktolist::compare_value(ATTRIBUTE_TYPE attrtype, string value, vector< Condition >conditions)
{
	for (int i = 0; i < conditions.size(); i++)
	{
		string string1 = value;
		string string2 = conditions[i].value;
		std::stringstream ss;
		switch (attrtype)
		{
		case INT:
			int int1, int2;
			ss << string1; ss >> int1; ss.clear();
			ss << string2; ss >> int2; ss.clear();
			switch (conditions[i].op)
			{
			case Lt:
				if (int1 >= int2) return false;
				break;
			case Le:
				if (int1 > int2) return false;
				break;
			case Gt:
				if (int1 <= int2) return false;
				break;
			case Ge:
				if (int1 < int2) return false;
				break;
			case Eq:
				if (int1 != int2) return false;
				break;
			case Ne:
				if (int1 == int2) return false;
				break;
			}
			break;
		case CHAR:
			switch (conditions[i].op)
			{
			case Lt:
				if (string1 >= string2) return 0;
				break;
			case Le:
				if (string1 > string2) return 0;
				break;
			case Gt:
				if (string1 <= string2) return 0;
				break;
			case Ge:
				if (string1 < string2) return 0;
				break;
			case Eq:
				if (string1 != string2) return 0;
				break;
			case Ne:
				if (string1 == string2) return 0;
				break;
			}
			break;
		case FLOAT:
			float float1, float2;
			ss << string1; ss >> float1; ss.clear();
			ss << string2; ss >> float2; ss.clear();
			switch (conditions[i].op)
			{
			case Lt:
				if (float1 >= float2) return false;
				break;
			case Le:
				if (float1 > float2) return false;
				break;
			case Gt:
				if (float1 <= float2) return false;
				break;
			case Ge:
				if (float1 < float2) return false;
				break;
			case Eq:
				if (float1 != float2) return false;
				break;
			case Ne:
				if (float1 == float2) return false;
				break;
			}
			break;
		default:
			break;
		}
	}
	return true;
}
