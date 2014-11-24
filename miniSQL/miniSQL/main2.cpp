#define _CRT_SECURE_NO_WARNINGS

#define MAIN2
#ifdef MAIN2

#include <iostream>
#include "error.h"
#include "typedef.h"
#include "global.cpp"
using namespace std;

int main()
{
	interp.welcome();
	interp.loop();
	system("pause");
	return 0;
}

#endif