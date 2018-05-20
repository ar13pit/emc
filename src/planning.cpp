#include "planning.h"
#include "helper.hpp"
#include "main.hpp"
#include "opencv2/opencv.hpp"
#include <cmath>
#include <math.h>




/*---------------------------------------------------------------------------------
---------------------------inside the room-----------------------------------------
---------------------------------------------------------------------------------*/

// set furthest point
Furthest_point set_furthest_point(Point point){
    Furthest_point far;
    far.dist = sqrt(point.x*point.x + point.y*point.y);
    far.x = point.x;
    far.y = point.y;
    far.angle = atan(point.y/point.x);
    return far;
}

//----------------------------------------------------------------------------------


// calculate the destination when exit identified
Destination *calc_exit_dest (Exit exit, Destination *dest){
    dest->x = (exit.x1 + exit.x2)/2;
    dest->y = (exit.y1 + exit.y2)/2;
    dest->angle = (exit.angle1 + exit.angle2)/2;
    return dest;
}


// calculate the destination when exit in not identified
Destination *calc_furthest_dest (Furthest_point furthest, Destination *dest){
    dest->x = furthest.x/4;
    dest->y = furthest.y/4;
    dest->angle = furthest.angle;
    return dest;
}


// check whether the new point is further than the saved one
Furthest_point compare_furthest_point(Point point, Furthest_point far){
    double new_dist;
    new_dist = sqrt(point.x*point.x + point.y*point.y);
    if (new_dist>far.dist) {
        far = set_furthest_point(point);
    }
    return far;
}



Destination main_logic(Exit exit, Point furthest, Status status, Destination *dest){


    bool set_turn_flag;
    bool turned_once_flag;
    bool move_to_exit;
    Furthest_point far;
    //Destination dest;

    // initialize
    far.dist = 0;
    far.angle = 0;
    far.x = 0;
    far.y = 0;
    turned_once_flag = false;   // assume always start without turning

    switch(status){


    case(ROOM_ESCAPE):

        // check if the exit detected
        if ((exit.detected) || (move_to_exit)) {

            move_to_exit = true;
            dest = calc_exit_dest(exit,dest);        // define destination

        } else {

            // check if we have turned around already
            if (!turned_once_flag){
                set_turn_flag = true;           // command control to turn 180
                turned_once_flag = true;        // remember that we've turned around
                far = set_furthest_point(furthest);     // save the furthest point given info from perception
            } else {

                // check if exit detected after we turned
                if ((exit.detected) || (move_to_exit)) {
                    move_to_exit = true;
                    dest = calc_exit_dest(exit,dest);        // define destination
                } else {
                    far = compare_furthest_point(furthest,far);     // check if the first point was further
                    dest = calc_furthest_dest(far,dest);             // set the furthest point as a destination
                }
            }


        }


        break;

    case(IN_CORRIDOR):
        break;
    }
    return dest;
}
