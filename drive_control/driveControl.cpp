#include "driveControl.h"
#include "config.h"
#include <cmath>
using namespace std;

#define TURN_MARGIN 0.25*M_PI // [rad/s]
#define TURN_COMPLETE 0.1 // [rad/s]

    //Main method
    void DriveControl::picoDrive(double angle){
        emc::OdometryData odomRef;
        emc::OdometryData odomCur;
        emc::Rate r(10);

        if ( abs(angle) > TURN_MARGIN )
        {
            r.sleep();
            inOut->readOdometryData(odomRef);
            r.sleep();
            inOut->readOdometryData(odomCur);
            double destA = odomRef.a-angle;
            if( destA < -M_PI )
                destA = M_PI - abs(fmod(destA,M_PI));
            if( destA > M_PI )
                destA = -1*fmod(destA,M_PI);
            angle < 0 ? picoTurnLeft() : picoTurnRight();
            while(abs(destA - odomCur.a) > TURN_COMPLETE)
            {
                r.sleep();
                inOut->readOdometryData(odomCur);
            }
            picoForward();
        }
        else
        {
            picoSideDrive(angle);
        }
        return;
    }

    void DriveControl::picoTurnRight() {
        inOut->sendBaseReference(0.0, 0.0, ROTATE_SPEED);
        return;
    }
    
    void DriveControl::picoTurnLeft() {
        inOut->sendBaseReference(0.0, 0.0, -ROTATE_SPEED);
        return;
    }

    void DriveControl::picoForward() {
        inOut->sendBaseReference(FORWARD_SPEED, 0.0, 0.0);
    }

    void DriveControl::picoSideDrive(double angle) {
        double x = FORWARD_SPEED*cos(angle);
        double y = -1*FORWARD_SPEED*cos(0.5*M_PI-angle);
        inOut->sendBaseReference(x, y, 0.0);
    }

    void DriveControl::stop() {
        inOut->readOdometryData(odom);
        inOut->sendBaseReference(0.0, 0.0, 0.0);
    }
