#pragma once
#include "general.hpp"
#include "auton.hpp"

void autonomous(void);
void autonSelectorInit(void);
lv_res_t buttonPressed(lv_obj_t * obj);
void autonSelectorReset(void);