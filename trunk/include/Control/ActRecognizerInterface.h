#ifndef _ACTRECOGNIZERINTERFACE_H_
#define _ACTRECOGNIZERINTERFACE_H_

#include <string>
#include <map>
#include <Control/VfsmPos.h>
#include <vector>
#include <Control/FsmTrans.h>

using namespace std;

class ActRecognizerInterface{
  public:
   ActRecognizerInterface();
   ~ActRecognizerInterface();
   ActRecognizerInterface(string, const vector<string> &);
   void setRecognizer(string s, const vector<string> &);
   void progress(RelationalState & r){myRec->progress(r);}
   bool getCurTerminal(){return myRec->getCurTerminal();}
   string getFullVerbName();
  string getShortVerbName(){return myRecName;};
 void reset(){myRec->reset();};  
  
  protected:
    VfsmPos * myRec;
    void construct(string, const vector<string> &);
    string myRecName;
   vector<string>* myObjs;
};


#endif
