#include <cmath>
//#include <config.h>
#include <opencv2/opencv.hpp>

#ifndef planning_H
#define planning_H

// Class Point
class Point {
    double x_, y_;

public:
    Point(double x, double y){
        x_ = x;
        y_ = y;
    };

    double get_x();
    double get_y();
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
/*class Line {
    Point point1_, point2_, midpoint_;
    double slope_, equation[3];

    Point calculate_midpoint(Point point1, Point point2);
    double calculate_slope(Point point1, Point point2);
    void calculate_equation(Point point1, double slope, double &equation_array);

public:
    Line(Point point1, Point point2){
        point1_ = point1;
        point2_ = point2;
        midpoint_ = calculate_midpoint(point1_, point2_);
        slope_ = calculate_slope(point1_, point2_);
        calculate_equation(point1_, slope_, equation);
    };

    Point get_line_point1();
    Point get_line_point2();
    Point get_line_midpoint();
    double get_line_slope();
    double get_line_equation();
};


class RelativePoint{
    Point coordinates_;
    double angle_, radius_;

};
*/

// Furthest point
typedef struct {
    int x;
    int y;
    double angle;
    double dist;
} Furthest_point;


// Store exit data: two corners, angles and a flag whether detected
typedef struct {
    bool detected;
    double x1;
    double y1;
    double x2;
    double y2;
    double angle1;
    double angle2;
} Exit;


// Destination that is passed to the Control block
typedef struct {
    int x;
    int y;
    double angle;
} Destination;


//Should not be here
typedef enum {
    ROOM_ESCAPE,
    IN_CORRIDOR
} Status;

void main_logic(Exit exit, Point furthest, Status status, Destination *dest);


#endif
