#pragma once

#include "general.hpp"
#include "controllerScreen.hpp"
#include "auton.hpp"
#include "gui.hpp"
#include "rerunAuton.hpp"

static pros::Task contScreen (contScreenTask, (void *)"", TASK_PRIORITY_DEFAULT - 2, TASK_STACK_DEPTH_DEFAULT, "contScreen");
static pros::Task autonButton (guiManagerTask, (void *)"", TASK_PRIORITY_DEFAULT - 2, TASK_STACK_DEPTH_DEFAULT, "autonButton");

static pros::Task rerunRecord (recordRerun, (void *)"", TASK_PRIORITY_DEFAULT + 1, TASK_STACK_DEPTH_DEFAULT, "rerunRecord");