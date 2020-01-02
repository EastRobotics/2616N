#include "main.h"


typedef enum {
  DRIVE_FRONT_RIGHT = 1,
  DRIVE_BACK_RIGHT,
  DRIVE_FRONT_LEFT,
  DRIVE_BACK_LEFT,
  TRAY_ANGLE_ADJUSTOR,
  LIFT_ADJUSTOR,
  INTAKE_RIGHT,
  INTAKE_LEFT  
} motors;

const int DEADZONE_RADIUS = 25; //Circle about the origin
const int ANGLE_TOLERANCE = 5;  //Surrounding the axes +/-



pros::Controller master(pros::E_CONTROLLER_MASTER);

//Drive Motors
pros::Motor f_right_mtr(DRIVE_FRONT_RIGHT);
pros::Motor b_right_mtr(DRIVE_BACK_RIGHT);
pros::Motor f_left_mtr(DRIVE_FRONT_LEFT);
pros::Motor b_left_mtr(DRIVE_BACK_LEFT);

pros::Motor tray_mtr(TRAY_ANGLE_ADJUSTOR);
pros::Motor lift_mtr(LIFT_ADJUSTOR);
pros::Motor right_intake_mtr(INTAKE_RIGHT);
pros::Motor left_intake_mtr(INTAKE_LEFT);

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

    f_right_mtr.set_reversed(true);
    b_right_mtr.set_reversed(true);
    right_intake_mtr.set_reversed(true);

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

void autonomous() {}


/*
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

void joystickDataFixer(int &x, int &y) {
        double r = sqrt(pow(x, 2) + pow(y, 2));
        if (r < DEADZONE_RADIUS)
        {
            x = 0;
            y = 0;
            return;
        }

        r = (r - DEADZONE_RADIUS) * 128 / (128.0 - DEADZONE_RADIUS);

        double theta = (x < 0) * 180 + (180 / M_PI) * atan(y / (x * 1.0));
        if (x == 0)
        {
            theta = 90 + 180 * (y < 0);
            return;
        }
        if (theta < 0)
            theta += 360;

        x = round(r * cos(M_PI / 180 * theta));
        y = round(r * sin(M_PI / 180 * theta));
}

//TODO: Make a struct for the x/y values
void robotDrive() {
    int y = master.get_analog(ANALOG_LEFT_Y);
    int x = master.get_analog(ANALOG_LEFT_X);
    

    joystickDataFixer(x, y);

    f_left_mtr = y + x;
    f_right_mtr = y - x;
    b_left_mtr = y + x;
    b_right_mtr = y - x;
}



void opcontrol() {


	while (true) {
		// robotDrive();
         if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
             right_intake_mtr.move_voltage(12000);
             left_intake_mtr.move_voltage(12000);
         } 
         if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
             right_intake_mtr.move_voltage(-12000);
             left_intake_mtr.move_voltage(-12000);
         }

         if(!master.get_digital(pros::E_CONTROLLER_DIGITAL_R2) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
              right_intake_mtr.move_voltage(0);
             left_intake_mtr.move_voltage(0);            
         }
        // right_intake_mtr.move(127);
        // left_intake_mtr.move(127);
        
        


		pros::delay(20);
	}
}
