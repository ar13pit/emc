#include <cmath>
#include <iostream>
#include "config.h"
#include "detection.h"
#include "helper.hpp"
#include "main.hpp"
#include "opencv2/opencv.hpp"


#ifndef planning_H
#define planning_H

<<<<<<< Updated upstream
/*
--------------------------------------------------------------------------------
                                Class Point
--------------------------------------------------------------------------------
*/
class Point {
    // Class Point
    // Constructs a 2D point object in relative coordinates
    // Properties:
    //     x_          : x-coordinate relative to origin
    //     y_          : y-cooridnate relative to origin
    //     angle_      : angle of vector made (x, y) from y-axis
    //     radius_     : distance of the point from the relative origin
    //

    double x_, y_, angle_, radius_;

    void calculate_angle();
    void calculate_radius();

public:
    Point(){
        x_ = 0.0;
        y_ = 0.0;
        angle_ = 0.0;
        radius_ = 0.0;
    };

=======

void planning(Detection_data *data, Destination *dest, bool in_corridor);
void room_logic(Exit exit, Point_det current_furthest,  Destination *dest);






// Class Point
class Point {
    double x_, y_;

public:
>>>>>>> Stashed changes
    Point(double x, double y){
        x_ = x;
        y_ = y;
        calculate_angle();
        calculate_radius();
    };

    double get_x();
    double get_y();
<<<<<<< Updated upstream
    double get_angle();
    double get_radius();
    void print();
    // void offset(Point offsetPoint);
    // void offset(double offsetConstant);
};


/*
--------------------------------------------------------------------------------
                                Class Line
--------------------------------------------------------------------------------
*/

class Line {
    // Class Line
    // Constructs a line object from 2 input points
    //
    // Properties:
    //     midpoint_   : Stores the midpoint of the line joining the 2 input
    //                   points as Point object
    //     slope_      : Stores the computed slope of the line as a double
    //     equation    : Stores coefficients of equation of
    //                   line (ax + by + c = 0) as a double array
    //

    Point point1_, point2_, midpoint_;
    double slope_, equation[3], perpendicular_point1[3], perpendicular_point2[3], perpendicular_midpoint[3];

    void calculate_midpoint();
    void calculate_slope();
    void calculate_equation();
    void calculate_perpendicular();
=======
    void offset(Point offsetPoint);
    void offset(double offsetConstant);
};


// Class Line
// Constructs a line object from 2 input points
//
// Properties:
//     midpoint_   : Stores the midpoint of the line joining the 2 input points as Point object
//     slope_      : Stores the computed slope of the line as a double
//     equation    : Stores coefficients of equation of line (ax + by + c = 0) as a double array
//
class Line {
    Point point1_, point2_, midpoint_;
    double slope_, equation[3];

    Point calculate_midpoint(Point point1, Point point2);
    double calculate_slope(Point point1, Point point2);
    void calculate_equation(Point point1, double slope, double &equation_array);
>>>>>>> Stashed changes

public:
    Line(Point point1, Point point2){
        point1_ = point1;
        point2_ = point2;
<<<<<<< Updated upstream
        calculate_midpoint();
        calculate_slope();
        calculate_equation();
        calculate_perpendicular();
=======
        midpoint_ = calculate_midpoint(point1_, point2_);
        slope_ = calculate_slope(point1_, point2_);
        calculate_equation(point1_, slope_, equation);
>>>>>>> Stashed changes
    };

    Point get_line_point1();
    Point get_line_point2();
    Point get_line_midpoint();
    double get_line_slope();
    void get_line_equation(double input_array[3]) const;
    void get_line_perpendicular1(double input_array[3]) const;
    void get_line_perpendicular2(double input_array[3]) const;
    void get_line_midpoint(double input_array[3]) const;
    void print();
};


<<<<<<< Updated upstream
/*
--------------------------------------------------------------------------------
                                Class Corridor
--------------------------------------------------------------------------------
*/

class Corridor {
    // Class Corridor
    // Generates a setpoint based on predefined distance for the robot in the corridor
    //
    // Properties:
    //     left_       : Line representing the left wall of the corridor
    //     right_      : Line representing the right wall of the corridor
    //     center_     : Line representing the center of the corridor
    //     setpoint_   : The generated setpoint for the robot
    //

    Line left_, right_, center_;
    Point setpoint_;

    void calculate_center_line();
    void calculate_setpoint();

public:
    Corridor (Line leftLine, Line rightLine) {
        left_ = leftLine;
        right_ = rightLine;
        calculate_center_line();
        calculate_setpoint();
    };
=======
class RelativePoint{
    Point coordinates_;
    double angle_, radius_;
>>>>>>> Stashed changes

    Line get_corridor_line_left();
    Line get_corridor_line_right();
    Line get_corridor_line_center();
    Point get_corridor_setpoint();
};



// Destination that is passed to the Control block
typedef struct {
    int x;
    int y;
    int angle;
} Destination;




#endif
