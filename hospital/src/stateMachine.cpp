#include "stateMachine.h"

bool state_machine(struct High_state  &high_st,struct Low_state  &low_st, WorldModel * worldModel){

    // World model should be read for:
    // 1) number of rooms detected, number of rooms entered
    // 2) current location (flag with room or corridor)
    // 3) properties of the current room (exits, corners, including empty fields
    //                                        (e.g. unidentified corners))
    // 4) knowledge about the exit that we have entered from
    // 5) nearest distance to the wall


    bool end_of_program=false;

    //---------------------------------------//
    //                                       //
    //     to be assigned from world model   //
    //                                       //
    //---------------------------------------//

    int numb_rooms_in_corridor = 0;    // total number of rooms detected
    int numb_rooms_explored = worldModel->get_enteredRooms();
    int numb_nexted_exits = worldModel->get_nestedExits();      // number of total exits in rooms - 1*numb_rooms_explored

    Location location = worldModel->get_currentLocation();
    bool object_found = false;      // second phase
    bool near_object = false;


    vector<Room> allRooms = WorldModel::get_globalRooms();
    int current_room_number = WorldModel::get_currentRoom();
    Room current_room = allRooms[current_room_number];

    int numb_corners_detected = 0;
    int numb_exits = 0;

    vector<int> explorationStack = WorldModel::get_explorationStack();
    if (location == IN_ROOM){
        numb_corners_detected = current_room.corners.size();
        numb_exits = explorationStack.size();// assigned from the world model
    }


    bool end_of_corridor = false;       // have PICO reached the end of the corridor once?
    bool at_start = false;              // have we returned back to the initial condition yet?

    if (location == IN_CORRIDOR){
        if (current_room.corners[1].dist < DIST_SETPOINT || current_room.corners[2].dist <DIST_SETPOINT){
            end_of_corridor = true;
        }
        if (WorldModel::get_globalPosition() < DIST_SETPOINT) {
            at_start = true;
        }

    }


    //------------end of assigning variables-------------//




    switch(high_st) {
    case EXPLORE_HOSPITAL:

        std::cout << "EXPLORING THE HOSPITAL " << std::endl;


        if (location == IN_CORRIDOR) {

            std::cout << "In corridor" << "\n";

            switch(low_st){

            case EXPLORE_CORRIDOR:

                if (numb_rooms_in_corridor == 0 || !end_of_corridor){
                    std::cout << "Exploring the corridor " << std::endl;
                } else if (numb_rooms_in_corridor > numb_rooms_explored - numb_nexted_exits) {
                    low_st = GO_TO_NEXT_ROOM;
                    std::cout << "Moving to the next room" << "\n";
                }
                break;

            case GO_TO_NEXT_ROOM:

                if (worldModel->get_destination() < DIST_SETPOINT){
                    low_st = GO_INSIDE_ROOM;
                    std::cout << "Entering a room from corridor" << "\n";
                } else {
                    std::cout << "Moving to the next room" << "\n";
                }
                break;

            case GO_INSIDE_ROOM:
                location = IN_ROOM;
                low_st = EXPLORE_ROOM;
                WorldModel::set_currentLocation(location);
                std::cout << "Exploring the room " << std::endl;
                break;
            }

            // command to go to  first exit
        } else if (location == IN_ROOM) {

            switch(low_st){

            case EXPLORE_ROOM:

                std::cout << "Exploring the room " << std::endl;

                // check if all the corners are detected
                if (numb_corners_detected == 4){

                    // if nested exit detected
                    if (numb_exits > 1) {
                        low_st = GO_TO_NEXT_ROOM;
                        std::cout << "Going to the nested room " << std::endl;
                    }
                    else {
                        low_st = EXIT_TO_PREV_ROOM;
                        std::cout << "Exiting to the previous room " << std::endl;
                    }
                }

                break;

            case GO_INSIDE_ROOM:
                if (current_room.previousRoom == 0) {
                    low_st = GO_TO_NEXT_ROOM;
                    location = IN_CORRIDOR;
                    WorldModel::set_currentLocation(location);
                    std::cout << "Going into the corridor " << std::endl;
                } else {
                    low_st = EXPLORE_ROOM;
                }
                break;

            case GO_TO_NEXT_ROOM:

                if (worldModel->get_destination() < DIST_SETPOINT){
                    low_st = GO_INSIDE_ROOM;
                    std::cout << "Entering a nested room" << "\n";
                } else {
                    std::cout << "Going to nested room " << std::endl;
                }
                break;

            case EXIT_TO_PREV_ROOM:
                if (worldModel->get_destination() < DIST_SETPOINT){
                    low_st = GO_INSIDE_ROOM;
                    std::cout << "Exiting to lower order room" << "\n";
                } else {
                    std::cout << "Leaving the room" << "\n";
                }
                break;
            }
        }


        if (numb_rooms_explored == (numb_nexted_exits + numb_rooms_explored)) {
            std::cout << "Moving to starting point " << std::endl;
            high_st = RETURN_TO_INIT;
            low_st = GO_TO_START;
        }

        break;

    case RETURN_TO_INIT:
        // assuming we are in the corridor
        std::cout << "RETURNING TO INITIAL POSITION " << std::endl;


        switch(low_st) {

        case GO_TO_START:
            if (at_start){
                low_st = PARKING;
                std::cout << "Parking " << std::endl;
            } else {
                std::cout << "Moving to starting point " << std::endl;
            }
            break;

        case PARKING:
            std::cout << "Parked! " << std::endl;
            end_of_program = true;
            break;
        }

        break;


        //not executed currently
    case GO_TO_ROOM:
        std::cout << "FINDING OBJECT PHASE " << std::endl;
        switch(low_st) {

        case EXIT_CORRIDOR:
            if (location == IN_CORRIDOR){
                std::cout << "Exiting corridor " << std::endl;
            } else if (location == IN_ROOM){
                low_st = GO_INSIDE_ROOM;
                std::cout << "Entering the room " << std::endl;
            } else{
                std::cout << "ERROR exiting corridor " << std::endl;
            }
            break;

        case GO_INSIDE_ROOM:
            low_st = GO_TO_NEXT_ROOM;
            std::cout << "Going to next room " << std::endl;
            break;

        case GO_TO_NEXT_ROOM:

            if (object_found){
                low_st = STAND_NEXT_TO_OBJECT;
            } else {
                if (worldModel->get_destination() < DIST_SETPOINT){
                    low_st = GO_INSIDE_ROOM;
                    std::cout << "Entering the room " << std::endl;
                } else {
                    std::cout << "Going to next room " << std::endl;
                }
            }
            break;

        case STAND_NEXT_TO_OBJECT:
            std::cout << "Moving to the object " << std::endl;
            if (near_object){
                end_of_program = true;
                std::cout << "Near the object " << std::endl;
            }
            break;
        }

        break;
    }


    return end_of_program;
}
