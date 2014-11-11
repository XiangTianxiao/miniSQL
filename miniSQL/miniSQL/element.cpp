#include "element.h"
#include <cassert>
#include <cmath>
#include <cstdio>
bool element::operator < (const element &s) const {
	assert(this->type==s.type);
	switch (s.type) {
	case 0:return this->datai < s.datai;
	case 1:return this->dataf < s.dataf;
	case 2:return this->datas < s.datas;			  
	default: assert(false);
	}
}
bool element::operator == (const element &s) const {
	assert(this->type==s.type);
	switch (s.type) {
	case 0:return this->datai == s.datai;
	case 1:return fabs(this->dataf-s.dataf)<1e-7;
	case 2:return this->datas == s.datas;			  
	default: assert(false);
	}		
}
bool element::operator > (const element &s) const {
	return !(*this<s || *this==s);
}	
bool element::operator >= (const element &s) const {
	return (*this>s || *this==s);
}	
bool element::operator <= (const element &s) const {
	return (*this<s || *this==s);
}	
bool element::operator != (const element &s) const {
	return !(*this==s);
}
element::element():type(-1) {}
element::element(int d):datai(d),type(0) {}
element::element(float d):dataf(d),type(1) {}
element::element(double d):dataf(d),type(1) {}
element::element(string d):datas(d),type(2) {}	
element::element(const char *d):datas(d),type(2) {}	
void element::	 print() {
	switch (type) {
	case 0:printf("%d",datai); return;
	case 1:printf("%f",dataf); return;
	case 2:printf("%s",datas.c_str()); return;
	default: assert(false);
	}		
}
