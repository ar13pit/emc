#include "planning.h"
#include "main.hpp"
#include "config.h"
#include "worldModel.h"
#include "detection.h"
#include "iostream"
#include "math.h"
#include <cmath>
#include <vector>
using namespace std;
using namespace WorldModel;


Destination Planning::picoPlan(High_State highSt, Low_State lowSt, bool wallDetected){
    std::cout << "High state =  " << highSt << std::endl;
    std::cout << "Low state =  " << highSt << std::endl;

    Room room;
    Point_det navigateTo;
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
                    room = getRoom();
                    navigateTo = getNearbyExitPoint(room);
                    dest = driveToPoint(navigateTo);
                    return dest;
                case EXPLORE_ROOM :
                    room = getRoom();
                    dest = driveInRoom(room);
                    return dest;
                case GO_TO_NEXT_ROOM :
                    room = get_closestRoom();
                    navigateTo = getNearbyExitPoint(room);
                    dest = driveToPoint(navigateTo);
                    return dest;
                case GO_INSIDE_ROOM :
                    // Go through the exit...
                    break;
                }
            case RETURN_TO_INIT :
                switch(lowSt){
                case EXIT_TO_PREV_ROOM :
                    room = getRoom();
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
                case GO_TO_NEXT_EXIT :
                    room = getMostNestedRoom();
                    room = getNextRoom(room);
                    navigateTo = getNearbyExitPoint(room);
                    dest = driveToPoint(navigateTo);
                    return dest;
                case GO_INSIDE_ROOM :
                    // Go through the exit...
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

    Point_det closestPoint = WorldModel::get_closestPointWall();

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

Room Planning::get_closestRoom(){

    Room closestRoom;
    vector<Room> allRooms = WorldModel::getAllRooms();
    int curRoom = WorldModel::getCurrentRoom();
    Point_det curPos = WorldModel::getCurrentPosition();
    double shortestDist = INFINITY;

    for(int i = 0;i<allRooms.size(); i++){
//        ///////////////////////////  TODO  ///////////////////////////////
//        ///    This works, but better to set closestRoom.corners[0].x
//        ///    to 0 when initialized

        if(allRooms[i].exit_previous.detected && curRoom == allRooms[i].previousRoom &&
               (abs(closestRoom.corners[0].x) < 0.001 || abs(closestRoom.corners[0].x > 100))){
            //Get middle point of the exit
            cout << '1' << endl;
            double xMid = 0.5*(allRooms[i].exit_previous.exitPoint_det1.x + allRooms[i].exit_previous.exitPoint_det2.x);
            double yMid = 0.5*(allRooms[i].exit_previous.exitPoint_det1.y + allRooms[i].exit_previous.exitPoint_det2.y);

            double distToRoom = sqrt(pow(curPos.x-xMid, 2) + pow(curPos.y-yMid, 2));
            if(distToRoom < shortestDist){
                shortestDist = distToRoom;
                closestRoom = allRooms[i];
            }
            //cout << "Exit Room " << i << " is at (" << xMid << ',' << yMid << ')' << endl;
        }
    }
    //cout << "Closest Exit is at (" << 0.5*(closestRoom.exit_previous.exitPoint_det1.x + closestRoom.exit_previous.exitPoint_det2.x) << ',' << 0.5*(closestRoom.exit_previous.exitPoint_det1.y + closestRoom.exit_previous.exitPoint_det2.y) << ')' << endl;
    return closestRoom;
}

Point_det Planning::getNearbyExitPoint(Room closestRoom){

    Point_det destination;
    Point_det extPnt1 = closestRoom.exit_previous.exitPoint_det1;
    Point_det extPnt2 = closestRoom.exit_previous.exitPoint_det2;
    Point_det curPos = WorldModel::getCurrentPosition();

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

Destination Planning::driveToPoint(Point_det goToPoint){

//    ///////////////////  TODO  //////////////////////////////////////
//    /// Check wheter is works when the relative angle is different
//    /// Unable to check in simulation without other classes...
//    ///
    Point_det curPos = WorldModel::getCurrentPosition();
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

Room Planning::getRoom(){

    Room prevRoom;
    int curRoom = WorldModel::getCurrentRoom();
    vector<Room> allRooms = WorldModel::getAllRooms();

    //ASSUMPTION: Rooms are stored in order where the currentRoom == (i+1)'th element in AllRooms
    if(allRooms.size() >= curRoom-1)
        prevRoom = allRooms[curRoom-1];
    else
        cout << "NO VALID ROOM NUMBER!" << endl;

    return prevRoom;
}

Destination Planning::driveInRoom(Room curRoom){

    Point_det closestPoint = WorldModel::get_closestPointWall();
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

Point_det Planning::getStartPos(){

    Point_det startPos;
    startPos.x = 0;
    startPos.y = 0;

    return startPos;
}

Room Planning::getMostNestedRoom(){

    vector<Room> allRooms = WorldModel::getAllRooms();
    Room mostNestedRoom;
    int highestNesting = 0;

    for(int i=0; i<allRooms.size(); i++){
        int countNesting = 0;
        Room lowerRoom = allRooms[i];

        //While previousRoom is not the corridor
        while(lowerRoom.previousRoom != 0){
            //ASSUMPTION: Rooms are stored in order where the currentRoom == (i+1)'th element in AllRooms
            lowerRoom = allRooms[lowerRoom.previousRoom-1];
            countNesting++;
        }
        if(countNesting > highestNesting){
            mostNestedRoom = allRooms[i];
        }
    }
    cout << "MostNestedRoom = Room nested in Nr " << mostNestedRoom.previousRoom << endl;
    return mostNestedRoom;
}

Room Planning::getNextRoom(Room mostNestedRoom){

    int curRoom = WorldModel::getCurrentRoom();
    vector<Room> allRooms = WorldModel::getAllRooms();
    Room nextRoom;

    while(nextRoom.previousRoom != curRoom){
        nextRoom = allRooms[nextRoom.previousRoom-1];
    }
    return nextRoom;
}

Destination Planning::parkPico(){

//    ////////////////// TODO  /////////////////////////////////////////
//    ///         Needs to be tested
//   ///     What should the distance be? Drive backwards?


    Point_det corr_end = worldModel->get_pointStraightAhead();
    Destination dest;

    //Pico is backwards to the back wall
    dest.angle = M_PI - corr_end.angle;

    //Move the distance to the back wall - 5cm
    dest.dist = corr_end.dist-0.05;
}


Destination Planning::get_Destination(){
    return dest;
}

