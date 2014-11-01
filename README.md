miniSQL
====

项目描述
----
本程序是数据库系统设计的课程作业，它是一个简单的SQL实现。它使用C++编写，使用visual studio 2013 组织工程文件。<br>
整个程序结构如下<br>
![](https://github.com/XiangTianxiao/miniSQL/raw/master/doc/img/structure.png)

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
~~块分配表~~ | ~~*.bat~~ | ~~bin~~ | ~~这个是数据块的索引,用于确定块的使用情况~~
数据表定义 | *.def | ? | create table 中的定义,及其它
B+数索引 | *.ind | ? | 数据内容的B+数索引

创建一个表，产生这~~四个~~三个文件。表名就是文件的名称。<br>
如 create table a, 产生 a.blo ~~a.bat~~ a.def a.ind ~~四个~~三个文件<br>
数据块的每条记录开头都有一个`标记位`，如果为true的话就说明这条记录是有效的，如果为false的话则说明这条记录无效。这样标记的话可以简单实现`懒惰删除`。
