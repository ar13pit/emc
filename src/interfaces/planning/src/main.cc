#include "planning.h"
#include <iostream>

using namespace std;

int main() {
    PointCorridor p(2.0, 2.0), q(6.0, 4.0), r(2.0, -3.0), s(6.0, -1.0), test;
    LineCorridor l1, l2, testLine;
    double eqn[3];
    Corridor corridor;

    l1 = LineCorridor(p,q);
    l2 = LineCorridor(r,s);
    corridor = Corridor(l1, l2);
    testLine = corridor.get_corridor_line_center();
    test = corridor.get_corridor_setpoint();

    cout << "Left Point 1 is "; p.print(); cout << '\n';
    cout << "Left Point 2 is "; q.print(); cout << '\n';
    cout << "Right Point 1 is "; r.print(); cout << '\n';
    cout << "Right Point 2 is "; s.print(); cout << '\n' << '\n';

    cout << "Left Line details: \n"; l1.print(); cout << '\n' << '\n';
    cout << "Right Line details: \n"; l2.print(); cout << '\n' << '\n';
    cout << "Corridor Center Line details: \n"; testLine.print(); cout << '\n' << '\n';

    cout << "Corridor Set-Point is "; test.print(); cout << '\n';

    // test = l1.get_line_point1();
    // test.print(); cout << '\n';
    //
    // test = l1.get_line_point2();
    // test.print(); cout << '\n';
    //
    // test = l1.get_line_midpoint();
    // test.print(); cout << '\n';
    //
    // l1.get_line_equation(eqn);
    // cout << "Line Equation: " << eqn[0] << "x + " << eqn[1] << "y + " << eqn[2] << " = 0" << '\n';
    //
    // l1.get_line_perpendicular_point1(eqn);
    // cout << "Line Perpendicular 1: " << eqn[0] << "x + " << eqn[1] << "y + " << eqn[2] << " = 0" << '\n';
    //
    // l1.get_line_perpendicular_point2(eqn);
    // cout << "Line Perpendicular 2: " << eqn[0] << "x + " << eqn[1] << "y + " << eqn[2] << " = 0" << '\n';
    //
    // l1.get_line_perpendicular_midpoint(eqn);
    // cout << "Line Perpendicular through Mid-Point: " << eqn[0] << "x + " << eqn[1] << "y + " << eqn[2] << " = 0" << '\n';

    return 0;
}
