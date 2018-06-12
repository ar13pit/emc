#include "planning.h"



//Destination Planning::picoPlan(High_State highSt, Low_State lowSt, bool wallDetected){
//    std::std::cout << "High state =  " << highSt << std::endl;
//    std::std::cout << "Low state =  " << highSt << std::endl;

//    Room room;
//    Point navigateTo;
//    Destination dest;
//    dest.angle = 0;
//    dest.dist = 0;
//    dest.x = 0;
//    dest.y = 0;

//    if(wallDetected){
//        dest = getAwayFromWall(lowSt);
//        std::std::cout << "WALL DETECT"<<  dest.angle << std::endl;
//    }
//    else{
//        switch(highSt){
//            case EXPLORE_HOSPITAL :                //Exlore_Hospital
//                switch(lowSt){
//                case EXPLORE_CORRIDOR :            //Explore_corridor
//                    dest = setpointInCorridor();
//                    return dest;
//                case EXIT_TO_PREV_ROOM :
//                    room = WorldModel.get_curRoom();
//                    navigateTo = getNearbyExitPoint(room);
//                    dest = driveToPoint(navigateTo);
//                    return dest;
//                case EXPLORE_ROOM :
//                    room = WorldModel.get_curRoom();
//                    dest = driveInRoom(room);
//                    return dest;
//                case GO_TO_NEXT_ROOM :
//                    room = WorldModel.get_closestRoom();
//                    navigateTo = getNearbyExitPoint(room);
//                    dest = driveToPoint(navigateTo);
//                    return dest;
//                case GO_INSIDE_ROOM :
//                    room = WorldModel.get_closestRoom();
//                    navigateTo = getThroughtExitPoint(room);
//                    dest = driveToPoint(navigateTo);
//                    break;
//                }
//            case RETURN_TO_INIT :
//                switch(lowSt){
//                case EXIT_TO_PREV_ROOM :
//                    room = WorldModel.get_curRoom();
//                    navigateTo = getNearbyExitPoint(room);
//                    dest = driveToPoint(navigateTo);
//                    return dest;
//                case GO_TO_START :
//                    navigateTo = getStartPos();
//                    dest = driveToPoint(navigateTo);
//                    return dest;
//                case PARKING :
//                    dest = parkPico();
//                    return dest;
//                }
//            case GO_TO_ROOM :
//                switch(lowSt){
//                case GO_TO_NEXT_ROOM :
//                    room = WorldModel.get_nextRoom();
//                    navigateTo = getNearbyExitPoint(room);
//                    dest = driveToPoint(navigateTo);
//                    return dest;
//                case GO_INSIDE_ROOM :
//                    room = WorldModel.get_closestRoom();
//                    navigateTo = getThroughtExitPoint(room);
//                    dest = driveToPoint(navigateTo);
//                    break;
//                case STAND_NEXT_TO_OBJECT :
//                    // Drive to the object...
//                    break;
//                }
//        }
//    }

//    return dest;
//}

Destination Planning::setpointInCorridor(){

    //Move straight ahead, getAwayFromWall prevents from crashing into walls
    dest.dist = DIST_SETPOINT;

    return dest;
}

Destination Planning::getAwayFromWall(WorldModel *worldModel){

    Point closestPoint = worldModel->get_closestPointWall();
    Destination dest;
    Low_State lowSt = worldModel->get_currentLowState();

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

Point Planning::getNearbyExitPoint(Room closestRoom, WorldModel *worldModel){

    Point destination;
    Point_map extPnt1 = closestRoom.exit.point1;
    Point_map extPnt2 = closestRoom.exit.point2;
    Point_map curPos = worldModel->get_globalPosition();

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

    std::cout << "Destination Point = (" << destination.x << ',' << destination.y << ')' <<std::endl;
    return destination;
}

Destination Planning::driveToPoint(Point goToPoint, WorldModel *worldModel){

//    ///////////////////  TODO  //////////////////////////////////////
//    /// Check wheter is works when the relative angle is different
//    /// Unable to check in simulation without other classes...
//    ///
    Point_map curPos = worldModel->get_globalPosition();
    Destination dest;

    double disX = goToPoint.x-curPos.x;
    double disY = goToPoint.y-curPos.y;

    if(disX > 0 && disY < 0){
        std::cout << "First Quadrant" <<std::endl;
        dest.angle = -atan(disY/disX);
    }
    else if(disX < 0 && disY < 0){
        std::cout << "Second Quadrant" <<std::endl;
        dest.angle = M_PI-atan(disY/disX);
    }
    else if(disX < 0 && disY > 0){
        std::cout << "Third Quadrant" <<std::endl;
        dest.angle = M_PI-atan(disY/disX);
    }
    else if(disX > 0 && disY > 0){
        std::cout << "Fourth Quadrant" <<std::endl;
        dest.angle = -atan(disY/disX);
    }
    dest.dist = DIST_SETPOINT;


//    dest.angle = atan(disY/disX);

    std::cout << "ANGLE = " << dest.angle <<std::endl;

    return dest;
}

Destination Planning::driveInRoom(WorldModel *worldModel){

    Point closestPoint = worldModel->get_closestPointWall();
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

Destination Planning::parkPico(WorldModel *worldModel){

//    ////////////////// TODO  /////////////////////////////////////////
//    ///         Needs to be tested
//   ///     What should the distance be? Drive backwards?


    Point corr_end = worldModel->get_pointStraightAhead();
    Destination dest;

    //Pico is backwards to the back wall
    dest.angle = M_PI - corr_end.angle;

    //Move the distance to the back wall - 5cm
    dest.dist = corr_end.dist-0.05;

    return dest;
}

Point Planning::getThroughtExitPoint(Room roomFromMapping, WorldModel *worldModel){

    Point navigateTo;

    double closestX = INFINITY;
    double closestY = INFINITY;

    std::vector<Exit_map> allDetectedExits = worldModel->getAllDetectedExits();

    Point_map extPntMap1 = roomFromMapping.exit.point1;
    Point_map extPntMap2 = roomFromMapping.exit.point2;

    //Get the middle point of the exit from the mapping
    double xMidMap = 0.5*(extPntMap1.x + extPntMap2.x);
    double yMidMap = 0.5*(extPntMap1.y + extPntMap2.y);

    //In case multiple exits are found, select the correct one
    for(int i=0;i<allDetectedExits.size();i++){
        Point_map extPntDet1 = allDetectedExits[i].point1;
        Point_map extPntDet2 = allDetectedExits[i].point2;

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
