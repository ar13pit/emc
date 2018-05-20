#include <cmath>
//#include <config.h>
//#include <opencv2/opencv.hpp>
#include "detection.h"

#ifndef planning_H
#define planning_H

// Class Point_cor
class Point_cor {
    double x_, y_;

public:
    Point_cor(double x, double y){
        x_ = x;
        y_ = y;
    };

    double get_x();
    double get_y();
    void offset(Point_cor offsetPoint_cor);
    void offset(double offsetConstant);
};


// Class Line
// Constructs a line object from 2 input Point_cors
//
// Properties:
//     midPoint_cor_   : Stores the midPoint_cor of the line joining the 2 input Point_cors as Point_cor object
//     slope_      : Stores the computed slope of the line as a double
//     equation    : Stores coefficients of equation of line (ax + by + c = 0) as a double array
//
class Line {
    Point_cor Point_cor1_, Point_cor2_, midPoint_cor_;
    double slope_, equation[3];

    Point_cor calculate_midPoint_cor(Point_cor Point_cor1, Point_cor Point_cor2);
    double calculate_slope(Point_cor Point_cor1, Point_cor Point_cor2);
    void calculate_equation(Point_cor Point_cor1, double slope, double &equation_array);

public:
    Line(Point_cor Point_cor1, Point_cor Point_cor2){
        Point_cor1_ = Point_cor1;
        Point_cor2_ = Point_cor2;
        midPoint_cor_ = calculate_midPoint_cor(Point_cor1_, Point_cor2_);
        slope_ = calculate_slope(Point_cor1_, Point_cor2_);
        calculate_equation(Point_cor1_, slope_, equation);
    };

    Point_cor get_line_Point_cor1();
    Point_cor get_line_Point_cor2();
    Point_cor get_line_midPoint_cor();
    double get_line_slope();
    double get_line_equation();
};


class RelativePoint_cor{
    Point_cor coordinates_;
    double angle_, radius_;

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
