#include "driveControl.h"
#include "detection.h"
#include "worldModel.h"
#include "planning.h"

void initialize(Detection_data *data, Destination *dest, Flags *flags){

    // Flags

    flags->in_corridor = false;
    flags->in_process = false;
    flags->turned_once = false;



    // Destination

    dest->angle = 0;
    dest->x = 0;
    dest->y = 0;



    // Detection data

    //corridor initialization
    data->corridor.escaped = false;

    data->corridor.leftWall1.angle = 0;
    data->corridor.leftWall1.dist = 0;
    data->corridor.leftWall1.x = 0;
    data->corridor.leftWall1.y = 0;

    data->corridor.leftWall2.angle = 0;
    data->corridor.leftWall2.dist = 0;
    data->corridor.leftWall2.x = 0;
    data->corridor.leftWall2.y = 0;

    data->corridor.rightWall1.angle = 0;
    data->corridor.rightWall1.dist = 0;
    data->corridor.rightWall1.x = 0;
    data->corridor.rightWall1.y = 0;

    data->corridor.rightWall2.angle = 0;
    data->corridor.rightWall2.dist = 0;
    data->corridor.rightWall2.x = 0;
    data->corridor.rightWall2.y = 0;


    // exit initialization
    data->exit.detected = false;

    data->exit.exitPoint_det1.angle = 0;
    data->exit.exitPoint_det1.dist = 0;
    data->exit.exitPoint_det1.x = 0;
    data->exit.exitPoint_det1.y = 0;

    data->exit.exitPoint_det2.angle = 0;
    data->exit.exitPoint_det2.dist = 0;
    data->exit.exitPoint_det2.x = 0;
    data->exit.exitPoint_det2.y = 0;


    // furthest point initialization
    data->furthest_point.angle = 0;
    data->furthest_point.dist = 0;
    data->furthest_point.x = 0;
    data->furthest_point.y = 0;


}
