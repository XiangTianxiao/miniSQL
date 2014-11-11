#include "fitter.h"
#include <cassert>
#include <cstdio>
Rule::Rule(int index,int type,element rhs):index(index),type(type),rhs(rhs) {

}
Fitter::Fitter() {

}


void Fitter::addRule(const Rule &rule) {
	rules.push_back(rule);
}

bool Fitter::test(const vector <element> &data) const{
	for (int i=0;i<rules.size();i++) {
		int index=rules[i].index;
		assert(index<data.size());
		switch (rules[i].type) {
		case 0:if (!(data[index]<rules[i].rhs)) return false;break;
		case 1:if (!(data[index]<=rules[i].rhs)) return false;break;
		case 2:if (!(data[index]==rules[i].rhs)) return false;break;
		case 3:if (!(data[index]>=rules[i].rhs)) return false;break;
		case 4:if (!(data[index]>rules[i].rhs)) return false;break;
		case 5:if (!(data[index]!=rules[i].rhs)) return false;break;				   
		default:assert(false);
		}
	}
	return true;
}
