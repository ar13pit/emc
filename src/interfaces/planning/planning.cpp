#include "planning.h"
#include "helper.hpp"
#include "main.hpp"
#include "opencv2/opencv.hpp"
// #include <math.h>

//-----------------------------------------------------------------------------------
// might be useful for corridor

// Methods of Class Point
double Point::get_x(){
    return x_;
};

double Point::get_y(){
    return y_;
};

void Point::offset(Point offsetPoint){
    x_ += offsetPoint.get_x();
    y_ += offsetPoint.get_y();
};

void Point::offset(double offsetConstant){
    x_ += offsetConstant;
    y_ += offsetConstant;
};

// Methods of Class Line
Point Line::calculate_midpoint(Point point1, Point point2){
    double x_mid, y_mid;

    x_mid = (point1.get_x() + point2.get_x())/2;
    y_mid = (point1.get_y() + point2.get_y())/2;

    return Point(x_mid, y_mid);
};

double Line::calculate_slope(Point point1, Point point2){
    double slope;

    slope = (point1.get_y() - point2.get_y())/(point1.get_x() - point2.get_x());

    return slope;
};

void Line::calculate_equation(Point point1, double slope, double &equation_array){
    // double x, y;
    //
    // x = point1.get_x();
    // y = point1.get_y();

    equation_array[0] = slope;
    equation_array[1] = -1;
    equation_array[2] = -slope*point1.get_x() + point1.get_y();
};

Point Line::get_line_point1(){
    return point1_;
};

Point Line::get_line_point2(){
    return point2_;
};

Point Line::get_line_midpoint(){
    return midpoint_;
};

double Line::get_line_slope(){
    return slope_;
};

double get_line_equation(){
    return equation;
};


// // set furthest point
// Furthest_point set_furthest_point(Point point){
//     Furthest_point far;
//     far.dist = sqrt(point.x*point.x + point.y*point.y);
//     far.x = point.x;
//     far.y = point.y;
//     far.angle = atan(point.y/point.x);
//     return far;
// }
//
// //----------------------------------------------------------------------------------
//
//
// // calculate the destination when exit identified
// Destination calc_exit_dest (Exit exit){
//     Destination dest;
//     dest.x = (exit.x1 + exit.x2)/2;
//     dest.y = (exit.y1 + exit.y2)/2;
//     dest.angle = (exit.angle1 + exit.angle2)/2;
//     return dest;
// }
//
//
// // calculate the destination when exit in not identified
// Destination calc_furthest_dest (Furthest_point furthest){
//     Destination dest;
//     dest.x = furthest.x/4;
//     dest.y = furthest.y/4;
//     dest.angle = furthest.angle;
//     return dest;
// }
//
//
// // check whether the new point is further than the saved one
// Furthest_point compare_furthest_point(Point point, Furthest_point far){
//     double new_dist;
//     new_dist = sqrt(point.x*point.x + point.y*point.y);
//     if (new_dist>far.dist) {
//         far = set_furthest_point(point);
//     }
//     return far;
// }
//
//
// Destination main_logic(Exit exit, Point furthest, Status status){
//
//
//     bool set_turn_flag;
//     bool turned_once_flag;
//     bool move_to_exit;
//     Furthest_point far;
//     Destination dest;
//
//     // initialize
//     far.dist = 0;
//     far.angle = 0;
//     far.x = 0;
//     far.y = 0;
//     turned_once_flag = false;   // assume always start without turning
//
//     switch(status){
//
//
//     case(ROOM_ESCAPE):
//
//         // check if the exit detected
//         if ((exit.detected) || (move_to_exit)) {
//
//             move_to_exit = true;
//             dest = calc_exit_dest(exit);        // define destination
//
//         } else {
//
//             // check if we have turned around already
//             if (!turned_once_flag){
//                 set_turn_flag = true;           // command control to turn 180
//                 turned_once_flag = true;        // remember that we've turned around
//                 far = set_furthest_point(furthest);     // save the furthest point given info from perception
//             } else {
//
//                 // check if exit detected after we turned
//                 if ((exit.detected) || (move_to_exit)) {
//                     move_to_exit = true;
//                     dest = calc_exit_dest(exit);        // define destination
//                 } else {
//                     far = compare_furthest_point(furthest,far);     // check if the first point was further
//                     dest = calc_furthest_dest(far);             // set the furthest point as a destination
//                 }
//             }
//
//
//         }
//
//
//         break;
//
//     case(IN_CORRIDOR):
//         break;
//     }
//     return dest;
// }
