//#ifndef _ERROR_
//#define _ERROR_
#pragma once
#include "ErrorCode.h"
#include <string>
using namespace std;

class error
{
private:
	int error_code;			//错误代码
	string function;		//发生错误所在的函数
	string module;			//发生错误所在的模块
	string description;		//描述
public:
	error();
	error(int error_code, string module, string function, string description);
	~error();

	void error_occurred(int error_code, string module, string function, string description);
	void clear();
	int get_error_code();
	string get_function();
	string get_module();
	string get_description();
};

//#endif