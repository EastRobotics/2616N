#pragma once

#include "general.hpp"

typedef enum {
    ONE_POINT = 0,
    RED_UNPROT,
    BLUE_UNPROT
} autonSelections;

autonSelections autonSelection = ONE_POINT;

void autonDriveStop(void);
void autonIntakes(int power);
void autonomous(void);
void autonStraightDrive(int power);
void autonDriveLeftRight(int left, int right);
void blueUnprotAuton(void);
void deploy(void);
void onePointAuton(void);
void redUnprotAuton(void);
void tareDrive(void);