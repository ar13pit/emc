#include <cmath>
#include <iostream>
#include <config.h>
// #include <math.h>
#include "detection.h"
#include "main2.hpp"


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

    void calculate_xangle();
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
        calculate_xangle();
        calculate_radius();
    };

    double get_x();
    double get_y();
    double get_xangle();
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
    LineCorridor() {}

    LineCorridor(PointCorridor point1, PointCorridor point2) {
        point1_ = point1;
        point2_ = point2;
        calculate_midpoint();
        calculate_slope();
        calculate_equation();
        calculate_perpendicular();
    }

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



// Destination that is passed to the Control block
typedef struct {
    double x;
    double y;
    double dist;
    double angle;
} Destination;


class Planning{
private:
    Destination dest;
    Point_det absolute_furthest;

    void room_logic(Detection_data *data, Flags *flags, Destination *far_point);
    void corrid2dest_transf(Corridor corr, Detection_data *data);
    void planning(Detection_data *data, Flags* flags);



    void set_furthest_point(Point_det *point);
    void calc_exit_dest (Detection_data *data);
    void calc_furthest_dest (Point_det furthest);
    void compare_furthest_point(Point_det *point);
    void turn_around();
    void exit_center_realign(Detection_data *data);

    double distance_calc(double x, double y);

    bool check_corridor(Detection_data *data);

public:

    Planning(Detection_data *data, Flags *flags, Destination *far_point){


        if (flags->in_corridor){
            std::cout << "Corridor detected" << "\n";

            PointCorridor left1(data->corridor.leftWall1.x, data->corridor.leftWall1.y);
            PointCorridor left2(data->corridor.leftWall2.x, data->corridor.leftWall2.y);
            PointCorridor right1(data->corridor.rightWall1.x, data->corridor.rightWall1.y);
            PointCorridor right2(data->corridor.rightWall2.x, data->corridor.rightWall2.y);

//            left1.print(); std::cout << '\n';
//            left2.print(); std::cout << '\n';
//            right1.print(); std::cout << '\n';
//            right2.print(); std::cout << '\n';

            LineCorridor leftLine(left1, left2);
            LineCorridor rightLine(right1, right2);

            std::cout << "\n Calculating Corridor setpoint" << '\n';
            Corridor corridor(leftLine, rightLine);

//            rightLine.print();
//            leftLine.print();

//            std::cout << "" << corridor.get_corridor_setpoint()
            // assign destination point

            corrid2dest_transf(corridor,data);
            flags->drive_frw = true;

        }



        std::cout << "Exit is " << data->exit.exitPoint_det1.dist << " " << data->exit.exitPoint_det2.dist << " away"<< "\n";

        if (!flags->in_corridor &&((data->exit.exitPoint_det1.dist < EXIT_THRESHOLD) || (data->exit.exitPoint_det2.dist < EXIT_THRESHOLD))){

            if ((data->exit.exitPoint_det1.dist < EXIT_THRESHOLD) && (data->exit.exitPoint_det2.dist < EXIT_THRESHOLD)){

                flags->in_corridor = true;
                std::cout << "corridor detected " << "\n";

            } else {

                exit_center_realign(data);
                std::cout << "realigning with the corridor" << "\n";
                flags->drive_frw = true;
                flags->in_corridor = true;
            }

        } else {
            room_logic(data, flags, far_point);
        }


        if (fabs(dest.angle) >= M_PI+TURN_MARGIN){
            dest.angle = (fabs(dest.angle) - M_PI)*(-1);
        }
    }



    Destination get_Destination();



   // bool wallDetected(double minDistance);// Method to check if any wall is in the neighbourhood of the robot

};



#endif
