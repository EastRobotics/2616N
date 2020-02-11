#pragma once

#include "general.hpp"
#include <vector>

typedef struct {
    void (*function)(void);
    std::string code;
} auton_t;

static int autonSelection = 0;

void autonDriveStop(void);
void autonIntakes(int power);
void autonomous(void);
void autonStraightDrive(int power);
void autonDriveLeftRight(int left, int right);
void blueUnprotAuton(void);
void deploy(bool deployAntiTips);
void onePointAuton(void);
void redUnprotAuton(void);
void tareDrive(void);
void autonSwitcherTask(void * a);

static std::vector<auton_t> autons = {
    {onePointAuton, "OnePoint"},
    {blueUnprotAuton, "B-Unprot"},
    {redUnprotAuton, "R-Unprot"}
};