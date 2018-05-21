#include "planning.h"

// main function for decisions inside the block
void planning(Detection_data *data, Destination *dest, bool in_corridor){

    if (!in_corridor) {
        room_logic(data->exit,data->furthest_point,dest);
    }
    else if (in_corridor){
        //do something with arpit
    } else {
        std::cout << "Fatal error: not is room and not in a corridor" << endl;
    }

}



/*
-------------------------------------------------------------------------------
                        Class PointCorridor Methods
-------------------------------------------------------------------------------
*/

void PointCorridor::calculate_angle() {
    angle_ = ((PI/2) - atan(y_/x_));
};

void PointCorridor::calculate_radius() {
    radius_ = sqrt(pow(x_, 2) + pow(y_, 2));
};

double PointCorridor::get_x() {
    return x_;
};

double PointCorridor::get_y() {
    return y_;
};

double PointCorridor::get_angle() {
    return angle_;
};

double PointCorridor::get_radius() {
    return radius_;
};

void PointCorridor::print() {
    std::cout << "Coordinates: (" << x_ << ", " << y_ << "), Radius: " << radius_ << " Angle: " << angle_ ;
}


/*
-------------------------------------------------------------------------------
                        Class LineCorridor Methods
-------------------------------------------------------------------------------
*/

void LineCorridor::calculate_midpoint() {
    double x_mid, y_mid;

    x_mid = (point1_.get_x() + point2_.get_x())/2;
    y_mid = (point1_.get_y() + point2_.get_y())/2;
    midpoint_ = PointCorridor(x_mid, y_mid);
};

void LineCorridor::calculate_slope() {
    slope_ = (point1_.get_y() - point2_.get_y())/(point1_.get_x() - point2_.get_x());
};

void LineCorridor::calculate_equation() {
    equation[0] = slope_;
    equation[1] = -1.0;
    equation[2] = -slope_*point1_.get_x() + point1_.get_y();
};

void LineCorridor::calculate_perpendicular(){
    perpendicular_point1[0] = 1.0;
    perpendicular_point1[1] = slope_;
    perpendicular_point1[2] = -point1_.get_x() -slope_*point1_.get_y();

    perpendicular_point2[0] = 1.0;
    perpendicular_point2[1] = slope_;
    perpendicular_point2[2] = -point2_.get_x() -slope_*point2_.get_y();

    perpendicular_midpoint[0] = 1.0;
    perpendicular_midpoint[1] = slope_;
    perpendicular_midpoint[2] = -midpoint_.get_x() -slope_*midpoint_.get_y();
};

PointCorridor LineCorridor::get_line_point1() {
    return point1_;
};

PointCorridor LineCorridor::get_line_point2() {
    return point2_;
};

PointCorridor LineCorridor::get_line_midpoint() {
    return midpoint_;
};

double LineCorridor::get_line_slope() {
    return slope_;
};

void LineCorridor::get_line_equation(double input_array[3]) const {
    for(int i = 0; i < 3; ++i) {
        input_array[i] = equation[i];
    }
};

void LineCorridor::get_line_perpendicular_point1(double input_array[3]) const {
    for(int i = 0; i < 3; ++i) {
        input_array[i] = perpendicular_point1[i];
    }
};

void LineCorridor::get_line_perpendicular_point2(double input_array[3]) const {
    for(int i = 0; i < 3; ++i) {
        input_array[i] = perpendicular_point2[i];
    }
};

void LineCorridor::get_line_perpendicular_midpoint(double input_array[3]) const {
    for(int i = 0; i < 3; ++i) {
        input_array[i] = perpendicular_midpoint[i];
    }
};

void LineCorridor::print() {
    std::cout << "Line Point 1: "; point1_.print(); std::cout << '\n';
    std::cout << "Line Point 2: "; point2_.print(); std::cout << '\n';
    std::cout << "Line Midpoint: "; midpoint_.print(); std::cout << '\n';
    std::cout << "Line Slope: " << slope_ << '\n';
    std::cout << "Line Equation: " << equation[0] << "x + " << equation[1] << "y + " << equation[2] << " = 0" << '\n';
    std::cout << "Line Perpendicular through Point 1: " << perpendicular_point1[0] << "x + " << perpendicular_point1[1] << "y + " << perpendicular_point1[2] << " = 0" << '\n';
    std::cout << "Line Perpendicular through Point 2: " << perpendicular_point2[0] << "x + " << perpendicular_point2[1] << "y + " << perpendicular_point2[2] << " = 0" << '\n';
    std::cout << "Line Perpendicular through Mid-Point: " << perpendicular_midpoint[0] << "x + " << perpendicular_midpoint[1] << "y + " << perpendicular_midpoint[2] << " = 0" << '\n';
};


/*
-------------------------------------------------------------------------------
                        Class Corridor Methods
-------------------------------------------------------------------------------
*/

