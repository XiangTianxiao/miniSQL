#define _CRT_SECURE_NO_WARNINGS
//#define MAIN_1
#ifdef MAIN_1
#include <iostream>
#include "error.h"
#include "typedef.h"
#include "global.cpp"
using namespace std;

int main()
{
	try{
		/////////////////////////////////////////catalog manager的用法
		/*
		cm.load("t.tab");
		table t("t");
		t.insert_attribute(attribute("a", ATTRIBUTE_TYPE::INT, false, false));
		cm.add_table(t);
		cm.add_index("t", "a", "i");
		cm.flush("c.tab");
		*/


		////////////////////////////////////////////buffer manager的用法
		//bm.create_file("haha", FILE_TYPE::RECORD);

		///////////////////////////////////////////record_manager
		/*
		table t("test");
		t.insert_attribute(attribute("a", ATTRIBUTE_TYPE::CHAR, 4, false, false));
		record r;
		element e;
		e.m_type = ATTRIBUTE_TYPE::CHAR;
		e.m_char_num = 4;
		e.m_char = "haha";
		r.m_e.push_back(e);
		rm.insert(t, r);
		*/
		//////////////////////////////////////////////index manager
		/*
		element e;
		e.m_char = 'b';
		e.m_char_num = 1;
		e.m_type = ATTRIBUTE_TYPE::CHAR;
		BYTE b[100];
		BYTE* p = b;
		*(int*)p = 1;
		*(char*)(p + 4) = 'a';
		p += 5;
		*(int*)p = 3;
		*(char*)(p + 4) = 'c';
		p += 5;
		*(int*)p = -1;
		im.insert_in_leaf(b, 5, 5, 2, e);
		*/

		///////////////////////////////////////////index manager 测试
		/*
		index ind;
		ind.m_index = "haha";
		ind.m_char_num = 2000;
		ind.m_type = ATTRIBUTE_TYPE::CHAR;

		element a("a", 2000);
		element b("b", 2000);
		element c("c", 2000);
		element d("d", 2000);
		element e("e", 2000);
		element f("f", 2000);
		element g("g", 2000);
		element h("h", 2000);
		if (false)
		{
			im.insert_item(ind, a, 1);
			im.insert_item(ind, b, 2);
			im.insert_item(ind, c, 3);
			im.insert_item(ind, d, 4);
			im.insert_item(ind, e, 5);
			im.insert_item(ind, f, 6);
			im.insert_item(ind, g, 7);
			im.insert_item(ind, h, 8);
		}
		if (true)
		{
			cout << im.search_item(ind, a) << endl;
			cout << im.search_item(ind, b) << endl;
			cout << im.search_item(ind, c) << endl;
			cout << im.search_item(ind, d) << endl;
			cout << im.search_item(ind, e) << endl;
			cout << im.search_item(ind, f) << endl;
			cout << im.search_item(ind, g) << endl;
			cout << im.search_item(ind, h) << endl;
		}

		if (false)
		{
			index_list list;
			for (int i = 1; i < 7; i++)
			{
				block b = bm.read("haha", FILE_TYPE::INDEX, i);
				im.unserialize(list, b, ind);
				cout << "offset = " << list.offset << endl;
				switch (list.m_node_type)
				{
				case NODE_TYPE::LEAF:
					cout << "node type = " << "leaf" << endl;
					break;
				case NODE_TYPE::NONE:
					cout << "node type = " << "none" << endl;
					break;
				case NODE_TYPE::ROOT:
					cout << "node type = " << "root" << endl;
					break;
				default:
					break;
				}
				cout << "parent offset = " << list.m_parent_offset << endl;
				cout << "next offset = " << list.m_next_offset << endl;
				cout << "list = " << endl;
				for (auto iterator = list.m_list_char.begin(); iterator != list.m_list_char.end(); iterator++)
				{
					cout << "\t" << "string = " << iterator->first << "\toffset = " << iterator->second << endl;
				}
				cout << endl;
				cout << endl;
			}
		}
		*/



		/*
		index ind;
		ind.m_index = "haha";
		ind.m_char_num = 10;
		ind.m_type = ATTRIBUTE_TYPE::CHAR;
		if (false)
		{
			for (int i = 0; i < 100000; i++)
			{
				char c[11];
				sprintf(c, "t%d", i);
				element a(c, 10);
				im.insert_item(ind, a, i);
			}
		}
		if (false)
		{
			for (int i = 0; i < 200; i++)
			{
				char c[11];
				sprintf(c, "t%d", i);
				element a(c, 10);
				cout << im.search_item(ind, a) << endl;
			}
		}
		if (false)
		{
			index_list list;
			for (int i = 1; i < 21; i++)
			{
				block b = bm.read("haha", FILE_TYPE::INDEX, i);
				im.unserialize(list, b, ind);
				cout << "offset = " << list.offset << endl;
				switch (list.m_node_type)
				{
				case NODE_TYPE::LEAF:
					cout << "node type = " << "leaf" << endl;
					break;
				case NODE_TYPE::NONE:
					cout << "node type = " << "none" << endl;
					break;
				case NODE_TYPE::ROOT:
					cout << "node type = " << "root" << endl;
					break;
				default:
					break;
				}
				cout << "parent offset = " << list.m_parent_offset << endl;
				cout << "next offset = " << list.m_next_offset << endl;
				cout << "list = " << endl;
				for (auto iterator = list.m_list_char.begin(); iterator != list.m_list_char.end(); iterator++)
				{
					cout << "\t" << "string = " << iterator->first << "\toffset = " << iterator->second << endl;
				}
				cout << endl;
				cout << endl;
			}
		}

		*/
/*
		//table 的建立
		table t("haha");
		attribute a("first", ATTRIBUTE_TYPE::CHAR, 10, false, false);
		attribute b("second", ATTRIBUTE_TYPE::CHAR, 20, false, false);
		attribute c("third", ATTRIBUTE_TYPE::FLOAT, false, false);
		attribute d("forth", ATTRIBUTE_TYPE::INT, false, false);
		t.insert_attribute(a);
		t.insert_attribute(b);
		t.insert_attribute(c);
		t.insert_attribute(d);
//#define TABLE_SETUP
#ifdef TABLE_SETUP
		for (int i = 'a'; i <= 'z'; i++)
		{
			record r;
			char s[21];
			sprintf(s, "%c", i);
			element e1(s, 10);
			element e2("hahahahahahahahahah", 20);
			element e3(float(i+0.5));
			element e4(i - 'a');

			r.m_e.push_back(e1);
			r.m_e.push_back(e2);
			r.m_e.push_back(e3);
			r.m_e.push_back(e4);

			rm.insert(t, r);
		}
#endif
		vector<comparison> v_comp;
		comparison comp;
		comp.m_a = d;
		comp.m_e = element(0);
		comp.m_op_code = OP_CODE::MORE_THAN;
		v_comp.push_back(comp);
		selection sel = rm.select(t, v_comp);

		*/


		system("pause");



	}

	catch (error e)
	{
		cout << "发生错误" << endl
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

#endif