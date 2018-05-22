#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv ;
int main ( )
{
// Create b lack empty images

Mat image = Mat::zeros(400,400,CV_8UC3 ) ;
// Draw a circle
cv::circle(image,Point(200,200),32.0,Scalar(100,150,0),1,8);

// Draw a rectangle
rectangle(image,Point(10,20),Point(40,50),Scalar(0,55,255),+1,4);

// Draw a filled rectangle 
rectangle(image,Point(215,320),Point(270,350),Scalar(140,255,255),+1,8);

imshow( "Image" , image ) ;
waitKey ( 0 ) ;
return ( 0 ) ;

}