void Corridor::calculate_center_line() {
    PointCorridor center_point1, center_point2, temp;
    double x2, y1, y2;
    double center_equation[3], center_equation_temp[3], opposite_wall_equation[3], opposite_wall_equation_temp[3];
    // double *center_equation, *center_equation_temp, *opposite_wall_equation, *opposite_wall_equation_temp;

    y1 = left_.get_line_point1().get_y();
    y2 = left_.get_line_point2().get_y();
    right_.get_line_equation(opposite_wall_equation);

    if (y1 > y2) {
        center_point1 = left_.get_line_point1();
        left_.get_line_perpendicular_point1(center_equation);
    }
    else {
        center_point1 = left_.get_line_point2();
        left_.get_line_perpendicular_point2(center_equation);
    }

    y1 = right_.get_line_point1().get_y();
    y2 = right_.get_line_point2().get_y();
    left_.get_line_equation(opposite_wall_equation_temp);

    if (y1 > y2) {
        temp = right_.get_line_point1();
        right_.get_line_perpendicular_point1(center_equation_temp);
    }
    else {
        temp = right_.get_line_point2();
        right_.get_line_perpendicular_point2(center_equation_temp);
    }

    if (center_point1.get_radius() > temp.get_radius()) {
        center_point1 = temp;
        for (int i = 0; i < 3; ++i) {
            center_equation[i] = center_equation_temp[i];
            opposite_wall_equation[i] = opposite_wall_equation_temp[i];
        }
    }

    y2 = (center_equation[0]*opposite_wall_equation[2] - opposite_wall_equation[0]*center_equation[2])/(opposite_wall_equation[0]*center_equation[1] - center_equation[0]*opposite_wall_equation[1]);
    x2 = (center_equation[2] + center_equation[1]*y2)/(-center_equation[0]);

    center_point2 = PointCorridor(x2, y2);
    center_ = LineCorridor(center_point1, center_point2);
};

void Corridor::calculate_setpoint() {
    double x1, x2, y1, y2, a, b, c, c1, discriminant, denominator, setpoint_line_equation[3], center_line_equation[3], val1, val2;

    center_.get_line_perpendicular_midpoint(setpoint_line_equation);
    center_.get_line_perpendicular_midpoint(center_line_equation);

    c1 = SETPOINT_CORRIDOR;
    a = setpoint_line_equation[0];
    b = setpoint_line_equation[1];
    c = setpoint_line_equation[2];

    denominator = pow(a, 2) + pow(b, 2);
    discriminant = sqrt(denominator*pow(c1, 2) - pow(c, 2));

    x1 = (-a*c + b*discriminant)/denominator;
    x2 = (-a*c - b*discriminant)/denominator;

    y1 = sqrt(pow(c1, 2) - pow(x1, 2));
    y2 = sqrt(pow(c1, 2) - pow(x2, 2));

    val1 = center_line_equation[0]*x1 + center_line_equation[1]*y1 + center_line_equation[2];
    val2 = center_line_equation[0]*x2 + center_line_equation[1]*y2 + center_line_equation[2];

    if (val1 > 0) {
        setpoint_ = PointCorridor(x1, y1);
    }
    else {
        setpoint_ = PointCorridor(x2, y2);
    }

};

LineCorridor Corridor::get_corridor_line_left() {
    return left_;
};

LineCorridor Corridor::get_corridor_line_right() {
    return right_;
};

LineCorridor Corridor::get_corridor_line_center() {
    return center_;
};

PointCorridor Corridor::get_corridor_setpoint() {
    return setpoint_;
};


/*---------------------------------------------------------------------------------
---------------------------inside the room-----------------------------------------
---------------------------------------------------------------------------------*/

// set furthest point
Point_det set_furthest_point(Point point){
    Point_det absolute_furthest;
    absolute_furthest.dist = sqrt(point.x*point.x + point.y*point.y);
    absolute_furthest.x = point.x;
    absolute_furthest.y = point.y;
    absolute_furthest.angle = atan(point.y/point.x);
    return absolute_furthest;
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
Destination *calc_furthest_dest (Point_det furthest, Destination *dest){
    dest->x = furthest.x/4;
    dest->y = furthest.y/4;
    dest->angle = furthest.angle;
    return dest;
}


// check whether the new point is further than the saved one
Point_det compare_furthest_point(Point point, Point_det absolute_furthest){
    double new_dist;
    new_dist = sqrt(point.x*point.x + point.y*point.y);
    if (new_dist>absolute_furthest.dist) {
        absolute_furthest = set_absolute_furthest(point);
    }
    return absolute_furthest;
}



void room_logic(Exit exit, Point_det current_furthest,  Destination *dest){


    bool set_turn_flag;
    bool turned_once_flag;
    bool move_to_exit;
    Point_det absolute_furthest;
    //Destination dest;

    // initialize
    absolute_furthest.dist = 0;
    absolute_furthest.angle = 0;
    absolute_furthest.x = 0;
    absolute_furthest.y = 0;
    turned_once_flag = false;   // assume always start without turning



    // check if the exit detected
    if ((exit.detected) || (move_to_exit)) {

        move_to_exit = true;
        dest = calc_exit_dest(exit,dest);        // define destination

    } else {

        // check if we have turned around already
        if (!turned_once_flag){
            set_turn_flag = true;           // command control to turn 180
            turned_once_flag = true;        // remember that we've turned around
            absolute_furthest = set_furthest_point(current_furthest);     // save the furthest point given info from perception
        } else {

            // check if exit detected after we turned
            if ((exit.detected) || (move_to_exit)) {
                move_to_exit = true;
                dest = calc_exit_dest(exit,dest);        // define destination
            } else {
                absolute_furthest = compare_furthest_point(current_furthest,absolute_furthest);     // check if the first point was further
                dest = calc_furthest_dest(absolute_furthest,dest);             // set the furthest point as a destination
            }
        }


    }

}
