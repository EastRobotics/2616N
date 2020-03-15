#pragma once

#include "general.hpp"
#include "auton.hpp"

void contScreenTask(void * a);

typedef struct motorCode {
    pros::Motor * motor;
    std::string code;
} motorCode;

const std::vector<motorCode> motorCodes = 
        {   {&b_left_mtr, "BL"},      {&f_left_mtr, "FL"},
            {&b_right_mtr, "BR"},     {&f_right_mtr, "FR"},
            {&lift_mtr, "LT"},        {&tray_mtr, "TR"},
            {&left_intake_mtr, "LI"}, {&right_intake_mtr, "RI"}
        };