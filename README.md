miniSQL
====

项目描述
----
本程序是数据库系统设计的课程作业，它是一个简单的SQL实现。它使用C++编写，使用visual studio 2013 组织工程文件。<br>
整个程序结构如下<br>
![](https://github.com/XiangTianxiao/miniSQL/raw/master/doc/img/structure.png)<br>

错误代码定义
----
每一个人分配不同的数字段，错误返回相应的定义<br>
如：`#define CAN_NOT_OPEN_FILE 1000`<br>
ErrorCode.h文件内有如下内容，每一个人`分别分配`一个错误代码定义头文件，以避免重复<br>
```cpp
//1000<=ERROR_CODE<2000
#include "ErrorCode1.h"
//2000<=ERROR_CODE<3000
#include "ErrorCode2.h"
//3000<=ERROR_CODE<4000
#include "ErrorCode3.h"
```

Buffer Manager
----
```cpp
//读取block，成功返回true，失败返回false
bool bRead(block* b, string filename, int offset);
//写入block，成功返回true，失败返回false
bool bwrite(block* b, string filename, int offset);
//在文件中建立新的block，也就是在文件末尾添加一块新的块，返回offset

int bNewBlock(string filename);
//删除一个块，成功true，失败返回false
//所谓的删除并非是删除一个块，之后后面的都向前移动一位，
//而是在块的开头中将之标记为删除。
bool bDeleteBlock(string filename, int offset);

//立即将缓冲区内容写到磁盘
void bFlush(string filename);
//清理干净缓冲区
void bClear(string filename);

//将一个块固定到缓冲区内，成功true，失败false
bool bPin(string filename, int offset);
//将块从缓冲区解锁
bool bUnPin(string filename, int offset);
```

磁盘文件定义
----
类型 | 扩展名 | 文本/二进制 | 描述
---- | ------ | ----------- | ----
数据块 | *.blo | bin | 数据块，存储根本数据的
数据表定义 | *.def | ascii | create table 中的定义,及其它
B+数索引 | *.ind | ? | 数据内容的B+数索引

创建一个表，产生这三个文件。表名就是文件的名称。如 create table a, 产生 a.blo a.def a.ind 三个文件<br>

* *.blo<br>
数据块的每条记录开头都有一个`标记位`，如果为true的话就说明这条记录是有效的，如果为false的话则说明这条记录无效。这样标记的话可以简单实现`懒惰删除`。<br>
![](https://github.com/XiangTianxiao/miniSQL/raw/master/doc/img/block.png)<br>
* *.def<br>
blank
* *.ind<br>
blank

catalog manager
----
![](https://github.com/XiangTianxiao/miniSQL/blob/master/doc/img/catalog_manager.png)<br>
###catalog_manager类
catalog manager 是一个类，使用的时候直接创建类即可：<br>
```cpp
catalog_manager cm;
cm.load("t.tab");
table t("t");
t.insert_attribute(attribute("a", ATTRIBUTE_TYPE::INT, false, false));
cm.add_table(t);
cm.add_index("t", "a", "i");
cm.flush("c.tab");
```

提供以下函数<br>
```cpp
	table& get_table(string name);
	attribute& get_attribute(string table_name, string attr_name);
	void add_table(table t);
	void drop_table(string name);
	void add_index(string table_name, string attr_name, string index_name);
	void drop_index(string name);
	void load(string file_name);
	void flush(string file_name);
```
###table类
```cpp
class table
{
private:
	string m_name;
	map<string, attribute> m_attr;
public:
	table();
	table(string name);
	~table();
	string get_name();
	attribute& get_attribute(string attr_name);
	void insert_attribute(attribute attr);
	friend ostream& operator<<(ostream& out, table t);
};
```
###attribute类
```cpp
class attribute
{
private:
	string m_name;
	ATTRIBUTE_TYPE m_type;
	int m_char_num;
	bool m_primary;
	bool m_unique;
	bool m_index;
	string m_index_name;
public:
	attribute();
	attribute(string name, ATTRIBUTE_TYPE type, int char_num, bool primary, bool unique);
	attribute(string name, ATTRIBUTE_TYPE type, bool primary, bool unique);
	~attribute();

	string get_name();
	ATTRIBUTE_TYPE get_type();
	int get_char_num();
	bool is_primary();
	bool is_unique();
	bool is_index();
	string get_index();

	void index_off();
	void index_on(string index_name);

	friend ostream& operator<<(ostream& out, attribute attr);
};
```
###ATTRIBUTE_TYPE枚举
```cpp
enum ATTRIBUTE_TYPE
{
	INT,
	CHAR,
	FLOAT
};
```
