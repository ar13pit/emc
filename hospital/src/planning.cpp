#include "planning.h"

using namespace std;
//using namespace WorldModel;


Destination Planning::picoPlan(High_State highSt, Low_State lowSt, bool wallDetected){
    std::cout << "High state =  " << highSt << std::endl;
    std::cout << "Low state =  " << highSt << std::endl;

    Room room;
    Point navigateTo;
    Destination dest;
    dest.angle = 0;
    dest.dist = 0;
    dest.x = 0;
    dest.y = 0;

    if(wallDetected){
        dest = getAwayFromWall(lowSt);
        std::cout << "WALL DETECT"<<  dest.angle << std::endl;
    }
    else{
        switch(highSt){
            case EXPLORE_HOSPITAL :                //Exlore_Hospital
                switch(lowSt){
                case EXPLORE_CORRIDOR :            //Explore_corridor
                    dest = setpointInCorridor();
                    return dest;
                case EXIT_TO_PREV_ROOM :
                    room = WorldModel.get_curRoom();
                    navigateTo = getNearbyExitPoint(room);
                    dest = driveToPoint(navigateTo);
                    return dest;
                case EXPLORE_ROOM :
                    room = WorldModel.get_curRoom();
                    dest = driveInRoom(room);
                    return dest;
                case GO_TO_NEXT_ROOM :
                    room = WorldModel.get_closestRoom();
                    navigateTo = getNearbyExitPoint(room);
                    dest = driveToPoint(navigateTo);
                    return dest;
                case GO_INSIDE_ROOM :
                    room = WorldModel.get_closestRoom();
                    navigateTo = getThroughtExitPoint(room);
                    dest = driveToPoint(navigateTo);
                    break;
                }
            case RETURN_TO_INIT :
                switch(lowSt){
                case EXIT_TO_PREV_ROOM :
                    room = WorldModel.get_curRoom();
                    navigateTo = getNearbyExitPoint(room);
                    dest = driveToPoint(navigateTo);
                    return dest;
                case GO_TO_START :
                    navigateTo = getStartPos();
                    dest = driveToPoint(navigateTo);
                    return dest;
                case PARKING :
                    dest = parkPico();
                    return dest;
                }
            case GO_TO_ROOM :
                switch(lowSt){
                case GO_TO_NEXT_ROOM :
                    room = WorldModel.get_nextRoom();
                    navigateTo = getNearbyExitPoint(room);
                    dest = driveToPoint(navigateTo);
                    return dest;
                case GO_INSIDE_ROOM :
                    room = WorldModel.get_closestRoom();
                    navigateTo = getThroughtExitPoint(room);
                    dest = driveToPoint(navigateTo);
                    break;
                case STAND_NEXT_TO_OBJECT :
                    // Drive to the object...
                    break;
                }
        }
    }

    return dest;
}

Destination Planning::setpointInCorridor(){

    //Move straight ahead, getAwayFromWall prevents from crashing into walls
    dest.dist = DIST_SETPOINT;

    return dest;
}

Destination Planning::getAwayFromWall(Low_State lowSt){

    Point closestPoint = WorldModel::get_closestPointWall();

    //Move sideways when PICO is inside the corridor OR
    //When closest point is at the side of PICO
    if(lowSt == EXPLORE_CORRIDOR || abs(closestPoint.angle) > FRONTWALL_ANGLE){
        dest.angle = closestPoint.angle < 0 ? MOVESIDEWAYS_FACTOR : -MOVESIDEWAYS_FACTOR;
        dest.dist = DIST_SETPOINT;
    }
    //If closest point is in the front of PICO, then turn around
    else{
        dest.angle = closestPoint.angle < 0 ? TURN_WHEN_OBJECT_IN_FRONT : -TURN_WHEN_OBJECT_IN_FRONT;
        dest.dist = 0;
    }
    return dest;
}

