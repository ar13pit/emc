#include "driveControl.hpp"

DriveControl::DriveControl (emc::IO* io, WorldModel* worldmodel) : inOut(io), WM(worldmodel), r(DRIVE_RATE) {};

void DriveControl::picoTurning () {

    while (!inOut->readOdometryData(odomRef)) { r.sleep(); }
    while (!inOut->readOdometryData(odomCur)) { r.sleep(); }

    double angle_cur = odomRef.a - odomCur.a;

    bool left = calibrate_angle();

    while (fabs(angle_cur) < fabs(ref_angle) - TURN_COMPLETE){
        if (left){
            picoTurnLeft();
        } else {
            picoTurnRight();
        }
        while (!inOut->readOdometryData(odomCur)) { r.sleep(); }
        r.sleep();
        angle_cur = odomRef.a - odomCur.a;
//        std::cout << "absolute odomCur angle " << fabs(odomCur.a) << std::endl;
//        std::cout << "absolute odomRef angle " << fabs(odomRef.a) << std::endl;
//        std::cout << "absolute dest angle " << fabs(dest.angle()) << std::endl;
    }
    picoStop();
};

void DriveControl::picoDrive(bool back){

    while (!inOut->readOdometryData(odomRef)) { r.sleep(); }
    while (!inOut->readOdometryData(odomCur)) { r.sleep(); }

    double dist_cur = sqrt(pow((odomCur.x - odomRef.x), 2) + pow((odomCur.y - odomRef.y), 2));
//    double dist_ref = sqrt(pow(odomRef.x,2) + pow(odomRef.y,2));


    while (fabs(dist_cur) < fabs(dest.dist()) - FRW_COMPLETE){
        if (!back){
            picoForward();
        } else {
            picoBackwards();
        }
        while (!inOut->readOdometryData(odomCur)){r.sleep();}
        dist_cur = sqrt(pow((odomCur.x - odomRef.x), 2) + pow((odomCur.y - odomRef.y), 2));
        r.sleep();
//        std::cout << "absolute odomCur distance " << fabs(dist_cur) << std::endl;
//        std::cout << "absolute odomRef distance " << fabs(dist_ref) << std::endl;
//        std::cout << "absolute dest distance " << fabs(dest.dist()) << std::endl;
    }
    picoStop();
};

bool DriveControl::calibrate_angle () {
    bool left;
    double destA = dest.angle();

    if( destA < -M_PI )
        destA = M_PI - abs(fmod(destA, M_PI));

    if( destA > M_PI )
        destA = -1*fmod(destA, M_PI);

    ref_angle = destA;

    if (destA < 0) {
        left = true;
    }
    else {
        left = false;
    }
};

void DriveControl::picoSideDrive () {

    while (!inOut->readOdometryData(odomRef)) { r.sleep(); }
    while (!inOut->readOdometryData(odomCur)) { r.sleep(); }

    double x = FORWARD_SPEED*cos(dest.angle());
    double y = -1*FORWARD_SPEED*cos(0.5*M_PI - dest.angle());

    double dist_cur = sqrt(pow(odomCur.x, 2) + pow(odomCur.y, 2));
    double dist_ref = sqrt(pow(odomRef.x, 2) + pow(odomRef.y, 2));

    while (fabs(dist_ref - dist_cur) > fabs(dest.dist()) - FRW_COMPLETE) {
        inOut->sendBaseReference(x, y, 0.0);
        while (!inOut->readOdometryData(odomCur)) { r.sleep(); }
        dist_cur = sqrt(pow(odomCur.x, 2) + pow(odomCur.y, 2));
        r.sleep();
    }
    picoStop();

};


void DriveControl::picoTurnRight() {
    inOut->sendBaseReference(0.0, 0.0, ROTATE_SPEED);
    //    std::cout << "Turn Right" << endl;
    return;
};

void DriveControl::picoTurnLeft() {
    inOut->sendBaseReference(0.0, 0.0, -ROTATE_SPEED);
    //    std::cout << "Turn Left" << endl;
    return;
};

void DriveControl::picoForward() {
    inOut->sendBaseReference(FORWARD_SPEED, 0.0, 0.0);
    //    std::cout << "Drive Forward" << endl;
};

void DriveControl::picoBackwards(){
    inOut->sendBaseReference(-FORWARD_SPEED, 0.0, 0.0);
};

void DriveControl::picoStop() {
    inOut->sendBaseReference(0.0, 0.0, 0.0);
};


bool DriveControl::driveDecision () {

    low_st = WM->get_currentLowState();
    dest = WM->get_destination();
    std::cout << "Destination in DriveControl" << '\n';
    std::cout << "x: " << dest.x << " y: " << dest.y << " a: " << dest.angle() << " d: " << dest.dist() << '\n';

    if (low_st == PARKING){
        dest.angle(dest.angle() + M_PI);
        dest.dist(DIST_BACKWARDS);
        picoTurning();
        picoDrive(true);
    }
    else if (low_st == EXPLORE_CORRIDOR){
        if (dest.angle() != 0){
            picoSideDrive();
        }
        else {
            picoDrive(false);
        }
    }
    else {
        picoTurning();
        picoDrive(false);
    }
};
