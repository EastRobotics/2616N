#pragma once

#include "general.hpp"
#include <array>

typedef enum {
    ONE_POINT = 0,
    BLUE_UNPROT,
    RED_UNPROT
} autonSelections;

static int autonSelection = ONE_POINT;

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