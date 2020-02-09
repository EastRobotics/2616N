#include "main.h"
#include "controllerScreen.hpp"
#include "drive.hpp"
#include "auton.hpp"
#include "tasks.hpp"
#include <array>

// #define NEW_TRAY_RETURN

void initialize()
{
    pros::lcd::initialize();

    right_intake_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    left_intake_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    lift_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    tray_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
}

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

bool liftInUse = false;
void lift()
{
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        liftInUse = true;
        lift_mtr.move_voltage(MAX_FORWARD/motorSlowdown);
        if (tray_mtr.get_position() < 1300) {
            tray_mtr.move_voltage(MAX_FORWARD/motorSlowdown);
        }
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        liftInUse = true;
        if(lift_mtr.get_position() < 1300 && tray_mtr.get_position() > TRAY_STOP) {
            tray_mtr.move_voltage(-8000/motorSlowdown);
        }
        lift_mtr.move_voltage(MAX_BACKWARD);
    } else {
        liftInUse = false;
        lift_mtr.move_voltage(0);
    }
}


void tray(void * a)
{
    while (true) {
        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
            if (tray_mtr.get_position() >= TRAY_STOP)
                tray_mtr.move_voltage(int(MAX_FORWARD*abs(cos(M_PI*tray_mtr.get_position()/12000)))/motorSlowdown);
            else
                tray_mtr.move_voltage(MAX_FORWARD);

        #ifdef NEW_TRAY_RETURN
        } else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
            
            // Lift out of the way
            while (lift_mtr.get_position() < 200 && !liftInUse) {
                lift_mtr.move_voltage(6000/motorSlowdown);
                pros::delay(20);
            }
            lift_mtr.move_voltage(0);

            // Quickly close to down
            while (tray_mtr.get_position() > TRAY_STOP + 1200) {
                tray_mtr.move_voltage(MAX_BACKWARD/motorSlowdown);
                pros::delay(20);
            }
            tray_mtr.move_voltage(-4000/motorSlowdown);

            // Lift back down
            while (lift_mtr.get_position() > 10 && !liftInUse) {
                lift_mtr.move_voltage(-6000/motorSlowdown);
                pros::delay(20);
            }
            lift_mtr.move_voltage(0);

            // Slowly the rest of the way
            while (tray_mtr.get_position() > TRAY_STOP + 400) {
                // tray_mtr.move_voltage(-4000/motorSlowdown);
                pros::delay(20);
            }
            tray_mtr.move_voltage(0);
        #else
        } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_B) && tray_mtr.get_position() > TRAY_STOP + 75) {
            tray_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
            tray_mtr.move_voltage(MAX_BACKWARD/motorSlowdown);
        #endif

        } else {
            tray_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
            tray_mtr.move_voltage(0);
        }
        pros::delay(30);
    }
}

void intakes()
{
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        right_intake_mtr.move_voltage(MAX_FORWARD/motorSlowdown);
        left_intake_mtr.move_voltage(MAX_FORWARD/motorSlowdown);   
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        right_intake_mtr.move_voltage(MAX_BACKWARD/motorSlowdown);
        left_intake_mtr.move_voltage(MAX_BACKWARD/motorSlowdown);
    } else {
        right_intake_mtr.move_voltage(0);
        left_intake_mtr.move_voltage(0);
    }
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

void opcontrol() {
    b_left_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    f_left_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    b_right_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    f_right_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    
    tray_mtr.move_voltage(3000);
    pros::delay(120);
    tray_mtr.move_voltage(0);

    pros::Task trayTask (tray, (void *)"", TASK_PRIORITY_DEFAULT + 2, TASK_STACK_DEPTH_DEFAULT, "trayTask");

	while (true) {
    	drive();
        intakes();
        lift();
        
        motorSlowdown = controller.get_digital(pros::E_CONTROLLER_DIGITAL_A) ? 2 : 1;

        std::string str = std::to_string(lift_mtr.get_position());
        pros::lcd::set_text(2, "Lift motor: "+ str);
        str = std::to_string(tray_mtr.get_position());
        pros::lcd::set_text(3, "Tray motor: "+str);

        //Deployment
        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))
            deploy();

        //Vibration at certain test values
        if (tray_mtr.get_position() <= -50) {
            controller.rumble("-");
        }

        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN) && !pros::competition::is_connected())
            autonomous();

        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT))
            tray_mtr.tare_position();

        pros::delay(20);

	}
}