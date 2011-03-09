#ifndef _RELATION_H_
#define _RELATION_H_

#include <string>

namespace CartWheel
{
using std::string;

class Relation{


public:
Relation(const string & n, const string & o1, const string & o2, bool b=true);
void setName(const string & s){myName = s;};
void setObj1(const string & s){obj1 = s;};
void setObj2(const string & s){obj2 = s;};
string getName()const{return myName;} ;
string getObj1()const{return obj1;} ;
string getObj2()const{return obj2;} ;
bool isPositive()const{return positive;};
bool equals(const Relation & r) const;
string toString();

protected:
string myName; //is my name
string obj1;
string obj2;
bool positive;

};

} // namespace CartWheel

#endif
