#include <emc/io.h>
#include <emc/rate.h>
#include <emc/odom.h>
#include <cmath>
#include <iostream>
#include <cstdlib>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <emc/io.h>
#include <string>
#include <sstream>
#include <iostream>
#include <emc/rate.h>


#include "driveControl.h"
#include "detection.h"
#include "worldModel.h"
#include "planning.h"
#include "config.h"


using namespace std;
using namespace cv;


void initialize(Detection_data *data, Destination *dest, Flags *flags)
{

    // Flags

    flags->in_corridor = false;
    flags->turn = false;
    flags->turned_once = false;
    flags->escaped = false;
    flags->drive_frw = false;



    // Destination

    dest->angle = 0;
    dest->dist = 0;



    // Detection data

    //corridor initialization
    data->corridor.escaped = false;

    data->corridor.leftWall1.angle = 0;
    data->corridor.leftWall1.dist = 0;
    data->corridor.leftWall1.x = 0;
    data->corridor.leftWall1.y = 0;

    data->corridor.leftWall2.angle = 0;
    data->corridor.leftWall2.dist = 0;
    data->corridor.leftWall2.x = 0;
    data->corridor.leftWall2.y = 0;

    data->corridor.rightWall1.angle = 0;
    data->corridor.rightWall1.dist = 0;
    data->corridor.rightWall1.x = 0;
    data->corridor.rightWall1.y = 0;

    data->corridor.rightWall2.angle = 0;
    data->corridor.rightWall2.dist = 0;
    data->corridor.rightWall2.x = 0;
    data->corridor.rightWall2.y = 0;


    // exit initialization
    data->exit.detected = false;

    data->exit.exitPoint_det1.angle = 0;
    data->exit.exitPoint_det1.dist = 0;
    data->exit.exitPoint_det1.x = 0;
    data->exit.exitPoint_det1.y = 0;

    data->exit.exitPoint_det2.angle = 0;
    data->exit.exitPoint_det2.dist = 0;
    data->exit.exitPoint_det2.x = 0;
    data->exit.exitPoint_det2.y = 0;


    // furthest point initialization
    data->furthest_point.angle = 0;
    data->furthest_point.dist = 0;
    data->furthest_point.x = 0;
    data->furthest_point.y = 0;


}

double resolution = 0.01;
cv::Point2d canvas_center;

cv::Point2d worldToCanvas(double x, double y)
{
    return cv::Point2d(-y / resolution, -x / resolution) + canvas_center;
}