Point Planning::getNearbyExitPoint(Room closestRoom){

    Point destination;
    Point extPnt1 = closestRoom.exit_previous.exitPoint1;
    Point extPnt2 = closestRoom.exit_previous.exitPoint2;
    Point curPos = WorldModel::get_globalPosition();

    double xMid = 0.5*(extPnt1.x + extPnt2.x);
    double yMid = 0.5*(extPnt1.y + extPnt2.y);

    //Vertical exit
    if(abs(extPnt1.x - extPnt2.x) > abs(extPnt1.y - extPnt2.y)){
        destination.x = xMid;

        double distExtSide1 = sqrt(pow(curPos.x-xMid, 2) + pow(curPos.y-(yMid-WAIT_BEFORE_EXIT), 2));
        double distExtSide2 = sqrt(pow(curPos.x-xMid, 2) + pow(curPos.y-(yMid+WAIT_BEFORE_EXIT), 2));

        destination.y = distExtSide1 < distExtSide2 ? yMid-WAIT_BEFORE_EXIT : yMid+WAIT_BEFORE_EXIT;
    }
    //Horizontal exit
    else{
        destination.y = yMid;

        double distExtSide1 = sqrt(pow(curPos.x-(xMid-WAIT_BEFORE_EXIT), 2) + pow(curPos.y-yMid, 2));
        double distExtSide2 = sqrt(pow(curPos.x-(xMid+WAIT_BEFORE_EXIT), 2) + pow(curPos.y-yMid, 2));

        destination.x = distExtSide1 < distExtSide2 ? xMid-WAIT_BEFORE_EXIT : xMid+WAIT_BEFORE_EXIT;
    }

    cout << "Destination Point = (" << destination.x << ',' << destination.y << ')' << endl;
    return destination;
}

Destination Planning::driveToPoint(Point goToPoint){

//    ///////////////////  TODO  //////////////////////////////////////
//    /// Check wheter is works when the relative angle is different
//    /// Unable to check in simulation without other classes...
//    ///
    Point curPos = WorldModel::get_globalPosition();
    Destination dest;

    double disX = goToPoint.x-curPos.x;
    double disY = goToPoint.y-curPos.y;

    if(disX > 0 && disY < 0){
        cout << "First Quadrant" << endl;
        dest.angle = -atan(disY/disX);
    }
    else if(disX < 0 && disY < 0){
        cout << "Second Quadrant" << endl;
        dest.angle = M_PI-atan(disY/disX);
    }
    else if(disX < 0 && disY > 0){
        cout << "Third Quadrant" << endl;
        dest.angle = M_PI-atan(disY/disX);
    }
    else if(disX > 0 && disY > 0){
        cout << "Fourth Quadrant" << endl;
        dest.angle = -atan(disY/disX);
    }
    dest.dist = DIST_SETPOINT;


//    dest.angle = atan(disY/disX);

    cout << "ANGLE = " << dest.angle << endl;

    return dest;
}

Destination Planning::driveInRoom(Room curRoom){

    Point closestPoint = WorldModel::get_closestPointWall();
    Destination dest;

    //Turn around when front wall is too close
    if(abs(closestPoint.angle) < FRONTWALL_ANGLE && closestPoint.dist < DISTANCE_FROM_WALL_IN_ROOM){
        dest.angle = M_PI;
        dest.dist = DIST_SETPOINT;
    }
    else
        dest.dist = DIST_SETPOINT;

    return dest;
}

Point Planning::getStartPos(){

    Point startPos;
    startPos.x = 0;
    startPos.y = 0;

    return startPos;
}

Destination Planning::parkPico(){

//    ////////////////// TODO  /////////////////////////////////////////
//    ///         Needs to be tested
//   ///     What should the distance be? Drive backwards?


    Point corr_end = worldModel->get_pointStraightAhead();
    Destination dest;

    //Pico is backwards to the back wall
    dest.angle = M_PI - corr_end.angle;

    //Move the distance to the back wall - 5cm
    dest.dist = corr_end.dist-0.05;
}

Point Planning::getThroughtExitPoint(Room roomFromMapping){

    Point_det navigateTo;

    double closestX = INFINITY;
    double closestY = INFINITY;

    vector<Exit> allDetectedExits = getAllDetectedExits();

    Point extPntMap1 = roomFromMapping.exit_previous.exitPoint_det1;
    Point extPntMap2 = roomFromMapping.exit_previous.exitPoint_det2;
    Point curPos = getCurrentPosition();

    //Get the middle point of the exit from the mapping
    double xMidMap = 0.5*(extPntMap1.x + extPntMap2.x);
    double yMidMap = 0.5*(extPntMap1.y + extPntMap2.y);

    //In case multiple exits are found, select the correct one
    for(int i=0;i<allDetectedExits.size();i++){
        Point extPntDet1 = allDetectedExits[i].exitPoint_det1;
        Point extPntDet2 = allDetectedExits[i].exitPoint_det2;

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



Destination Planning::get_Destination(){
    return dest;
}
