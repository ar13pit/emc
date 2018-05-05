#include <emc/io.h>
#include <emc/rate.h>
#include <iostream>
using namespace std;

int main()
{
    // Create IO object, which will initialize the io layer
    emc::IO io;

    // Create Rate object, which will help using keeping the loop at a fixed frequency
    emc::Rate r(10);
    emc::LaserData scan;
    emc::OdometryData odom;

    double maxDistWall = 0.5;
    bool wall = false;

    // Loop while we are properly connected
    while(io.ok())
    {

        // Sleep remaining time
        r.sleep();

        if(io.readLaserData(scan))
        {
            for(unsigned int i = 0; i < scan.ranges.size(); ++i)
            {
                if (scan.ranges[i] < maxDistWall)
                {
                    wall = true;
                }
                if (wall)
                {
                    io.sendBaseReference(0, 0, 0);
                }
                else
                {
                    io.sendBaseReference(0.8, 0, 0);
                }
            }
            if (io.readOdometryData(odom))
            {
                cout << "x position = " << odom.x << endl;
            }
        }
    }

    return 0;
}
