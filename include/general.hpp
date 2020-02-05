#pragma once
#ifndef GENERAL_HPP
#define GENERAL_HPP

void deploy(void);
void lift(void);
void intakes(int voltage);
void tray(void * a);

typedef enum {
  DRIVE_FRONT_RIGHT = 11,
  DRIVE_BACK_RIGHT=2,
  DRIVE_FRONT_LEFT=20,
  DRIVE_BACK_LEFT=8,
  TRAY_ANGLE_ADJUSTOR=10,
  LIFT_ADJUSTOR=9,
  INTAKE_RIGHT=1,
  INTAKE_LEFT= 7
} motors;



constexpr int MAX_FORWARD = 12000;
constexpr int MAX_BACKWARD = -12000;
constexpr int TRAY_STOP = 100;

#ifndef M_PI
constexpr double M_PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679;
#endif

static int motorSlowdown = 1;

static pros::Controller controller(pros::E_CONTROLLER_MASTER);

static pros::Motor f_right_mtr(DRIVE_FRONT_RIGHT, pros::E_MOTOR_GEARSET_18, true);
static pros::Motor b_right_mtr(DRIVE_BACK_RIGHT, pros::E_MOTOR_GEARSET_18, true);
static pros::Motor f_left_mtr(DRIVE_FRONT_LEFT, pros::E_MOTOR_GEARSET_18, false);
static pros::Motor b_left_mtr(DRIVE_BACK_LEFT, pros::E_MOTOR_GEARSET_18, false);

static pros::Motor tray_mtr(TRAY_ANGLE_ADJUSTOR, pros::E_MOTOR_GEARSET_18, true);
static pros::Motor lift_mtr(LIFT_ADJUSTOR, pros::E_MOTOR_GEARSET_36, true);

static pros::Motor right_intake_mtr(INTAKE_RIGHT, pros::E_MOTOR_GEARSET_18, true);
static pros::Motor left_intake_mtr(INTAKE_LEFT, pros::E_MOTOR_GEARSET_18, false);

#endif