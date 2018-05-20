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

};

void Corridor::calculate_setpoint() {

};

Line Corridor::get_corridor_line_left() {

};

Line Corridor::get_corridor_line_right() {

};

Line Corridor::get_corridor_line_center() {

};

Point Corridor::get_corridor_setpoint() {

};
