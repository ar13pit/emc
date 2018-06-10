#ifndef planning_H
#define planning_H

#include "stateMachine.h"
#include "detection.h"
#include "worldModel.h"
#include <vector>


// Destination that is passed to the Control block
typedef struct {
    double x;
    double y;
    double dist;
    double angle;
} Destination;

bool state_machine(struct High_state high_st,struct Low_state low_st);

class Planning{
private:

    Destination dest;

public:


    Destination driveToPoint(Point navigateTo);
    Destination setpointInCorridor();               //Navigate Pico through Corridor
    Destination getAwayFromWall(Low_State lowSt);
    Destination driveInRoom(Room curRoom);
    Destination parkPico();

//    Destination get_Destination();

    Room get_closestRoom();
    Room getMostNestedRoom();
    Room getNextRoom(Room mostNestedRoom);
    Room getRoom();

    Point getNearbyExitPoint(Room closestRoom);
    Point getStartPos();


    Planning(){
        return;
    }

};
#endif //planning_H
