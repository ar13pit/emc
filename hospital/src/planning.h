#ifndef planning_H
#define planning_H

#include "main.hpp"
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
    Destination setpointInCorridor();               //Navigate Pico through Corridor
    Destination getAwayFromWall(Low_State lowSt);
    Room get_closestRoom();
    Point_det getNearbyExitPoint(Room closestRoom);
    Destination driveToPoint(Point_det navigateTo);
    Room getRoom();
    Destination driveInRoom(Room curRoom);
    Point_det getStartPos();
    Room getMostNestedRoom();
    Room getNextRoom(Room mostNestedRoom);
    Destination parkPico();

public:
    Destination get_Destination();

    Planning(){
        return;
    }

};
#endif //planning_H
