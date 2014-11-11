#ifndef _ELEMENT_H_
#define _ELEMENT_H_
#include <string>
using namespace std;
class element {
public :
	int datai;
	float dataf;
	string datas;
	int type; // 0: int 1: float 2:string -1:invalid;
	bool operator < (const element &s) const ;
	bool operator == (const element &s) const ;
	bool operator > (const element &s) const ;
	bool operator >= (const element &s) const ;
	bool operator <= (const element &s) const ;
	bool operator != (const element &s) const ;	
	element(); 
	element(int d);
	element(float d); 
	element(double d);
	element(string d);
	element(const char *d);
	void print();
};

#endif
