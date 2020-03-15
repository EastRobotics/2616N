#pragma once

#include "general.hpp"
#include "betterController.hpp"
#include "controllerScreen.hpp"
#include "auton.hpp"
#include "gui.hpp"

static pros::Task contScreen (contScreenTask, (void *)"", TASK_PRIORITY_DEFAULT - 2, TASK_STACK_DEPTH_DEFAULT, "contScreen");
static pros::Task bController (bContPrintTask, (void*)"", TASK_PRIORITY_DEFAULT - 2, TASK_STACK_DEPTH_DEFAULT, "bController");
static pros::Task autonButton (guiManagerTask, (void *)"", TASK_PRIORITY_DEFAULT - 2, TASK_STACK_DEPTH_DEFAULT, "autonButton");
