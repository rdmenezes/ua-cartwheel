#include <Control/ActRecognizerInterface.h>
#include <iostream>

using std::cout;
using std::endl;

namespace CartWheel
{

ActRecognizerInterface::ActRecognizerInterface(){
   myRecName = "";
   myObjs = NULL;
}

ActRecognizerInterface::ActRecognizerInterface(const string & s, const vector<string> & objs){
  myRecName = "";
  construct(s, objs);
  myRecName = s;
}

ActRecognizerInterface::~ActRecognizerInterface(){
  if(myObjs != NULL){
   delete myRec;
   delete myObjs;
 }
}

void ActRecognizerInterface::setRecognizer(const string & s, const vector<string> & objs){
   myRecName  = s;
   construct(s, objs);
}

string ActRecognizerInterface::getFullVerbName(){
  if(myRecName.compare("") == 0)
	return "";
  string s = "";
  s.append(myRecName);
  s.append("(");
  for(int x = 0; x < myObjs->size(); x++)
     s.append((*myObjs)[x]);
  s.append(")");

 return s;
}

void ActRecognizerInterface::construct(const string & s, const vector<string> & names){
  map<int, vector<FsmTrans*>* >* transMap = NULL;
  map<int, bool> * term = NULL;
  if(s.compare("meet") == 0){
    RelationalState * rs1 = new RelationalState();
    Relation * r1 = new Relation("DistanceDecreasing",names[0], names[1]);  
    rs1->addRelation(*r1);
    transMap = new map<int, vector<FsmTrans*>* >();
    vector<FsmTrans*>* v = new vector<FsmTrans*>();
    FsmTrans * x = new FsmTrans(0,1,rs1);
    v->push_back(x);
    (*transMap)[0] = v;  
    rs1 = new RelationalState();
     r1 = new Relation("DistanceConstant",names[0], names[1]); 
    rs1->addRelation(*r1);  
     r1 = new Relation("At",names[0], names[1]);  
   rs1->addRelation(*r1);  
    x = new FsmTrans(1,2,rs1);
    v = new vector<FsmTrans*>();
    v->push_back(x);
    (*transMap)[1] = v;
     term = new map<int, bool>();
    (*term)[2] = true;
   }
   else{ return;}
  myObjs = new vector<string>(names); 
  if(!(myRecName.compare("") == 0))
      delete myRec;
  myRecName = s; 
  myRec = new VfsmPos(3, transMap, 0, term);
}
} // namespace CartWheel
