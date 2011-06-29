/* 
 * File:   MoveObject.h
 * Author: alfredo
 *
 * Created on Jun 27, 2011, 2:42 PM
 */

#ifndef MOVEOBJECT_H
#define	MOVEOBJECT_H

#include <string>
#include <Behaviors/Behavior.h>

using namespace CartWheel::Behaviors;

namespace CartWheel {
    class CartWheel3D;
    namespace Behaviors {

        class MoveObject : public Behavior {
        private:
            Vector3d vSpeed;
            Vector3d vOrientation;
            Vector3d vAngSpeed;
            Point3d vPosition;
            std::string sObjName;
            
        protected:
            virtual void runStep();
            virtual void onInit();
            virtual void onFinish();

        public:
            MoveObject(CartWheel3D* cw, std::string objName, MoveObject_Params* params);
        };
    }
}

#endif	/* MOVEOBJECT_H */

