#include "interpreter.h"
#include "error.h"
#include "typedef.h"
#include <sstream>


interpreter::interpreter(catalog_manager* cm, API* api)
{
	m_catalog = cm;
	m_api = api;
}


interpreter::~interpreter()
{
}

void interpreter::welcome()
{

}


void interpreter::loop()
{

	while (true)
	{
		try
		{
			vector<string> in;
			string temp;
			while (cin >> temp)
			{
				bool finish = false;
				auto start = temp.begin();
				//取出();,这些符号
				for (auto i = temp.begin(); i != temp.end(); i++)
				{
					string s;
					switch (*i)
					{
					case '(':
					case ')':
					case ',':
					case '\'':
					case '=':
						if (start != i)
							in.push_back(string(start, i));
						s.push_back(*i);
						in.push_back(s);
						start = i;
						start++;
						break;
					case '<':
					case '>':
						if (start != i)
							in.push_back(string(start, i));
						s.push_back(*i);
						i++;
						if (i != temp.end() && (*i == '=' || *i == '>'))
							s.push_back(*i);
						else
							i--;
						in.push_back(s);
						start = i;
						start++;
						break;
					case ';':
						if (start != i)
							in.push_back(string(start, i));
						start = i;
						start++;
						finish = true;
						break;
					default:
						break;
					}
					if (finish == true)
						break;
				}
				if (start != temp.end())
				{
					string s = string(start, temp.end());
					if (s != ";")
						in.push_back(s);
				}
				if (finish == true)
					break;
			}
			if (is_null(in) == true)
				continue;
			if (is_quit(in))
				exit(0);
			if (is_execfile(in) == true)
			{
				execfile(in);
			}
			else
				execute_statement(in);
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
			cout << "未知错误！" << endl;
		}
	}
}

bool interpreter::is_quit(vector<string> in)
{
	//短路,所以说是安全的
	if (in.size() == 1 && in[0] == "quit")
		return true;
	return false;
}

bool interpreter::is_null(vector<string> in)
{
	if (in.size() == 0)
		return true;
	return false;
}

bool interpreter::is_execfile(vector<string> in)
{
	if (in.size() == 2 && in[0] == "execfile")
		return true;
	return false;
}

bool interpreter::is_create_table(vector<string> in)
{
	if (in.size() >= 2 && in[0] == "create" && in[1] == "table")
		return true;
	return false;
}
bool interpreter::is_drop_table(vector<string> in)
{
	if (in.size() == 3 && in[0] == "drop" && in[1] == "table")
		return true;
	return false;
}

bool interpreter::is_create_index(vector<string> in)
{
	if (in.size() >= 2 && in[0] == "create" && in[1] == "index")
		return true;
	return false;
}
bool interpreter::is_drop_index(vector<string> in)
{
	if (in.size() == 3 && in[0] == "drop" && in[1] == "index")
		return true;
	return false;
}

bool interpreter::is_select(vector<string> in)
{
	if (in.size() >= 3 && in[0] == "select" && in[1] == "*" && in[2] == "from")
		return true;
	return false;
}
bool interpreter::is_insert(vector<string> in)
{
	if (in.size() >= 2 && in[0] == "insert" && in[1] == "into")
		return true;
	return false;
}
bool interpreter::is_delete(vector<string> in)
{
	if (in.size() >= 2 && in[0] == "delete" && in[1] == "from")
		return true;
	return false;
}

void interpreter::execute_statement(vector<string> in)
{
	if (is_create_table(in))
	{
		create_table(in);
		cout << "创建表格成功！" << endl;
		return;
	}
	if (is_drop_table(in))
	{
		drop_table(in);
		cout << "删除表格成功！" << endl;
		return;
	}
	if (is_create_index(in))
	{
		create_index(in);
		cout << "创建索引成功！" << endl;
		return;
	}
	if (is_drop_index(in))
	{
		drop_index(in);
		cout << "删除索引成功！" << endl;
		return;
	}
	if (is_select(in))
	{
		select(in);
		cout << "选择执行完毕！" << endl;
		return;
	}
	if (is_insert(in))
	{
		insert(in);
		cout << "插入执行完毕！" << endl;
		return;
	}
	if (is_delete(in))
	{
		del(in);
		cout << "删除成功！" << endl;
		return;
	}
	throw error(0, "interpreter", "execute_statement", "语法错误");
}

