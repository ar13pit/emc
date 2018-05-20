#include "planning.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

int main() {
    Destination dest;
    Exit exit;
    Point p(3.0, 5.0);
    Status st;
    //Furthest_point far;

    exit.detected = false;
    exit.x1 = 30.0;
    exit.x2 = 40.0;
    exit.y1 = 10.0;
    exit.y2 = 10.0;
    exit.angle1 = atan(exit.y1/exit.x1);
    exit.angle2 = atan(exit.y2/exit.x2);
    st = ROOM_ESCAPE;

    double temp;
    temp = exit.y1/exit.x1;


    main_logic(exit, p, st, &dest);

    cout << "Destination (x,y) coordinates (" << dest.x << ", " << dest.y <<")"  << endl;
    cout << "Destination angle " << dest.angle <<endl;
    cout << "Initial angle exit " << exit.angle1 <<" " << exit.angle2 <<endl;
    cout << "Test " << temp <<endl;


    /*cout << "First point is "; p.print(); cout << '\n';
    cout << "Second point is "; q.print(); cout << '\n' << '\n';
    cout << "Line details: \n"; l1.print(); cout << '\n';

    test = l1.get_line_point1();
    test.print(); cout << '\n';

    test = l1.get_line_point2();
    test.print(); cout << '\n';

    test = l1.get_line_midpoint();
    test.print(); cout << '\n';

    l1.get_line_equation(eqn);
    cout << "Line Equation: " << eqn[0] << "x + " << eqn[1] << "y + " << eqn[2] << " = 0" << '\n';

    l1.get_line_perpendicular1(eqn);
    cout << "Line Perpendicular 1: " << eqn[0] << "x + " << eqn[1] << "y + " << eqn[2] << " = 0" << '\n';

    l1.get_line_perpendicular2(eqn);
    cout << "Line Perpendicular 2: " << eqn[0] << "x + " << eqn[1] << "y + " << eqn[2] << " = 0" << '\n';
*/
    return 0;
}
