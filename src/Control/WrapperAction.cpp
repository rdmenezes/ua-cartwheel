#include <Control/WrapperAction.h>

namespace CartWheel {

    void WrapperAction::executeSetup(CartWheel3D * cw) {
        //  if(switched){
        int com = myCommand;
        if (myCommand == 10 || myCommand == 11)
            com = 0;
//        cw->setController(actorName, com); //TODO: how do we send parameters?
        if (myParams.size() > 1 && (myCommand == 0 || myCommand == 10))
            cw->doSerialBehavior((std::string)"Walk", (std::string)actorName, &Behaviors::Walk_Params(0, myTime, myParams[1], std::numeric_limits<double>::quiet_NaN()));
//            cw->setHumanSpeed(actorName, myParams[1]);
        else if (myParams.size() > 1 && myCommand == 11)
            cw->doSerialBehavior((std::string)"Walk", (std::string)actorName, &Behaviors::Walk_Params(0, myTime, std::numeric_limits<double>::quiet_NaN(), myParams[1]));
//            cw->setHumanHeading(actorName, myParams[1]);

        if (myParams.size() > 2 && myCommand == 10)
            cw->doSerialBehavior((std::string)"Walk", (std::string)actorName, &Behaviors::Walk_Params(0, myTime, std::numeric_limits<double>::quiet_NaN(), myParams[2]));
//            cw->setHumanHeading(actorName, myParams[2]);
        //    switched = false;
        //  }
        if (myCommand == 12)
            cw->doSerialBehavior((std::string)"WalkInArc", (std::string)actorName, &Behaviors::WalkInArc_Params(0, myTime, myParams[1], myParams[2]));
        if (myCommand == 13) {
            if(myParams.size() >= 4) {           
                //Old Way
//                Trajectory3d* trj = new Trajectory3d();
//                int nVertices = (myParams.size()-2)/4;
//                int nB = 0;
//                for(int i=0; i<nVertices; i++) {
//                    nB = 2 + i*4;
//                    trj->addKnot(myParams[nB+3], Point3d(myParams[nB], myParams[nB+1], myParams[nB+2]));
//                }                
//                cw->doSerialBehavior((std::string)"WalkInPath", (std::string)actorName, &Behaviors::WalkInPath_Params(0, myTime, myParams[1], trj));
                //New Way with X,Y,Z
//                Trajectory3d* trj = new Trajectory3d();
//                int nVertices = (myParams.size())/4;
//                double nTotalTime = 0;
//                double nTime = 0;
//                int nB = 0;                
//                for(int i=0; i<nVertices; i++) {
//                    nB = i*4;
//                    nTotalTime += myParams[nB+3];
//                }
//                for(int i=0; i<nVertices; i++) {
//                    nB = i*4;
//                    nTime += myParams[nB+3];
//                    trj->addKnot(nTime/nTotalTime, Point3d(myParams[nB], myParams[nB+1], myParams[nB+2]));
//                }
//                cw->doSerialBehavior((std::string)"WalkInPath", (std::string)actorName, &Behaviors::WalkInPath_Params(0, myTime, 0, trj));
                //New Way with X,Z
                Trajectory3d* trj = new Trajectory3d();
                int nVertices = (myParams.size())/3;
                double nTotalTime = 0;
                double nTime = 0;
                int nB = 0;                
                for(int i=0; i<nVertices; i++) {
                    nB = i*3;
                    nTotalTime += myParams[nB+2];
                }
                for(int i=0; i<nVertices; i++) {
                    nB = i*3;
                    nTime += myParams[nB+2];
                    trj->addKnot(nTime/nTotalTime, Point3d(myParams[nB], 1, myParams[nB+1]));
                }
                cw->doSerialBehavior((std::string)"WalkInPath", (std::string)actorName, &Behaviors::WalkInPath_Params(0, nTotalTime, 0, trj));
            } else {
                printf("WalkInPath must have params with at least one vertex of the path in 4d <x,y,z,t>.\n");
//                printf("WalkInPath must have params duration, angSpeed, and at least one vertex of the path in 4d <x,y,z,t>.\n");
            }
        }
        if (myCommand == 14) {
            cw->doSerialBehavior((std::string)"WaveHand", (std::string)actorName, &Behaviors::WaveHand_Params(0, myTime, "Right"));            
        }
        if (myCommand == 15) {
            //MoveObject_Params(double startTime, double duration, Point3d position, Vector3d orientation, Vector3d speed, Vector3d angSpeed)
            cw->doSerialBehavior((std::string)"MoveObject", (std::string)actorName, &Behaviors::MoveObject_Params(0, myTime, 
                    Point3d(myParams[1],myParams[2],myParams[3]), Vector3d(0,0,0), Vector3d(myParams[4],myParams[5],myParams[6]),
                    Vector3d(0,0,0)));            
        }

    }

    std::string WrapperAction::getActionName(int x) {
        std::map<std::string, int>::iterator iter;

        for (iter = actionMap.begin(); iter != actionMap.end(); iter++) {
            if (iter->second == x)
                return iter->first;
        }
        return "";
    }

    //this should be static, but being a bit lazy

    void WrapperAction::populateMap() {
        actionMap["walk"] = 0;
        actionMap["standStill"] = 1;
        actionMap["jog"] = 2;
        actionMap["sit"] = 3;
        actionMap["standUp"] = 4;
        actionMap["push"] = 5;
        actionMap["pull"] = 6;
        actionMap["pickup"] = 7;
        actionMap["raiseArms"] = 8;
        actionMap["kick"] = 9;
        actionMap["walkTurn"] = 10;
        actionMap["stillTurn"] = 11;
        actionMap["WalkInArc"] = 12;
        actionMap["WalkInPath"] = 13;
        actionMap["WaveHand"] = 14;
        actionMap["MoveBall"] = 15;
    }

    double WrapperAction::getPrior(std::vector<double> & params) {
        double timeM = 8.0;
        double timeS = 5.0;

        double timePD = ControlUtils::gaussianPD(timeM, timeS, params[0]);

        return timePD;
    }

    double& WrapperAction::getParam(size_t i) {
        if (i < Base::numParams())
            return Base::getParam(i);
        i -= Base::numParams();

        if (i >= myParams.size())
            throw std::out_of_range("WrapperAction::getParam -- index is out of range");

        return myParams[i];
    }

    double WrapperAction::getParam(size_t i) const {
        WrapperAction& self = const_cast<WrapperAction&> (*this);
        return self.getParam(i);
    }

} // namespace CartWheel
