#include "planning.h"
#include "helper.hpp"
#include "main.hpp"
#include "detection.h"
#include "opencv2/opencv.hpp"
#include <cmath>
#include <math.h>


// Methods of Class Point
double Point_cor::get_x(){
    return x_;
};

double Point::get_y(){
    return y_;
};

void Point_cor::offset(Point_cor offsetPoint){
    x_ += offsetPoint.get_x();
    y_ += offsetPoint.get_y();
};

void Point_cor::offset(double offsetConstant){
    x_ += offsetConstant;
    y_ += offsetConstant;
};

// Methods of Class Line
Point_cor Line::calculate_midpoint(Point_cor point1, Point_cor point2){
    double x_mid, y_mid;

    x_mid = (point1.get_x() + point2.get_x())/2;
    y_mid = (point1.get_y() + point2.get_y())/2;

    return Point_cor(x_mid, y_mid);
};

double Line::calculate_slope(Point_cor point1, Point_cor point2){
    double slope;

    slope = (point1.get_y() - point2.get_y())/(point1.get_x() - point2.get_x());

    return slope;
};

void Line::calculate_equation(Point_cor point1, double slope, double &equation_array){
    // double x, y;
    //
    // x = point1.get_x();
    // y = point1.get_y();

    equation_array[0] = slope;
    equation_array[1] = -1;
    equation_array[2] = -slope*point1.get_x() + point1.get_y();
};

Point_cor Line::get_line_point1(){
    return point1_;
};

Point_cor Line::get_line_point2(){
    return point2_;
};

Point_cor Line::get_line_midpoint(){
    return midpoint_;
};

double Line::get_line_slope(){
    return slope_;
};

double get_line_equation(){
    return equation;
};



/*---------------------------------------------------------------------------------
---------------------------inside the room-----------------------------------------
---------------------------------------------------------------------------------*/

// conversion from Exit to Exit_pl
Exit_pl exits_conversion(Exit ex_det){
    Exit_pl ex_conv;
    ex_conv.x1
}

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
    dest->x = (exit.exitPoint_det1.x + exit.exitPoint_det2.x)/2;
    dest->y = (exit.exitPoint_det1.y + exit.exitPoint_det2.y)/2;
    dest->angle = (exit.exitPoint_det1.angle + exit.exitPoint_det2.angle)/2;
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



