#include <cmath>
#include <iostream>
#include <config.h>
#include "detection.h"
#include "helper.hpp"
#include "main.hpp"
#include "opencv2/opencv.hpp"


#ifndef planning_H
#define planning_H

/*
--------------------------------------------------------------------------------
                                Class PointCorridor
--------------------------------------------------------------------------------
*/
class PointCorridor {
    // Class PointCorridor
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
    PointCorridor(){
        x_ = 0.0;
        y_ = 0.0;
        angle_ = 0.0;
        radius_ = 0.0;
    };

    PointCorridor(double x, double y){
        x_ = x;
        y_ = y;
        calculate_angle();
        calculate_radius();
    };

    double get_x();
    double get_y();
    double get_angle();
    double get_radius();
    void print();
    // void offset(Point offsetPoint);
    // void offset(double offsetConstant);
};


/*
--------------------------------------------------------------------------------
                                Class LineCorridor
--------------------------------------------------------------------------------
*/

class LineCorridor {
    // Class LineCorridor
    // Constructs a line object from 2 input points
    //
    // Properties:
    //     midpoint_   : Stores the midpoint of the line joining the 2 input
    //                   points as PointCorridor object
    //     slope_      : Stores the computed slope of the line as a double
    //     equation    : Stores coefficients of equation of
    //                   line (ax + by + c = 0) as a double array
    //

    PointCorridor point1_, point2_, midpoint_;
    double slope_, equation[3], perpendicular_point1[3], perpendicular_point2[3], perpendicular_midpoint[3];

    void calculate_midpoint();
    void calculate_slope();
    void calculate_equation();
    void calculate_perpendicular();

public:
    LineCorridor() {};

    LineCorridor(PointCorridor point1, PointCorridor point2) {
        point1_ = point1;
        point2_ = point2;
        calculate_midpoint();
        calculate_slope();
        calculate_equation();
        calculate_perpendicular();
    };

    PointCorridor get_line_point1();
    PointCorridor get_line_point2();
    PointCorridor get_line_midpoint();
    double get_line_slope();
    void get_line_equation(double input_array[3]) const;
    void get_line_perpendicular_point1(double input_array[3]) const;
    void get_line_perpendicular_point2(double input_array[3]) const;
    void get_line_perpendicular_midpoint(double input_array[3]) const;
    void print();
};


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

    LineCorridor left_, right_, center_;
    PointCorridor setpoint_;

    void calculate_center_line();
    void calculate_setpoint();

public:
    Corridor() {};

    Corridor(LineCorridor left_line, LineCorridor right_line) {
        left_ = left_line;
        right_ = right_line;
        calculate_center_line();
        calculate_setpoint();
    };

    LineCorridor get_corridor_line_left();
    LineCorridor get_corridor_line_right();
    LineCorridor get_corridor_line_center();
    PointCorridor get_corridor_setpoint();
};


// Furthest Point
typedef struct {
    int x;
    int y;
    double angle;
    double dist;
} Furthest_Point;

/*
 * Use definition from Jari
 *
// Store exit data: two corners, angles and a flag whether detected
typedef struct {
    bool detected;
    int x1;
    int y1;
    int x2;
    int y2;
    double angle1;
    double angle2;
} Exit_pl;*/


// Destination that is passed to the Control block
typedef struct {
    int x;
    int y;
    int angle;
} Destination;


#endif
