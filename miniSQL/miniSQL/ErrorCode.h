#ifndef _ERROR_CODE_
#define _ERROR_CODE_

#include <string>
using namespace std;


#define EC_NO_ERROR 0

//本头文件只用于定义错误代码，每一个人分配不同的数字段，错误返回相应的定义
//注意，每一个define都要以EC_开头，为了便于确定是什么，EC代表error code
//e.g.
//#define EC_CAN_NOT_OPEN_FILE 1000

//1000<=ERROR_CODE<2000
#include "ErrorCode1.h"
//2000<=ERROR_CODE<3000
#include "ErrorCode2.h"
//3000<=ERROR_CODE<4000
#include "ErrorCode3.h"





#endif /*_ERROR_CODE_*/