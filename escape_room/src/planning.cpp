#include "planning.h"


/*
-------------------------------------------------------------------------------
                        Class Point Methods
-------------------------------------------------------------------------------
*/

void Point::calculate_angle() {
    angle_ = ((PI/2) - atan(y_/x_));
};

void Point::calculate_radius() {
    radius_ = sqrt(pow(x_, 2) + pow(y_, 2));
};

double Point::get_x() {
    return x_;
};

double Point::get_y() {
    return y_;
};

double Point::get_angle() {
    return angle_;
};

double Point::get_radius() {
    return radius_;
};

void Point::print() {
    std::cout << "Coordinates: (" << x_ << ", " << y_ << "), Radius: " << radius_ << " Angle: " << angle_ ;
}


/*
-------------------------------------------------------------------------------
                        Class Line Methods
-------------------------------------------------------------------------------
*/

void Line::calculate_midpoint() {
    double x_mid, y_mid;

    x_mid = (point1_.get_x() + point2_.get_x())/2;
    y_mid = (point1_.get_y() + point2_.get_y())/2;
    midpoint_ = Point(x_mid, y_mid);
};

void Line::calculate_slope() {
    slope_ = (point1_.get_y() - point2_.get_y())/(point1_.get_x() - point2_.get_x());
};

void Line::calculate_equation() {
    equation[0] = slope_;
    equation[1] = -1.0;
    equation[2] = -slope_*point1_.get_x() + point1_.get_y();
};

void Line::calculate_perpendicular(){
    perpendicular_point1[0] = 1.0;
    perpendicular_point1[1] = slope_;
    perpendicular_point1[2] = -point1_.get_x() -slope_*point1_.get_y();

    perpendicular_point2[0] = 1.0;
    perpendicular_point2[1] = slope_;
    perpendicular_point2[2] = -point2_.get_x() -slope_*point2_.get_y();
};

Point Line::get_line_point1() {
    return point1_;
};

Point Line::get_line_point2() {
    return point2_;
};

Point Line::get_line_midpoint() {
    return midpoint_;
};

double Line::get_line_slope() {
    return slope_;
};

void Line::get_line_equation(double input_array[3]) const {
    for(int i = 0; i < 3; ++i) {
        input_array[i] = equation[i];
    }
};

void Line::get_line_perpendicular1(double input_array[3]) const {
    for(int i = 0; i < 3; ++i) {
        input_array[i] = perpendicular_point1[i];
    }
};

void Line::get_line_perpendicular2(double input_array[3]) const {
    for(int i = 0; i < 3; ++i) {
        input_array[i] = perpendicular_point2[i];
    }
};

void Line::get_line_midpoint(double input_array[3]) const {
    for(int i = 0; i < 3; ++i) {
        input_array[i] = perpendicular_midpoint[i];
    }
};

void Line::print() {
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
    Point center_point1, center_point2, temp;
    double x2, y1, y2, center_equation[3], center_equation_temp[3], opposite_wall_equation[3], opposite_wall_equation_temp[3];

    y1 = left_.get_line_point1().get_y();
    y2 = left_.get_line_point2().get_y();
    right_.get_line_equation(opposite_wall_equation);

    if (y1 > y2) {
        center_point1 = left_.get_line_point1();
        left_.get_line_perpendicular1(center_equation);
    }
    else {
        center_point1 = left_.get_line_point2();
        left_.get_line_perpendicular2(center_equation);
    }

    y1 = right_.get_line_point1().get_y();
    y2 = right_.get_line_point2().get_y();
    left_.get_line_equation(opposite_wall_equation_temp);

    if (y1 > y2) {
        temp = right_.get_line_point1();
        right_.get_line_perpendicular1(center_equation_temp);
    }
    else {
        temp = right_.get_line_point2();
        right_.get_line_perpendicular2(center_equation_temp);
    }

    if center_point1.get_radius() > temp.get_radius() {
        center_point1 = temp;
        center_equation = center_equation_temp;
        opposite_wall_equation = opposite_wall_equation_temp;
    }

    y2 = (center_equation[0]*opposite_wall_equation[2] - opposite_wall_equation[0]*center_equation[2])/(opposite_wall_equation[0]*center_equation[1] - center_equation[0]*opposite_wall_equation[1]);
    x2 = (center_equation[2] + center_equation[1]*y2)/(-center_equation[0]);

    center_point2 = Point(x2, y2);
    center_ = Line(center_point1, center_point2);
};

void Corridor::calculate_setpoint() {
    double x1, x2, y1, y2, a, b, c, c1, discriminant, denominator setpoint_line_equation[3], center_line_equation[3], val1, val2;

    center_.get_line_midpoint(setpoint_line_equation);
    center_.get_line_midpoint(center_line_equation);

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
        setpoint_ = Point(x1, y1);
    }
    else {
        setpoint_ = Point(x2, y2);
    }

};

Line Corridor::get_corridor_line_left() {
    return left_;
};

Line Corridor::get_corridor_line_right() {
    return right_;
};

Line Corridor::get_corridor_line_center() {
    return center_;
};

Point Corridor::get_corridor_setpoint() {
    return setpoint_;
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
