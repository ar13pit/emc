#include "driveControl.h"
#include <cmath>
using namespace std;


//Main method
void DriveControl::picoDrive(Destination *dest, Flags *flags){
    emc::OdometryData odomRef;
    emc::OdometryData odomCur;
    emc::Rate r(20);

    double ref_dist;
    double angle = dest->angle;

    ref_dist = sqrt(pow(dest->x,2)+pow(dest->y,2));



    r.sleep();
    inOut->readOdometryData(odomRef);
    r.sleep();
    inOut->readOdometryData(odomCur);
    double destA = odomRef.a-angle;
    if( destA < -M_PI )
        destA = M_PI - abs(fmod(destA,M_PI));
    if( destA > M_PI )
        destA = -1*fmod(destA,M_PI);
    ref_angle = destA;

    std::cout << "flag to turn " << flags->turn << std::endl;

    std::cout << "flag to drive " << flags->drive_frw << std::endl;


    if (flags->turn) {
        if ( abs(angle) > TURN_MARGIN )
        {
            turn_ref(ref_angle, odomCur, angle, &r);
            flags->turn = false;
        }
    }
    else if(flags->drive_frw)
    {
        std::cout << "distance to go " << ref_dist << "\n";
        std::cout << "reference point (" << dest->x << " " << dest->y <<")" << "\n";
        turn_ref(ref_angle, odomCur, angle, &r);
        travel_dist(ref_dist, odomCur,&r);
        flags->drive_frw = false;
        //        picoSideDrive(angle);
    }
    return;
}

void DriveControl::turn_ref(double ref_angle, emc::OdometryData odomCur, double angle, emc::Rate *r){
    std::cout << "turning" << std::endl;

    angle < 0 ? picoTurnLeft() : picoTurnRight();

    while(abs(ref_angle - odomCur.a) > TURN_COMPLETE)
    {
        r->sleep();
        inOut->readOdometryData(odomCur);
//        std::cout << "ref Angle " << ref_angle << std::endl;
//        std::cout << "odometry angle " << odomCur.a << std::endl;

    }
    std::cout << "stopped turning" << std::endl;
    stop();

}


void DriveControl::travel_dist(double ref_dist, emc::OdometryData odomCur, emc::Rate *r){

    double meas_dist;
    meas_dist = sqrt(pow(odomCur.x,2)+pow(odomCur.y,2));
    picoForward();
    while(abs(ref_dist - meas_dist) > FRW_COMPLETE){
        r->sleep();
        inOut->readOdometryData(odomCur);

        meas_dist = sqrt(pow(odomCur.x,2)+pow(odomCur.y,2));
    }
    stop();
    std::cout << "stopped driving forward" << std::endl;
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
