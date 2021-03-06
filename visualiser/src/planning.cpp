#include "planning.h"



/*
-------------------------------------------------------------------------------
                        Class PointCorridor Methods
-------------------------------------------------------------------------------
*/

void PointCorridor::calculate_xangle() {
    angle_ = atan(y_/x_);
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

double PointCorridor::get_xangle() {
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
    // right_.print();

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
    // left_.print();

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
            // std::cout << "/* message */" << '\n';
        }
    }

    // center_point1.print(); std::cout << '\n';

    y2 = (center_equation[0]*opposite_wall_equation[2] - opposite_wall_equation[0]*center_equation[2])/(opposite_wall_equation[0]*center_equation[1] - center_equation[0]*opposite_wall_equation[1]);
    x2 = (center_equation[2] + center_equation[1]*y2)/(-center_equation[0]);

/*   std::cout << x2*center_equation[0] + y2*center_equation[1] + center_equation[2] << '\n';
    std::cout << x2*opposite_wall_equation[0] + y2*opposite_wall_equation[1] + opposite_wall_equation[2] << '\n';
*/
    center_point2 = PointCorridor(x2, y2);
    center_ = LineCorridor(center_point1, center_point2);
};

void Corridor::calculate_setpoint() {
    // double x1, x2, y1, y2, a, b, c, c1, discriminant, denominator, setpoint_line_equation[3], center_line_equation[3], val1, val2;
    double x2, y2, a, b, c, setpoint_line_equation[3];

    center_.get_line_perpendicular_midpoint(setpoint_line_equation);
    // center_.get_line_perpendicular_midpoint(center_line_equation);

    // c1 = SETPOINT_CORRIDOR;
    a = setpoint_line_equation[0];
    b = setpoint_line_equation[1];
    c = setpoint_line_equation[2];

    x2 = (b + c)/-a;

    //
    // denominator = pow(a, 2) + pow(b, 2);
    // discriminant = sqrt(denominator*pow(c1, 2) - pow(c, 2));
    //
    // std::cout << "devision test " << denominator << "\n";
    //
    // x1 = (-a*c + b*discriminant)/denominator;
    // x2 = (-a*c - b*discriminant)/denominator;
    //
    // y1 = sqrt(pow(c1, 2) - pow(x1, 2));
    // y2 = sqrt(pow(c1, 2) - pow(x2, 2));
    //
    // val1 = center_line_equation[0]*x1 + center_line_equation[1]*y1 + center_line_equation[2];
    // val2 = center_line_equation[0]*x2 + center_line_equation[1]*y2 + center_line_equation[2];
    //
    // if (val1 > 0) {
    //     setpoint_ = PointCorridor(x1, y1);
    // }
    // else {
    setpoint_ = PointCorridor(x2, 1);
    // }

    std::cout << "Corridor Setpoint (" << setpoint_.get_x() << " " << setpoint_.get_y() << ") X-Angle: "<< setpoint_.get_xangle() << "\n";



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


// trasformation of the corridor representation to Destination format
void Planning::corrid2dest_transf(Corridor corr, Detection_data *data){
    double temp_angle;
/*    std::cout <<"Corridor setpoint" << corr.get_corridor_setpoint().get_x() << "\n"<< "\n";
    std::cout <<"Corridor setpoint" << corr.get_corridor_setpoint().get_y() << "\n"<< "\n";
    std::cout <<"Corridor setpoint" << corr.get_corridor_setpoint().get_angle() << "\n"<< "\n";
*/
    dest.dist = distance_calc(corr.get_corridor_setpoint().get_x(), corr.get_corridor_setpoint().get_y());
    temp_angle = corr.get_corridor_setpoint().get_xangle();
    if (temp_angle < 0) {
        // std::cout << "Corridor X-Angle test absolute value: " << -temp_angle << '\n';
        dest.angle = -(M_PI/2) - temp_angle;
        // std::cout << "Calculation Test: " << -(M_PI/2) - temp_angle << '\n';
        std::cout << "Destination Angle to turn: " << dest.angle << '\n';

    }
    else {
        // std::cout << "Corridor X-Angle test absolute value: " << temp_angle << '\n';
        dest.angle = (M_PI/2) - temp_angle;
        // std::cout << "Calculation Test: " << (M_PI/2) - temp_angle << '\n';
        std::cout << "Destination Angle to turn: " << dest.angle << '\n';

    }

    // dest.angle = corr.get_corridor_setpoint().get_xangle() - M_PI - TURN_COMPLETE;
}


/*---------------------------------------------------------------------------------
---------------------------inside the room-----------------------------------------
---------------------------------------------------------------------------------*/

// distance calculation
double Planning::distance_calc(double x, double y){
    double dist;
    dist = sqrt(pow(x,2)+pow(y,2));
    return dist;
}

// set furthest point
void Planning::set_furthest_point(Point_det *point){
    absolute_furthest.dist = point->dist;
    absolute_furthest.x = point->x;
    absolute_furthest.y = point->y;
    absolute_furthest.angle = point->angle; // add pi because we assume turn at next instant
}


// calculate the destination when exit identified
void Planning::calc_exit_dest (Detection_data *data){

    double x;
    double y;

    x = (data->exit.exitPoint_det1.x + data->exit.exitPoint_det2.x)/2;
    y = (data->exit.exitPoint_det1.y + data->exit.exitPoint_det2.y)/2;
    dest.angle = (data->exit.exitPoint_det1.angle + data->exit.exitPoint_det2.angle)/2;

    dest.dist = distance_calc(x, y);


    if (dest.dist > DIST_SETPOINT)
        dest.dist = DIST_SETPOINT;


}


// calculate the destination when exit in not identified
void Planning::calc_furthest_dest (Point_det furthest){

    dest.dist = distance_calc(furthest.x/4, furthest.y/4);
    dest.angle = furthest.angle;
}


// check whether the new point is further than the saved one
void Planning::compare_furthest_point(Point_det *point){
    if (point->dist > absolute_furthest.dist) {
        set_furthest_point(point);
    }
}

void Planning::turn_around(){
    dest.angle = M_PI;
    dest.dist = 0;
}

void Planning::exit_center_realign(Detection_data * data){
    dest.dist = (data->exit.exitPoint_det1.dist - data->exit.exitPoint_det2.dist)/2;
    if (dest.dist<0) {
        dest.angle = data->exit.exitPoint_det2.angle;
    } else {
        dest.angle = data->exit.exitPoint_det1.angle;
    }
}


//bool Planning::check_corridor(Detection_data *data){
//    double dist_x;
//    double dist_y;
//    double dist_c;


//    dist_x = (data->exit.exitPoint_det1.x + data->exit.exitPoint_det2.x)/2;
//    dist_y = (data->exit.exitPoint_det1.y + data->exit.exitPoint_det2.y)/2;

//    dist_c = sqrt(pow(dist_x,2)+pow(dist_y,2));

//    std::cout << "distance c " << dist_c << "\n";

//    if (dist_c < THRESHOLD_CORRIDOR ){
//        std::cout << "Threshold not active" << "\n"<< "\n";
//        return 1;
//    } else {
//        std::cout << "Threshold is active" << "\n" << "\n";
//        return 0;
//    }
//}



void Planning::room_logic(Detection_data *data, Flags *flags, Destination *far_point){
    if (flags->in_corridor == true){
        std::cout << "In corridor = true" <<std::endl;
    } else {
        std::cout << "In corridor = false" <<std::endl;
    }

    if (data->exit.detected == true){
        std::cout << "Exit detected = true" <<std::endl;
    } else {
        std::cout << "Exit detected = false" <<std::endl;
    }

    Point_det current_furthest = data->furthest_point;

    // check if the exit detected
    if (data->exit.detected){
        calc_exit_dest(data);        // define destination
        flags->drive_frw = true;
    } else {

        // check if we have turned around already
        if (!flags->turned_once){
            flags->turned_once = true;        // remember that we've turned around
            set_furthest_point(&current_furthest);     // save the furthest point given info from perception
            turn_around();
            flags->turn = true;
            flags->drive_frw = false;

            calc_furthest_dest (absolute_furthest);
            far_point = &dest;
            far_point->angle = M_PI;//far_point->angle + M_PI;

            std::cout << "Turn to " << far_point->angle << std::endl;
            std::cout << "Turn around" <<std::endl;
        } else {
            std::cout << "Furthest point " << std::endl;
            absolute_furthest.x = far_point->x;
            absolute_furthest.y = far_point->y;
            absolute_furthest.angle = far_point->angle;
            absolute_furthest.dist = distance_calc(far_point->x, far_point->y);

            compare_furthest_point(&current_furthest);     // check if the first point was further
            calc_furthest_dest(absolute_furthest);             // set the furthest point as a destination
            flags->turned_once = false;
            flags->drive_frw = true;
        }
    }

/*    if (dest.angle>M_PI){
        dest.angle = M_PI;
        std::cout << "Reference angle is too large" << std::endl;
    }
*/
}




/*
// main function for decisions inside the block
void Planning::planning(Detection_data *data, Flags *flags){

    if (flags->in_corridor == true){
        std::cout << "signal in corridor = true" <<std::endl;
    } else {
        std::cout << "signal in corridor = false" <<std::endl;
    }

    if (!flags->in_corridor) {
        room_logic(data, flags);
    }
    else if (flags->in_corridor){
        PointCorridor left1(data->corridor.leftWall1.x, data->corridor.leftWall1.y);
        PointCorridor left2(data->corridor.leftWall2.x, data->corridor.leftWall2.y);
        PointCorridor right1(data->corridor.rightWall1.x, data->corridor.rightWall1.y);
        PointCorridor right2(data->corridor.rightWall2.x, data->corridor.rightWall2.y);

        LineCorridor leftLine(left1, left2);
        LineCorridor rightLine(right1, right2);
        Corridor corridor(leftLine, rightLine);

        // assign destination point
        corrid2dest_transf(corridor);
    } else {
 //       std::cout << "Fatal error: not in a room and not in a corridor" << endl;
    }

}
*/

Destination Planning::get_Destination(){
    return dest;
}