void interpreter::create_table(vector<string> in)
{
	STATE state = CREATE;
	table t;
	attribute attr;
	for (auto i = in.begin(); i != in.end(); i++)
	{
		switch (state)
		{
		case CREATE:
			state = TABLE;
			break;
		case TABLE:
			state = TABLE_NAME;
			break;
		case TABLE_NAME:
			state = LEFT_BRACKET;
			t.set_name(*i);
			break;
		case LEFT_BRACKET:
			if (*i != "(")
				throw error(0, "interpreter", "create table", "语法错误!");
			state = ATTRIBUTE_NAME;
			break;
		case ATTRIBUTE_NAME:
			state = ATTR_TYPE;
			attr.set_name(*i);
			break;
		case ATTR_TYPE:
			attr.set_type(string_to_type(*i));
			if (attr.get_type() == CHAR)
				state = CHAR_NUM_LEFT_BRACKET;
			else
				state = UNIQUE;
			break;
		case CHAR_NUM_LEFT_BRACKET:
			if (*i != "(")
				throw error(0, "interpreter", "create table", "语法错误!");
			state = CHAR_NUM;
			break;
		case CHAR_NUM:
			state = CHAR_NUM_RIGHT_BRACKET;
			attr.set_char_num(string_to_int(*i));
			break;
		case CHAR_NUM_RIGHT_BRACKET:
			if (*i != ")")
				throw error(0, "interpreter", "create table", "语法错误!");
			state = UNIQUE;
			break;
		case UNIQUE:
			if (*i == "unique")
				attr.set_unique(true);
			else if (*i == ",")
			{
				state = NEXT_ATTR;
				t.insert_attribute(attr);
				attr.clear();
			}
			else
				throw error(0, "interpreter", "create table", "语法错误!");
			break;
		case NEXT_ATTR:
			if (*i == "primary")
				state = KEY;
			else
			{
				state = ATTR_TYPE;
				attr.set_name(*i);
			}
			break;
		case KEY:
			if (*i == "key")
				state = PRIMARY_LEFT_BRACKET;
			else
				throw error(0, "interpreter", "create table", "语法错误!");
			break;
		case PRIMARY_LEFT_BRACKET:
			if (*i == "(")
				state = PRIMARY_KEY;
			else
				throw error(0, "interpreter", "create table", "语法错误!");
			break;
		case PRIMARY_KEY:
			state = PRIMARY_RIGHT_BRACKET;
			t.get_attribute(*i).set_primary(true);
			t.get_attribute(*i).set_unique(true);
			break;
		case PRIMARY_RIGHT_BRACKET:
			if (*i == ")")
				state = RIGHT_BRACKET;
			else
				throw error(0, "interpreter", "create table", "语法错误!");
			break;
		case RIGHT_BRACKET:
			if (*i == ")")
			{
				m_api->create_table(t);
				return;
			}
			else
				throw error(0, "interpreter", "create table", "语法错误!");
			break;
		default:
			throw error(0, "interpreter", "create table", "语法错误!");
			break;
		}
	}
	throw error(0, "interpreter", "create table", "语法错误!");
}
void interpreter::drop_table(vector<string> in)
{
	m_api->drop_table(in[2]);
	return;
}
void interpreter::create_index(vector<string> in)
{
	string index_name;
	string table_name;
	string attr_name;
	STATE state = CREATE;

	table t;
	for (auto i = in.begin(); i != in.end(); i++)
	{
		switch (state)
		{
		case CREATE:
			state = IND;
			break;
		case IND:
			state = INDEX_NAME;
			break;
		case INDEX_NAME:
			index_name = *i;
			state = ON;
			break;
		case ON:
			if (*i != "on")
				throw error(0, "interpreter", "create index", "语法错误!");
			state = TABLE_NAME;
			break;
		case TABLE_NAME:
			state = LEFT_BRACKET;
			table_name = *i;
			break;
		case LEFT_BRACKET:
			if (*i != "(")
				throw error(0, "interpreter", "create index", "语法错误!");
			state = ATTRIBUTE_NAME;
			break;
		case ATTRIBUTE_NAME:
			attr_name = *i;
			state = RIGHT_BRACKET;
			break;
		case RIGHT_BRACKET:
			if (*i != ")")
				throw error(0, "interpreter", "create index", "语法错误!");
			state = FINISH;
			break;
		case FINISH:
			throw error(0, "interpreter", "create index", "语法错误!");
		default:
			break;
		}
	}
	if (state==FINISH)
		m_api->create_index(index_name, table_name, attr_name);
	else
		throw error(0, "interpreter", "create index", "语法错误!");
}
void interpreter::drop_index(vector<string> in)
{
	m_api->drop_index(in[2]);
	return;
}
void interpreter::select(vector<string> in)
{
	STATE state = SELECT;
	string table_name;
	string attr_name;
	string opcode;
	string elem;
	comparison comp;
	vector<comparison> v;

	for (auto i = in.begin(); i != in.end(); i++)
	{
		switch (state)
		{
		case SELECT:
			state = STAR;
			break;
		case STAR:
			state = FROM;
			break;
		case FROM:
			state = TABLE_NAME;
			break;
		case TABLE_NAME:
			table_name = *i;
			state = WHERE;
			break;
		case WHERE:
			if (*i != "where")
				throw error(0, "interpreter", "select", "语法错误!");
			state = ATTRIBUTE_NAME;
			break;
		case ATTRIBUTE_NAME:
			attr_name = *i;
			state = OPCODE;
			break;
		case OPCODE:
			opcode = *i;
			state = ELEMENT_OR_MARK;
			break;
		case ELEMENT_OR_MARK:
			if (*i == "\'")
			{
				state = ELEMENT;
			}
			else
			{
				elem = *i;
				state = AND;
			}
			break;
		case ELEMENT:
			elem = *i;
			state = RIGHT_MARK;
			break;
		case RIGHT_MARK:
			if (*i != "\'")
				throw error(0, "interpreter", "select", "语法错误!");
			state = AND;
			break;
		case AND:
			v.push_back(create_compare(table_name, attr_name, opcode, elem));
			state = ATTRIBUTE_NAME;
			break;
		default:
			break;
		}
	}
	if (state == WHERE)
	{
		selection sel = m_api->select(table_name, v);
		sel.m_t = m_catalog->get_table(table_name);
		print_selection(sel);
		return;
	}
	else if (state == AND)
	{
		v.push_back(create_compare(table_name, attr_name, opcode, elem));
		selection sel = m_api->select(table_name, v);
		sel.m_t = m_catalog->get_table(table_name);
		print_selection(sel);
		return;
	}
	else
		throw error(0, "interpreter", "select", "语法错误!");

}

