#pragma once
#include "general.hpp"
#include "controllerScreen.hpp"
#include "auton.hpp"
#include <fstream>
#include <iostream>
#include <string>

typedef struct {
    std::ofstream file;
    int32_t (*functionInt)(uint8_t);
    double (*functionDouble)(uint8_t);
    int multiplier;
} loggingStruct;

constexpr std::array<uint8_t, 8> motorPortArray
        ({DRIVE_BACK_LEFT, DRIVE_FRONT_LEFT, DRIVE_BACK_LEFT, DRIVE_FRONT_RIGHT,
          LIFT_ADJUSTOR, TRAY_ANGLE_ADJUSTOR, INTAKE_LEFT, INTAKE_RIGHT});

constexpr std::array<pros::controller_analog_e_t, 4> joystickAxesArray
        ({ANALOG_LEFT_X, ANALOG_LEFT_Y, ANALOG_RIGHT_X, ANALOG_RIGHT_Y});

constexpr std::array<pros::controller_digital_e_t, 12> controllerButtonsArray
        ({DIGITAL_UP, DIGITAL_RIGHT, DIGITAL_DOWN, DIGITAL_LEFT,
         DIGITAL_X, DIGITAL_A, DIGITAL_B, DIGITAL_Y,
         DIGITAL_L1, DIGITAL_L2, DIGITAL_R1, DIGITAL_R2});

void recordRerun(void* a);
void replayRerun();