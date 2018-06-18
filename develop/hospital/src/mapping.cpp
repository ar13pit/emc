#include "mapping.h"

Mapping::Mapping (emc::IO* io, WorldModel* worldModel) : inOut(io), WM(worldModel) {
        odom = emc::OdometryData();
}


void Mapping::execute_mapping () {
    Mapping_data mapdata;

    std::cout<<"1"<<std::endl;
    map = WM->get_mapping().map;
    totalCorners = WM->get_allDetectedCorners();
    totalExits = WM->get_allDetectedExits();

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


//int show_canvas(emc::LaserData scan)
void Mapping::init_map ()
{
    Point globalpos;
    Point exitPoint_corr1;
    Point exitPoint_corr2;

    Exit corridorExit;

    std::vector<Point> corridorcorners;

    Room corridor;
    Mapping_data mapdata;

    // globalpos.x = 0;
    // globalpos.y = 0;
    // globalpos.angle = 0;
    WM->set_globalPosition(globalpos);        // Is this RIGHT??? Shouldn't it be more like get_globalPosition as WorldModel initializes to (0, 0)?

    //odom_diff.x = 0;
    //odom_diff.y = 0;
    //odom_diff.angle = 0;
    //std::vector<Room> map;
    //worldmodel->
    map.clear();

    // exitPoint_corr1.x = 0;
    // exitPoint_corr1.y = 0;
    // exitPoint_corr2.x = 0;
    // exitPoint_corr2.y = 0;


    // corridorExit.exitPoint1 = exitPoint_corr1;
    // corridorExit.exitPoint2 = exitPoint_corr2;

    // corridor.corners = corridorcorners;
    // corridor.exit = corridorExit;
    corridor.previousRoom  = -1; //DEFINITION: -1 means no room below this room --> Corridor
    corridor.roomID = 0;

    map.push_back(corridor);

    mapdata.map = map;
    mapdata.pico_position = globalpos;

    std::vector<Exit_map> exits;
    std::vector<Point_map> corners;
    WM->set_allDetectedExits(exits);
    WM->set_allDetectedCorners(corners);


}

//Update corners in the total corner vector and the current room vector based on current room
void Mapping::update_corners () {

    int currentRoom = 0;//worldmodel->get_currentRoom();
    double distance_thresh = 0.3;


    Exit exits_local[40]; // Data from worldmodel (written there by detection)
    Corner corners_local[40]; // Data from worldmodel (written there by detection)

    for(int i = 0; i < 40; ++i){
        exits_local[i] = WM->get_localDetection().Exits_total[i];
        corners_local[i] = WM->get_localDetection().Corners_total[i];
    }


    for(int i = 0; i < 40; ++i) { // ROBUSTNESS ISSUES, PAY ATTENTION
         if(corners_local[i].detected){
             Point_map corner_found = local2global(corners_local[i].cornerPoint);
             bool detected_already = false; // by default

             for(int j = 0; j < totalCorners.size(); ++j){
                 Point_map corner_map = totalCorners[j];
                 //Point_map corner_map = map[currentRoom].corners[j];
                 double distance = sqrt(pow(corner_map.x - corner_found.x, 2) + pow(corner_map.y - corner_found.y, 2));
                 if(distance < distance_thresh){
                    detected_already = true;
                 }
             }

             if(detected_already == false){
                map[currentRoom].corners.push_back(corner_found);
                totalCorners.push_back(corner_found);

             }

         } else{
             break;
         }
    }
}


//Update corners in the total corner vector and the current room vector based on current room
void Mapping::update_rooms () {

    int currentRoom = WM->get_currentRoom();
    double distance_thresh = 0.3;

    //Exit exits_local[40]; // Data from worldmodel (written there by detection)
    Exit exits_local[40]; // Data from worldmodel (written there by detection)


    for(int i = 0; i < 40; ++i){
        exits_local[i] = WM->get_localDetection().Exits_total[i];
    }


    for(int i = 0; i < 40; ++i){ // ROBUSTNESS ISSUES, PAY ATTENTION
         if(exits_local[i].detected){
             Exit_map exit_found;
             exit_found.point1 = local2global(exits_local[i].exitPoint1);
             exit_found.point2 = local2global(exits_local[i].exitPoint2);

             Point_map center_exit_found;
             center_exit_found.x = (exit_found.point1.x + exit_found.point2.x) * 0.5;
             center_exit_found.y = (exit_found.point1.y + exit_found.point2.y) * 0.5;



             bool detected_already= false;
             for(int j = 0; j < totalExits.size(); ++j){
                 Exit_map exit_map = totalExits[j];

                 Point_map center_exit_map;
                 center_exit_map.x = (exit_map.point1.x + exit_map.point2.x) * 0.5;
                 center_exit_map.y = (exit_map.point1.y + exit_map.point2.y) * 0.5;


                 //Point_map corner_map = map[currentRoom].corners[j];
                 double distance = sqrt(pow(center_exit_map.x - center_exit_found.x, 2) + pow(center_exit_map.y - center_exit_found.y, 2));
                 if(distance < distance_thresh){
                    detected_already = true;
                 }
             }

             if(detected_already == false){
                //map[currentRoom].corners[map[currentRoom].corners.size()] = corner_found;
                totalExits.push_back(exit_found);
                int nRooms = map.size();
                Room room;
                room.exit = exit_found;
                room.roomID = nRooms + 1;
                room.previousRoom = currentRoom;
                std::vector<Point_map> corners;
                room.corners = corners;
                map.push_back(room);

             }

         }else{
             break;
         }
    }
    // WorldModel::set_globalRooms(map[map.size()]);
}


Point_map Mapping::local2global (Point local) { //Convert local into global coordinate
    int i  = 0;

    Point_map global;

    global.x = global_pos.x + local.dist * sin(local.angle + global_pos.angle);
    global.y = global_pos.y + local.dist * cos(local.angle + global_pos.angle);
}


void Mapping::update_global_pos () {

    //update_global_pos();

    //First thing you check if results are total crap
    Point global_pos;
    global_pos.x = -odom.y;
    global_pos.y = odom.x;
    global_pos.angle(-odom.a);
    WM->set_globalPosition(global_pos);

    //STUFF WILL BE ADDED HERE TO ENSURE UPDATES OF THE LRF DATA

    //global_pos.x = odom_diff.x + global_pos.x;
    //global_pos.y = odom_diff.y + global_pos.y;
    //global_pos.angle = odom_diff.angle + global_pos.angle;

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
