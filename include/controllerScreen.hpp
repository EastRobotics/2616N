#pragma once

#include <array>
#include <vector>
#include <string>
#include "general.hpp"
#include "auton.hpp"

typedef struct motorCode {
    pros::Motor * motor;
    std::string code;
} motorCode;

static pros::Mutex mutexControllerScreen;
constexpr int tempLimit = 55;
const std::vector<motorCode> motorCodes = 
        {   {&b_left_mtr, "BL"},      {&f_left_mtr, "FL"},
            {&b_right_mtr, "BR"},     {&f_right_mtr, "FR"},
            {&lift_mtr, "LT"},        {&tray_mtr, "TR"},
            {&left_intake_mtr, "LI"}, {&right_intake_mtr, "RI"}
        };

void overTempWarning(void);
void OTWarning_task(void * a);
void showTemps(void);
void contScreenTask(void * a);
void passiveScreen(void);