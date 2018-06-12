#ifndef MONITORS_HPP_INCLUDED
#define MONITORS_HPP_INCLUDED

#include "stateMachine.h"
#include "worldModel.h"
#include "mapping.h"


// unite the program execution
bool monitoring(WorldModel *worldModel);
bool explore_hospital(WorldModel *worldModel);
bool return_to_init(WorldModel * worldModel);
bool search_for_object(WorldModel * worldModel);

#endif
