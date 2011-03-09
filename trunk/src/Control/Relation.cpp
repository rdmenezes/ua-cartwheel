#include "Control/Relation.h"

namespace CartWheel
{
Relation::Relation(const string & n, const string & o1, const string & o2, bool b ){
  myName = n;
  obj1 = o1;
  obj2 = o2;
  positive  = b;
}

bool Relation::equals(const Relation & r) const{
  return (r.myName.compare(myName) == 0 ) && (r.obj1.compare(obj1)==0) && (r.obj2.compare(obj2) == 0 );
}

string Relation::toString(){
  string s = myName;
  s.append("(");
  s.append(obj1);
  s.append(",");
  s.append(obj2);
  s.append(")");
  return s;
}

} // namespace CartWheel
