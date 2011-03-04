#ifndef _RELATION_H_
#define _RELATION_H_

#include <string>

using std::string;

class Relation{


public:
Relation(string n, string o1, string o2, bool b=true);
void setName(string s ){myName = s;};
void setObj1(string s){obj1 = s;};
void setObj2(string s){obj2 = s;};
string getName(){return myName;};
string getObj1(){return obj1;};
string getObj2(){return obj2;};
bool isNegated(){return positive;};
bool equals(const Relation & r) const;
string toString();

protected:
string myName; //is my name
string obj1;
string obj2;
bool positive;

};


#endif