int main(int argc, char *argv[])
{
    // Initialization of Robot
    emc::Rate r(2); //EXECUTION_RATE
    emc::IO io;
    //    emc::OdometryData odom;

    // Transition values
    Detection_data data;
    Destination dest;
    Destination far_point;
    Flags flags;
    initialize(&data, &dest, &flags);



    // Initialize the Classes
    DriveControl pico_drive(&io);
    Detection detection(&r, &io, &flags);

     while(io.ok()) {
        if(detection.get_data(&r)) {

//            if (data.exit.detected){
//                flags.in_corridor = true;
//            }

            Detection detection(&r, &io, &flags);
            //should be function to update flags instead
            data = detection.get_Detection_data();

            if (data.exit.detected){
                cout << "exit detected: true" << endl;
            } else{
                cout << "exit detected: false " << endl;
            }

//            if (flags.in_corridor){
//                data.exit.detected = false;
//            }

            cout << "exit Point 1 (x,y) " << data.exit.exitPoint_det1.x <<" "<< data.exit.exitPoint_det1.y <<endl;
            cout << "exit Point 2 (x,y) " << data.exit.exitPoint_det2.x <<" "<< data.exit.exitPoint_det2.y <<endl;

//            cout << "Furthest point (" << data.furthest_point.x << " " << data.furthest_point.y << ")" <<"\n" << "\n";

            cout << "flag in corridor " << flags.in_corridor << "\n";


            Planning plan(&data, &flags, &far_point);
            dest = plan.get_Destination();


            cout << "Relative angle " << dest.angle << endl << endl;

            pico_drive.picoDrive(&dest, &flags);


            cout <<"----------------------------" << endl << endl;
        }

        int show_canvas(emc::LaserData scan){

        Mat canvas = cv::Mat(500, 500, CV_8UC3, cv::Scalar(50, 50, 50));
        canvas_center = cv::Point2d(canvas.rows / 2, canvas.cols / 2);

        // Draw PICO
        cv::Scalar robot_color(0, 0, 255);
        std::vector<std::pair<double, double> > robot_points;
        robot_points.push_back(std::pair<double, double>( 0.1,  -0.2));
        robot_points.push_back(std::pair<double, double>( 0.1,  -0.1));
        robot_points.push_back(std::pair<double, double>( 0.05, -0.1));
        robot_points.push_back(std::pair<double, double>( 0.05,  0.1));
        robot_points.push_back(std::pair<double, double>( 0.1,   0.1));
        robot_points.push_back(std::pair<double, double>( 0.1,   0.2));
        robot_points.push_back(std::pair<double, double>(-0.1,   0.2));
        robot_points.push_back(std::pair<double, double>(-0.1,  -0.2));

        for(unsigned int i = 0; i < robot_points.size(); ++i)
        {
            unsigned int j = (i + 1);  robot_points.size();
            cv::Point2d p1 = worldToCanvas(robot_points[i].first, robot_points[i].second);
            cv::Point2d p2 = worldToCanvas(robot_points[j].first, robot_points[j].second);
            cv::line(canvas, p1, p2, robot_color, 2);
        }

    emc::LaserData scan;
    if(!io.readLaserData(scan)){
            continue;

        // Draw laser data
        double a = scan.angle_min;
        for(unsigned int i = 0; i < scan.ranges.size(); ++i)
        {
            double x = cos(a) * scan.ranges[i];
            double y = sin(a) * scan.ranges[i];

            cv::Point2d p = worldToCanvas(x,y);
            if (p.x >= 0 && p.y >= 0 && p.x < canvas.cols && p.y < canvas.rows)
                canvas.at<cv::Vec3b>(p) = cv::Vec3b(255, 255, 255);

            a += scan.angle_increment;
        }


        // Corridor lines
        cv::Point2d Wall_left_1 = worldToCanvas(data.corridor.leftWall1.x, data.corridor.leftWall1.y);
        cv::Point2d Wall_left_2 = worldToCanvas(data.corridor.leftWall2.x, data.corridor.leftWall2.x);
        cv::Point2d Wall_right_1 = worldToCanvas(data.corridor.rightWall1.x, data.corridor.rightWall1.y);
        cv::Point2d Wall_right_2 = worldToCanvas(data.corridor.rightWall2.x, data.corridor.rightWall2.y);

        line(canvas, Wall_left_1, Wall_left_2, Scalar(255,255,255),2,8);
        line(canvas, Wall_right_1, Wall_right_2, Scalar(255,255,255),2,8);


    //    Set Point
    //    cv::Point2d setpoint = worldToCanvas(dest); // input x,y coordinates from planning
    //    circle(canvas,setpoint,5,Scalar(0,255,255),5,8,0);
    //    putText(canvas,"set Point",cv::Point2d(25,85),FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,255,255));

        // Show legend
        putText(canvas,"Pico", cv::Point2d(25,25), FONT_HERSHEY_SIMPLEX, 0.5, Scalar( 0, 0, 255 ));
        putText(canvas,"Laser", cv::Point2d(25,45), FONT_HERSHEY_SIMPLEX, 0.5, Scalar( 255, 255, 255 ));

        // Show canvas
        imshow("PICO Visualisation", canvas);
        waitKey(3);

        r.sleep();
        std::cout << "Press Enter to continue ..." << '\n';
        cin.get();
    }

    return 0;
        }
}
}