void interpreter::print_selection(selection s)
{
	string flag(" ----------------------------------------");
	cout << flag << endl;
	table t = s.m_t;
	vector<attribute> attr = t.get_attribute_list();
	cout << "| ";
	for (auto i = attr.begin(); i != attr.end(); i++)
	{
		cout << i->get_name() << "\t";
	}
	cout << endl;

	vector<record> vr = s.m_r;

	for (auto i = vr.begin(); i != vr.end(); i++)
	{
		cout << "| ";
		vector<element> ve = i->m_e;
		for (auto j = ve.begin(); j != ve.end(); j++)
		{
			cout << *j << "\t";
		}
		cout << endl;
	}
	cout << flag << endl;
}

comparison interpreter::create_compare(string table_name, string attr_name, string opcode, string elem)
{
	attribute attr = m_catalog->get_attribute(table_name, attr_name);
	element e;
	e.m_type = attr.get_type();
	e.m_char_num = attr.get_char_num();
	switch (e.m_type)
	{
	case CHAR:
		e.m_char = elem;
		break;
	case FLOAT:
		e.m_float = string_to_float(elem);
		break;
	case INT:
		e.m_int = string_to_int(elem);
		break;
	default:
		break;
	}
	if (opcode == "<")
		return comparison(attr, OP_CODE::LESS_THAN, e);
	if (opcode == ">")
		return comparison(attr, OP_CODE::MORE_THAN, e);
	if (opcode == "=")
		return comparison(attr, OP_CODE::EQUAL, e);
	if (opcode == ">=")
		return comparison(attr, OP_CODE::MORE_AND_EQUAL, e);
	if (opcode == "<=")
		return comparison(attr, OP_CODE::LESS_AND_EQUAL, e);
	if (opcode == "<>")
		return comparison(attr, OP_CODE::NOT_EQUAL, e);
	throw error(0, "interpreter", "create_compare", "语法错误,未知的比较运算符");
}

