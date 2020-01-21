#include "main.h"

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

typedef enum {
  SINGLE_STICK_ARCADE = 0,
  DOUBLE_STICK_ARCADE, 
  DOUBLE_STICK_TANK
} driveMode;

const int DEADZONE_RADIUS = 25; //Circle about the origin
const int ANGLE_TOLERANCE = 5;  //Surrounding the axes +/-
const int DRIVE_MODE = SINGLE_STICK_ARCADE; //:)

pros::Controller master(pros::E_CONTROLLER_MASTER);


pros::Motor f_right_mtr(DRIVE_FRONT_RIGHT, pros::E_MOTOR_GEARSET_18, true);
pros::Motor b_right_mtr(DRIVE_BACK_RIGHT, pros::E_MOTOR_GEARSET_18, true);
pros::Motor f_left_mtr(DRIVE_FRONT_LEFT, pros::E_MOTOR_GEARSET_18, false);
pros::Motor b_left_mtr(DRIVE_BACK_LEFT, pros::E_MOTOR_GEARSET_18, false);

pros::Motor tray_mtr(TRAY_ANGLE_ADJUSTOR, pros::E_MOTOR_GEARSET_36, true);
pros::Motor lift_mtr(LIFT_ADJUSTOR, pros::E_MOTOR_GEARSET_36, true);
pros::Motor right_intake_mtr(INTAKE_RIGHT, pros::E_MOTOR_GEARSET_18, true);
pros::Motor left_intake_mtr(INTAKE_LEFT, pros::E_MOTOR_GEARSET_18, false);

int motorSlowdown=1;

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
	// pros::lcd::set_text(1, "Hello PROS User!");

    right_intake_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    left_intake_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    lift_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    tray_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

    // trayTare();
    // liftTare();
    // tray_mtr.tare_position();
	// pros::lcd::register_btn1_cb(on_center_button);
    
    //Deployment

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
void competition_initialize() {
}


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

void autonomous () {
    deploy();
    trayTare();
    liftTare();
    b_left_mtr.move_voltage(12000);
    f_left_mtr.move_voltage(12000);
    b_right_mtr.move_voltage(12000);
    f_right_mtr.move_voltage(12000);
    pros::delay(500);
    b_left_mtr.move_voltage(-12000);
    f_left_mtr.move_voltage(-12000);
    b_right_mtr.move_voltage(-12000);
    f_right_mtr.move_voltage(-12000);
    pros::delay(500);
    b_left_mtr.move_voltage(0);
    f_left_mtr.move_voltage(0);
    b_right_mtr.move_voltage(0);
    f_right_mtr.move_voltage(0);

    // autonDrive(0,127);
    // right_intake_mtr.move_voltage(12000);
    // left_intake_mtr.move_voltage(12000);
    // pros::delay(5000);
    // right_intake_mtr.move_voltage(0);
    // left_intake_mtr.move_voltage(0);
}


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

void liftTare()
{
    int slowSpeed = -6000;
    lift_mtr.tare_position();
    double pos = lift_mtr.get_position();
    do {
        pos = lift_mtr.get_position();
        lift_mtr.move_voltage(slowSpeed);
        pros::delay(20);
    } while (pos != lift_mtr.get_position());
    lift_mtr.move_voltage(0);
    lift_mtr.tare_position();
}

void trayTare()
{
    int slowSpeed = 6000;


    do {    
        // pos = tray_mtr.get_position();
        tray_mtr.move_velocity(slowSpeed);
        pros::delay(20);
        tray_mtr.move_velocity(0);
        pros::delay(20);
    // } while (pos != tray_mtr.get_position())
    } while (tray_mtr.get_actual_velocity());
    tray_mtr.tare_position();

}

