#include "mapping.hpp"

/*
--------------------------------------------------------------------------------
                                CHANGELOG
--------------------------------------------------------------------------------
1. Removed init_map() as WorldModel Constructor initializes all the global data.
2. Setting of data to WorldModel only happens in execute_mapping.
3. Get all required data from WorldModel at once in execute_mapping to remove
   redundancies.
4. Data from odom not being read anywhere.
5. Jari must stop copying comments!
6. Not sure how well would update_global_pos() work

*/


Mapping::Mapping (emc::IO* io, WorldModel* worldModel) : inOut(io), WM(worldModel), r(EXECUTION_RATE) {};


//int show_canvas(emc::LaserData scan)

void Mapping::execute_mapping () {

    std::cout<<"1"<<std::endl;
    map = WM->get_mapping().map;

    totalCorners = WM->get_allDetectedCorners();
    totalExits = WM->get_allDetectedExits();

    localDetection = WM->get_localDetection();

    for(int i = 0; i < 40; ++i){
        exits_local[i] = WM->get_localDetection().Exits_total[i];
        corners_local[i] = WM->get_localDetection().Corners_total[i];
    }

    currentRoomID = WM->get_currentRoomID();

    std::cout<<"2"<<std::endl;
    update_global_pos();

    std::cout<<"3"<<std::endl;
    update_rooms();

    std::cout<<"4"<<std::endl;
    update_corners();

    std::cout<<"5"<<std::endl;
    mapdata.map = map;
    mapdata.pico_position = global_pos;

    WM->set_mapping(mapdata);
}

void Mapping::update_global_pos () {

    //update_global_pos();
    while (!inOut->readOdometryData(odom)) { r.sleep(); }

    //First thing you check if results are total crap
    global_pos.x = -odom.y;
    global_pos.y = odom.x;
    global_pos.angle(-odom.a);

    // WM->set_globalPosition(global_pos);

    //STUFF WILL BE ADDED HERE TO ENSURE UPDATES OF THE LRF DATA

    //global_pos.x = odom_diff.x + global_pos.x;
    //global_pos.y = odom_diff.y + global_pos.y;
    //global_pos.angle = odom_diff.angle + global_pos.angle;

}


// Check if there is a new room found by comparing existing globals exits and current local exits
void Mapping::update_rooms () {

    Exit exit_found;
    Exit exit_map;

    Point center_exit_found;
    Point center_exit_map;

    bool detected_already;
    double distance;

    for (int i = 0; i < 40; ++i) { // ROBUSTNESS ISSUES, PAY ATTENTION

        // WHY USE THIS IF CONDITION? Shouldn't exit_local only have exits with detected flag? Makes no sense!
        if (exits_local[i].detected()) {

            exit_found.exitPoint1 = local2global(exits_local[i].exitPoint1);
            exit_found.exitPoint2 = local2global(exits_local[i].exitPoint2);

            center_exit_found.x = (exit_found.exitPoint1.x + exit_found.exitPoint2.x) * 0.5;
            center_exit_found.y = (exit_found.exitPoint1.y + exit_found.exitPoint2.y) * 0.5;

            detected_already = false;
            for (int j = 0; j < totalExits.size(); ++j) {

                exit_map = totalExits[j];

                center_exit_map.x = (exit_map.exitPoint1.x + exit_map.exitPoint2.x) * 0.5;
                center_exit_map.y = (exit_map.exitPoint1.y + exit_map.exitPoint2.y) * 0.5;


                //Point_map corner_map = map[currentRoom].corners[j];
                distance = sqrt(pow(center_exit_map.x - center_exit_found.x, 2) + pow(center_exit_map.y - center_exit_found.y, 2));
                if (distance < MAPPING_UNCERTAINTY) {
                    detected_already = true;
                    break;
                }
            }

            if (detected_already == false) {
                if (!(exit_found.exitPoint1.angle() - exit_found.exitPoint2.angle() < 0.1*M_PI)){
                    //map[currentRoom].corners[map[currentRoom].corners.size()] = corner_found;
                    totalExits.push_back(exit_found);
                    int nRooms = map.size();
                    Room room;
                    room.exit = exit_found;
                    room.roomID = nRooms + 1;
                    room.previousRoom = currentRoomID;

                    // WTF are you tring to do here JARI?
                    // std::vector<Point> corners;
                    // room.corners = corners;

                    map.push_back(room);
                }
            }

        }
        else {
            break;
        }
    }
    // WorldModel::set_globalRooms(map[map.size()]);
}


// Update corners in the total corner vector and the current room vector based on current room
void Mapping::update_corners () {

    // WTF are you tring to do here JARI?
    // int currentRoom = 0;    //worldmodel->get_currentRoom();

    Point corner_found;
    Point corner_map;

    bool detected_already;

    double distance;

    for(int i = 0; i < 40; ++i) { // ROBUSTNESS ISSUES, PAY ATTENTION

        // WHY USE THIS IF CONDITION? Shouldn't exit_local only have exits with detected flag? Makes no sense!
        if (corners_local[i].detected()) {

            corner_found = local2global(corners_local[i].cornerPoint);

            detected_already = false; // by default
            for (int j = 0; j < totalCorners.size(); ++j) {

                corner_map = totalCorners[j];
                //Point_map corner_map = map[currentRoom].corners[j];
                distance = sqrt(pow(corner_map.x - corner_found.x, 2) + pow(corner_map.y - corner_found.y, 2));
                if (distance < MAPPING_UNCERTAINTY) {
                    detected_already = true;
                    break;
                }
            }

            if (detected_already == false) {

                map[currentRoomID].corners.push_back(corner_found);
                totalCorners.push_back(corner_found);

            }

        }
        else {
            break;
        }
    }
}



Point Mapping::local2global (Point local) { //Convert local into global coordinate

    Point temp;

    temp.x = global_pos.x + local.dist() * sin(local.angle() + global_pos.angle());
    temp.y = global_pos.y + local.dist() * cos(local.angle() + global_pos.angle());

    return temp;
}



// METHODS BELOW ARE IMPORTANT IF UPDATES GLOBAL POSITION BASED ON LRF ARE IMPLEMENTED, COMMENTED FOR NOW

//void Mapping::update_Odometry(){
//    while(!inOut->readOdometryData(odom));
//}

//void Mapping::delta_Odometry(){

//odom_diff.x = global_pos.x - latest_odom.x;
//odom_diff.y = global_pos.y - latest_odom.y;
//odom_diff.angle = global_pos.angle - latest_odom.angle;

//}

//update the map into the world model
//the only function that is executed, so it has to include all the methods needed
//void Mapping::update_map(){

//}
