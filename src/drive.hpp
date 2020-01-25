#pragma once

#include "main.h"

typedef enum {
  SINGLE_STICK_ARCADE = 0,
  DOUBLE_STICK_ARCADE, 
  DOUBLE_STICK_TANK
} driveMode;

constexpr int DRIVE_MODE = SINGLE_STICK_ARCADE;

void joystickDataFixer(int &x, int &y);
void drive(void);