#include "driveControl.h"
#include <cmath>
using namespace std;

void DriveControl::picoTurning(Destination *dest){
    emc::OdometryData odomRef;
    emc::OdometryData odomCur;
    emc::Rate r(40);

    while (!inOut->readOdometryData(odomRef)){r.sleep();}
    while (!inOut->readOdometryData(odomCur)){r.sleep();}

    double angle_cur = odomRef.a - odomCur.a;

    bool left = calibrate_angle(dest->angle);

    while (fabs(angle_cur) < fabs(ref_angle) - TURN_COMPLETE){
        if (left){
            picoTurnLeft();
        } else {
            picoTurnRight();
        }
        while (!inOut->readOdometryData(odomCur)){r.sleep();}
        r.sleep();
        angle_cur = odomRef.a - odomCur.a;
        std::cout << "absolute odomCur angle " << fabs(odomCur.a) << std::endl;
        std::cout << "absolute odomRef angle " << fabs(odomRef.a) << std::endl;
        std::cout << "absolute dest angle " << fabs(dest->angle) << std::endl;
    }
    stop();
}

void DriveControl::picoDrive(Destination * dest, bool back){
    emc::OdometryData odomRef;
    emc::OdometryData odomCur;
    emc::Rate r(40);

    while (!inOut->readOdometryData(odomRef)){r.sleep();}
    while (!inOut->readOdometryData(odomCur)){r.sleep();}

    double dist_cur = sqrt(pow((odomCur.x - odomRef.x),2) + pow((odomCur.y-odomRef.y),2));
//    double dist_ref = sqrt(pow(odomRef.x,2) + pow(odomRef.y,2));


    while (fabs(dist_cur) < fabs(dest->dist) - FRW_COMPLETE){
        if (!back){
            picoForward();
        } else {
            picoBackwards();
        }
        while (!inOut->readOdometryData(odomCur)){r.sleep();}
        dist_cur = sqrt(pow((odomCur.x-odomRef.x),2) + pow((odomCur.y-odomRef.y),2));
        r.sleep();
        std::cout << "absolute odomCur distance " << fabs(dist_cur) << std::endl;
//        std::cout << "absolute odomRef distance " << fabs(dist_ref) << std::endl;
        std::cout << "absolute dest distance " << fabs(dest->dist) << std::endl;
    }
    stop();
}

bool DriveControl::calibrate_angle(double destA){
    bool left;

    if( destA < -M_PI )
        destA = M_PI - abs(fmod(destA,M_PI));
    if( destA > M_PI )
        destA = -1*fmod(destA,M_PI);
    ref_angle = destA;

    if (destA < 0) {
        left = true;
    } else {
        left = false;
    }
}

void DriveControl::picoSideDrive(Destination *dest) {

    emc::OdometryData odomRef;
    emc::OdometryData odomCur;
    emc::Rate r(40);

    while (!inOut->readOdometryData(odomRef)){r.sleep();}
    while (!inOut->readOdometryData(odomCur)){r.sleep();}

    double x = FORWARD_SPEED*cos(dest->angle);
    double y = -1*FORWARD_SPEED*cos(0.5*M_PI-dest->angle);

    double dist_cur = sqrt(pow(odomCur.x,2)+pow(odomCur.y,2));
    double dist_ref = sqrt(pow(odomRef.x,2)+pow(odomRef.y,2));

    while(fabs(dist_ref - dist_cur) > fabs(dest->dist) - FRW_COMPLETE){
        inOut->sendBaseReference(x, y, 0.0);
        while (!inOut->readOdometryData(odomCur)){r.sleep();}
        dist_cur = sqrt(pow(odomCur.x,2)+pow(odomCur.y,2));
        r.sleep();
    }
    stop();

}


void DriveControl::picoTurnRight() {
    inOut->sendBaseReference(0.0, 0.0, ROTATE_SPEED);
    //    std::cout << "Turn Right" << endl;
    return;
}

void DriveControl::picoTurnLeft() {
    inOut->sendBaseReference(0.0, 0.0, -ROTATE_SPEED);
    //    std::cout << "Turn Left" << endl;
    return;
}

void DriveControl::picoForward() {
    inOut->sendBaseReference(FORWARD_SPEED, 0.0, 0.0);
    //    std::cout << "Drive Forward" << endl;
}

void DriveControl::picoBackwards(){
    inOut->sendBaseReference(-FORWARD_SPEED, 0.0, 0.0);
}

void DriveControl::stop() {
    inOut->readOdometryData(odom);
    inOut->sendBaseReference(0.0, 0.0, 0.0);
}


bool DriveControl::driveDecision(Low_State low_st, WorldModel * worldModel){
    Destination dest = worldModel->get_destination();
    if (low_st == PARKING){
        dest.angle = dest.angle + M_PI;
        dest.dist = ;
        DriveControl::picoTurning(&dest);
        DriveControl::picoDrive(&dest,true);
    } else if (low_st == EXPLORE_CORRIDOR){
        if (dest.angle != 0){
            DriveControl::picoSideDrive(&dest);
        } else {
            DriveControl::picoDrive(&dest,false);
        }
    } else {
        DriveControl::picoTurning(&dest);
        DriveControl::picoDrive(&dest,false);
    }
}



int main(int argc, char *argv[])
{
    // Initialization of Robot
    emc::Rate r(2); //EXECUTION_RATE
    emc::IO io;
    //    emc::OdometryData odom;

    // Transition values
    Destination dest;
    dest.dist = 0;
    dest.angle = M_PI*0.5;
    dest.x = 0;
    dest.y = 0;
    bool back = true;
    DriveControl pico_drive(&io);

    int counter = 0;


    while(io.ok()) {

        pico_drive.picoTurning(&dest);
        pico_drive.picoDrive(&dest, back);
        dest.angle = dest.angle + (M_PI*counter/10);
        dest.dist = 0.2;
        counter++;
        cin.get();
        if (counter>3){
            break;
        }

        std::cout << "-------------------------------------" << std::endl;

        r.sleep();
    }


    return 0;
}
