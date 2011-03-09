#ifndef _ACTRECOGNIZERINTERFACE_H_
#define _ACTRECOGNIZERINTERFACE_H_

#include <string>
#include <map>
#include <Control/VfsmPos.h>
#include <vector>
#include <Control/FsmTrans.h>


namespace CartWheel
{

class ActRecognizerInterface{
  public:
   ActRecognizerInterface();
   ~ActRecognizerInterface();
   ActRecognizerInterface(const std::string &, const std::vector<std::string> &);
   void setRecognizer(const std::string & s, const std::vector<std::string> &);
   void progress(RelationalState & r){myRec->progress(r);}
   bool getCurTerminal(){return myRec->getCurTerminal();}
   std::string getFullVerbName();
   std::string getShortVerbName(){return myRecName;};
 void reset(){myRec->reset();};  
  
  protected:
    VfsmPos * myRec;
    void construct(const std::string &, const std::vector<std::string> &);
    std::string myRecName;
    std::vector<std::string>* myObjs;
};

}

#endif
