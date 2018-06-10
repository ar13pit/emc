#ifndef SM_HPP_INCLUDED
#define SM_HPP_INCLUDED

#include "main.hpp"
#include "worldModel.h"
#include "config.h"
#include "detection.h"

bool state_machine(struct High_state high_st,struct Low_state low_st, WorldModel * worldModel);

#endif
