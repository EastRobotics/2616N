#pragma once

#include "general.hpp"
#include <vector>

typedef struct {
    void (*function)(void);
    std::string code;
} auton_t;

static int autonSelection = 1;

void autonDriveStop(void);
void autonIntakes(int power);
void autonStraightDrive(int power);
void autonDriveLeftRight(int left, int right);
void blueUnprotAuton(void);
void deploy(bool deployAntiTips);
void onePointAuton(void);
void redUnprotAuton(void);
void tareDrive(void);
void blueProtAuton(void);
void redProtAuton(void);
void blueUnprotAutonSLOW(void);
void redUnprotAutonSLOW(void);

static std::vector<auton_t> autons = {
    {onePointAuton, "OnePoint"},
    {blueUnprotAuton, "B-Unprot"},
    {redUnprotAuton, "R-Unprot"},
    {blueProtAuton, "B-Prot"},
    {redProtAuton, "R-Prot"}, 
    {blueUnprotAutonSLOW, "B-UnSLOW"},
    {redUnprotAutonSLOW, "R-UnSLOW"}
};