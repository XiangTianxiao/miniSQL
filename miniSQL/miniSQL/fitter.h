#ifndef _FITTER_H_
#define _FITTER_H_
#include "element.h"
#include <vector>
using namespace std;
class Rule {
public:	
	int index;
	int type; // 0: < 1: <= 2: = 3: >= 4: > 5: <>
	element rhs;
	Rule(int index,int type,element rhs);
};
class Fitter {
public:
	vector <Rule> rules;
	Fitter();
	void addRule(const Rule &rule);
	bool test(const vector <element> &data) const;
};
#endif
