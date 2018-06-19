#include "planning.h"

Planning::Planning (WorldModel* worldmodel) : WM(worldmodel) {};


Point Planning::setpointInCorridor () {

    //Move straight ahead, getAwayFromWall prevents from crashing into walls
    dest.dist(DIST_SETPOINT);
    dest.angle(0);

    return dest;
}

Point Planning::getAwayFromWall () {

    double tempAngle;
    Point dest;
    Point closestPoint = WM->get_closestPointWall();
    Low_State lowSt = WM->get_currentLowState();

    //Move sideways when PICO is inside the corridor OR
    //When closest point is at the side of PICO
    if (lowSt == EXPLORE_CORRIDOR || abs(closestPoint.angle()) > FRONTWALL_ANGLE) {
        tempAngle = closestPoint.angle() < 0 ? MOVESIDEWAYS_FACTOR : -MOVESIDEWAYS_FACTOR;
        dest.angle(tempAngle);
        dest.dist(DIST_SETPOINT);
    }
    //If closest point is in the front of PICO, then turn around
    else {
        tempAngle = closestPoint.angle() < 0 ? TURN_WHEN_OBJECT_IN_FRONT : -TURN_WHEN_OBJECT_IN_FRONT;
        dest.angle(tempAngle);
        dest.dist(0);
    }

    return dest;
}

Point Planning::getNearbyExitPoint (Room closestRoom) {

    Point destination;
    Point extPnt1 = closestRoom.exit.exitPoint1;
    Point extPnt2 = closestRoom.exit.exitPoint2;
    Point curPos = WM->get_globalPosition();

    double xMid = 0.5*(extPnt1.x + extPnt2.x);
    double yMid = 0.5*(extPnt1.y + extPnt2.y);

    //Vertical exit
    if (abs(extPnt1.x - extPnt2.x) > abs(extPnt1.y - extPnt2.y)) {
        destination.x = xMid;

        double distExtSide1 = sqrt(pow(curPos.x - xMid, 2) + pow(curPos.y -(yMid - WAIT_BEFORE_EXIT), 2));
        double distExtSide2 = sqrt(pow(curPos.x - xMid, 2) + pow(curPos.y -(yMid + WAIT_BEFORE_EXIT), 2));

        destination.y = distExtSide1 < distExtSide2 ? (yMid - WAIT_BEFORE_EXIT) : (yMid + WAIT_BEFORE_EXIT);
    }
    //Horizontal exit
    else {
        destination.y = yMid;

        double distExtSide1 = sqrt(pow(curPos.x - (xMid - WAIT_BEFORE_EXIT), 2) + pow(curPos.y - yMid, 2));
        double distExtSide2 = sqrt(pow(curPos.x - (xMid + WAIT_BEFORE_EXIT), 2) + pow(curPos.y - yMid, 2));

        destination.x = distExtSide1 < distExtSide2 ? (xMid - WAIT_BEFORE_EXIT) : (xMid + WAIT_BEFORE_EXIT);
    }

    std::cout << "Destination Point = (" << destination.x << ',' << destination.y << ')' <<std::endl;
    return destination;
}

Point Planning::driveToPoint (Point goToPoint) {

//    ///////////////////  TODO  //////////////////////////////////////
//    /// Check wheter is works when the relative angle is different
//    /// Unable to check in simulation without other classes...
//    ///
    Point curPos = WM->get_globalPosition();
    Point dest;

    double disX = goToPoint.x - curPos.x;
    double disY = goToPoint.y - curPos.y;

    if (disX > 0 && disY < 0) {
        std::cout << "First Quadrant" <<std::endl;
        dest.angle(-atan(disY/disX));
    }
    else if (disX < 0 && disY < 0) {
        std::cout << "Second Quadrant" <<std::endl;
        dest.angle(M_PI - atan(disY/disX));
    }
    else if (disX < 0 && disY > 0) {
        std::cout << "Third Quadrant" <<std::endl;
        dest.angle(M_PI - atan(disY/disX));
    }
    else if (disX > 0 && disY > 0) {
        std::cout << "Fourth Quadrant" <<std::endl;
        dest.angle(-atan(disY/disX));
    }
    dest.dist(DIST_SETPOINT);


//    dest.angle = atan(disY/disX);

    std::cout << "ANGLE = " << dest.angle() <<std::endl;

    return dest;
}

Point Planning::driveInRoom () {

    Point closestPoint = WM->get_closestPointWall();
    Point dest;

    //Turn around when front wall is too close
    if(abs(closestPoint.angle()) < FRONTWALL_ANGLE && closestPoint.dist() < DISTANCE_FROM_WALL_IN_ROOM){
        dest.angle(M_PI);
        dest.dist(DIST_SETPOINT);
    }
    else
        dest.dist(DIST_SETPOINT);

    return dest;
}

Point Planning::getStartPos(){

    Point startPos;
    return startPos;
}

Point Planning::parkPico () {

    Point corr_end = WM->get_pointStraightAhead();
    Point dest;

    //Pico is backwards to the back wall
    dest.angle(M_PI - corr_end.angle());

    //Move the distance to the back wall - 5cm
    dest.dist(corr_end.dist() - 0.05);

    return dest;
}

Point Planning::getThroughtExitPoint (Room roomFromMapping) {

    Point navigateTo;

    double closestX = INFINITY;
    double closestY = INFINITY;

    std::vector<Exit> allDetectedExits = WM->get_allDetectedExits();

    Point extPntMap1 = roomFromMapping.exit.exitPoint1;
    Point extPntMap2 = roomFromMapping.exit.exitPoint2;

    //Get the middle point of the exit from the mapping
    double xMidMap = 0.5*(extPntMap1.x + extPntMap2.x);
    double yMidMap = 0.5*(extPntMap1.y + extPntMap2.y);

    //In case multiple exits are found, select the correct one
    for (int i = 0; i < allDetectedExits.size(); ++i) {
        Point extPntDet1 = allDetectedExits[i].exitPoint1;
        Point extPntDet2 = allDetectedExits[i].exitPoint2;

        //Get the middle point of the exit from the detection
        double xMidDet = 0.5*(extPntDet1.x + extPntDet2.x);
        double yMidDet = 0.5*(extPntDet1.y + extPntDet2.y);

        if(abs(xMidMap - xMidDet) + abs(yMidMap - xMidDet) < abs(closestX) + abs(closestY)){
            closestX = xMidDet;
            closestY = yMidDet;
        }
    }

    navigateTo.x = closestX;
    navigateTo.y = closestY;

    return navigateTo;
}



Point Planning::get_Destination () {
    return dest;
}
