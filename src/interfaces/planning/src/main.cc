#include "planning.h"
#include <iostream>

using namespace std;

int main() {
    Point p(3.0, 2.0), q(7.0, -4.0), test;
    Line l1(p, q);
    double eqn[3];

    cout << "First point is "; p.print(); cout << '\n';
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

    return 0;
}