void interpreter::insert(vector<string> in)
{
	STATE state = INSERT;
	string table_name;
	vector<string> values;
	for (auto i = in.begin(); i != in.end(); i++)
	{
		switch (state)
		{
		case INSERT:
			state = INTO;
			break;
		case INTO:
			state = TABLE_NAME;
			break;
		case TABLE_NAME:
			table_name = *i;
			state = VALUES;
			break;
		case VALUES:
			if (*i != "values")
				throw error(0, "interpreter", "insert", "语法错误!");
			state = LEFT_BRACKET;
			break;
		case LEFT_BRACKET:
			if (*i != "(")
				throw error(0, "interpreter", "insert", "语法错误!");
			state = VALUE_OR_MARK;
			break;
		case VALUE_OR_MARK:
			//如果是 'word' 这样的
			if (*i == "\'")
			{
				state = VALUE;
			}
			//如果是 word 这样的
			else
			{
				state = COMMA_OR_BRACKET;
				values.push_back(*i);
			}
			break;
		case VALUE:
			values.push_back(*i);
			state = RIGHT_MARK;
			break;
		case RIGHT_MARK:
			if (*i != "\'")
				throw error(0, "interpreter", "insert", "语法错误!");
			state = COMMA_OR_BRACKET;
			break;
		case COMMA_OR_BRACKET:
			if (*i == ",")
				state = VALUE_OR_MARK;
			else if (*i == ")")
				state = FINISH;
			break;
		case FINISH:
			throw error(0, "interpreter", "insert", "语法错误!");
			break;
		default:
			break;
		}
	}
	if (state == FINISH)
	{
		table t = m_catalog->get_table(table_name);
		vector<attribute> attr_list=t.get_attribute_list();
		if (attr_list.size() != values.size())
			throw error(0, "interpreter", "insert", "请检查插入的个数！");
		record r;
		auto i = attr_list.begin();
		auto v = values.begin();
		for (; i != attr_list.end(); i++, v++)
		{
			if (i->get_type() == CHAR)
			{
				element e(*v, i->get_char_num());
				r.m_e.push_back(e);
			}
			if (i->get_type() == FLOAT)
			{
				element e(string_to_float(*v));
				r.m_e.push_back(e);
			}
			if (i->get_type() == INT)
			{
				element e(string_to_int(*v));
				r.m_e.push_back(e);
			}
		}
		m_api->insert(table_name, r);
	}
	else
		throw error(0, "interpreter", "insert", "语法错误!");
}
void interpreter::del(vector<string> in)
{
	STATE state = DEL;
	string table_name;
	string attr_name;
	string opcode;
	string elem;
	comparison comp;
	vector<comparison> v;

	for (auto i = in.begin(); i != in.end(); i++)
	{
		switch (state)
		{
		case DEL:
			state = FROM;
			break;
		case FROM:
			state = TABLE_NAME;
			break;
		case TABLE_NAME:
			table_name = *i;
			state = WHERE;
			break;
		case WHERE:
			if (*i != "where")
				throw error(0, "interpreter", "select", "语法错误!");
			state = ATTRIBUTE_NAME;
			break;
		case ATTRIBUTE_NAME:
			attr_name = *i;
			state = OPCODE;
			break;
		case OPCODE:
			opcode = *i;
			state = ELEMENT_OR_MARK;
			break;
		case ELEMENT_OR_MARK:
			if (*i == "\'")
			{
				state = ELEMENT;
			}
			else
			{
				elem = *i;
				state = AND;
			}
			break;
		case ELEMENT:
			elem = *i;
			state = RIGHT_MARK;
			break;
		case RIGHT_MARK:
			if (*i != "\'")
				throw error(0, "interpreter", "select", "语法错误!");
			state = AND;
			break;
		case AND:
			v.push_back(create_compare(table_name, attr_name, opcode, elem));
			state = ATTRIBUTE_NAME;
			break;
		default:
			break;
		}
	}
	if (state == WHERE)
	{
		m_api->del(table_name, v);
		return;
	}
	else if (state == AND)
	{
		v.push_back(create_compare(table_name, attr_name, opcode, elem));
		 m_api->del(table_name, v);
		return;
	}
	else
		throw error(0, "interpreter", "select", "语法错误!");
}