void deploy()
{
    tray_mtr.move_voltage(-1200);
    lift_mtr.move_voltage(-1200);
    right_intake_mtr.move_voltage(-12000);
    left_intake_mtr.move_voltage(-12000);
    f_right_mtr.move_voltage(12000);
    b_right_mtr.move_voltage(12000);
    f_left_mtr.move_voltage(12000);
    b_left_mtr.move_voltage(12000);
    pros::delay(200);
    f_right_mtr.move_voltage(0);
    b_right_mtr.move_voltage(0);
    f_left_mtr.move_voltage(0);
    b_left_mtr.move_voltage(0);
    pros::delay(1000);
    tray_mtr.move_voltage(0);
    lift_mtr.move_voltage(0);
    right_intake_mtr.move_voltage(0);
    left_intake_mtr.move_voltage(0);
}

void robotDrive() {
    int y = 0;
    int x = 0;
    int left = 0;
    int right = 0;
    switch(DRIVE_MODE) {
        case SINGLE_STICK_ARCADE:
            y = master.get_analog(ANALOG_LEFT_Y)/motorSlowdown;
            x = master.get_analog(ANALOG_LEFT_X)/motorSlowdown;
            joystickDataFixer(x, y);

            f_left_mtr = y + x;
            f_right_mtr = y - x;
            b_left_mtr = y + x;
            b_right_mtr = y - x;
            break;
        case DOUBLE_STICK_ARCADE:
            y = master.get_analog(ANALOG_LEFT_Y)/motorSlowdown;
            x = master.get_analog(ANALOG_RIGHT_X)/motorSlowdown;

            f_left_mtr = y + x;
            f_right_mtr = y - x;
            b_left_mtr = y + x;
            b_right_mtr = y - x;
            break;
        case DOUBLE_STICK_TANK:
            left = master.get_analog(ANALOG_LEFT_Y)/motorSlowdown;
            right = master.get_analog(ANALOG_RIGHT_Y)/motorSlowdown;

            f_left_mtr = left;
            f_right_mtr = right;
            b_left_mtr = left;
            b_right_mtr = right;
            break;
    }
}


void liftController()
{
    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        lift_mtr.move_voltage(12000/motorSlowdown);
        if (tray_mtr.get_position() < 1050) {
            tray_mtr.move_voltage(12000/motorSlowdown);
        }
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        lift_mtr.move_voltage(-12000/motorSlowdown);
        if(lift_mtr.get_position() < 1000 && tray_mtr.get_position() > 0) {
            tray_mtr.move_voltage(-12000/motorSlowdown);
        }
    } else {
        lift_mtr.move_voltage(0);
    }

}

void intake() {
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        right_intake_mtr.move_voltage(12000/motorSlowdown);
        left_intake_mtr.move_voltage(12000/motorSlowdown);
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        right_intake_mtr.move_voltage(-12000/motorSlowdown);
        left_intake_mtr.move_voltage(-12000/motorSlowdown);
    } else if(!master.get_digital(pros::E_CONTROLLER_DIGITAL_R2) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        right_intake_mtr.move_voltage(0);
        left_intake_mtr.move_voltage(0);
    }
}

void tray() {
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
        tray_mtr.move_voltage(12000/motorSlowdown);
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
        tray_mtr.move_voltage(-12000/motorSlowdown);
    }
    
     else {
        tray_mtr.move_voltage(0);
    }
}

void precisionMode() {
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) { //Slow down
        motorSlowdown = 2;
    } else if (!master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) { //Reset to default, no slowdown
        motorSlowdown = 1;
    }
}

void opcontrol() {
	while (true) {
    	robotDrive();
        intake();
        tray();
        liftController();
        precisionMode();
        
        auto str = std::to_string(lift_mtr.get_position());
        pros::lcd::set_text(2, "Lift motor: "+ str);
        str = std::to_string(tray_mtr.get_position());
        pros::lcd::set_text(3, "Tray motor: "+str);

        //Panic button/debug, should never have to be used during a drive period
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))
            deploy();

        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
            tray_mtr.tare_position();
            lift_mtr.tare_position();
        }

		pros::delay(20);
	}
}
