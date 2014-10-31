miniSQL
====
项目描述
----
本程序是数据库系统设计的课程作业，它是一个简单的SQL实现。
使用visual 2013 编写。
![](https://github.com/XiangTianxiao/miniSQL/raw/master/doc/img/structure.svg)
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
//而是在块分配表(另一个文件)中将之标记为删除。
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
块分配表 | *.bat | bin | 这个是数据块的索引,用于确定块的使用情况
数据表定义 | .def | ? | create table 中的定义,及其它
B+数索引 | .ind | ? | 数据内容的B+数索引

创建一个表，产生这四个文件。表名就是文件的名称。
如 create table a, 产生<br>
a.blo a.bat a.def a.ind 四个文件