void interpreter::execfile(vector<string> exec)
{
	string file_name = exec[1];

	ifstream infile;
	infile.open(file_name);
	if (!infile)
		throw error(CAN_NOT_OPEN_TABLE_FILE, "catalog manager", "load", "无法打开执行文件");

	bool eof = false;
	string temp;
	while (eof == false)
	{
		vector<string> in;
		while (infile >> temp)
		{
			if (infile.eof() == true)
			{
				eof = true;
				break;
			}
			bool finish = false;
			auto start = temp.begin();
			//取出();,这些符号
			for (auto i = temp.begin(); i != temp.end(); i++)
			{
				string s;
				switch (*i)
				{
				case '(':
				case ')':
				case ',':
				case '\'':
				case '=':
					if (start != i)
						in.push_back(string(start, i));
					s.push_back(*i);
					in.push_back(s);
					start = i;
					start++;
					break;
				case '<':
				case '>':
					if (start != i)
						in.push_back(string(start, i));
					s.push_back(*i);
					i++;
					if (i != temp.end() && (*i == '=' || *i == '>'))
						s.push_back(*i);
					else
						i--;
					in.push_back(s);
					start = i;
					start++;
					break;
				case ';':
					if (start != i)
						in.push_back(string(start, i));
					start = i;
					start++;
					finish = true;
					break;
				default:
					break;
				}
				if (finish == true)
					break;
			}
			if (start != temp.end())
			{
				string s = string(start, temp.end());
				if (s != ";")
					in.push_back(s);
			}
			if (finish == true)
				break;
		}
		if (eof == true)
			break;
		execute_statement(in);
	}
	infile.close();


}

ATTRIBUTE_TYPE interpreter::string_to_type(string type)
{
	if (type == "char")
		return ATTRIBUTE_TYPE::CHAR;
	if (type == "int")
		return ATTRIBUTE_TYPE::INT;
	if (type == "float")
		return ATTRIBUTE_TYPE::FLOAT;
	throw error(0, "interpreter", "string_to_type", "没有这种类型的属性，只有char int 和 float这三种");
}

int interpreter::string_to_int(string s)
{
	stringstream ss(s);
	int a;
	ss >> a;
	if (ss.eof() == true)
		return a;
	else
		throw error(0, "interpreter", "string_to_int", "格式错误，不是int。请不要再输入int的地方输入其它字符");
}

float interpreter::string_to_float(string s)
{
	stringstream ss(s);
	float a;
	ss >> a;
	if (ss.eof() == true)
		return a;
	//这里主要处理出现float不识别int的情况，懒得改了，直接套用to_int
	else
	{
		stringstream ss(s);
		int a;
		ss >> a;
		if (ss.good() == false && ss.eof() == true)
			return float(a);
		else
			throw error(0, "interpreter", "string_to_float", "格式错误，不是float。请不要再输入float的地方输入其它字符");
	}
}

