#include "auton.hpp"
#include "tasks.hpp"
#include <string>

void tareDrive()
{
    b_left_mtr.tare_position();
    f_left_mtr.tare_position();
    b_right_mtr.tare_position();
    f_right_mtr.tare_position();
}

void autonStraightDrive(int power)
{
    b_left_mtr.move_voltage(power);
    f_left_mtr.move_voltage(power);
    b_right_mtr.move_voltage(power);
    f_right_mtr.move_voltage(power);
}

void autonDriveStop()
{
    b_left_mtr.move_voltage(0);
    f_left_mtr.move_voltage(0);
    b_right_mtr.move_voltage(0);
    f_right_mtr.move_voltage(0);
}

void autonIntakes(int power)
{
    left_intake_mtr.move_voltage(power);
    right_intake_mtr.move_voltage(power);
}

void autonDriveLeftRight(int left, int right)
{
    b_left_mtr.move_voltage(left);
    f_left_mtr.move_voltage(left);
    b_right_mtr.move_voltage(right);
    f_right_mtr.move_voltage(right);
}

void blueUnprotAuton()
{
    deploy(DONT_DEPLOY_ANTI_TIPS);
    pros::delay(1000);
    tareDrive();
    lift_mtr.move_voltage(-4000);

    // Move forward to get the 5 cubes
    autonIntakes(MAX_FORWARD);
    while(b_left_mtr.get_position() < 2900)
        autonStraightDrive(4000);
    autonDriveStop();
    pros::delay(500);

    autonIntakes(0);
    // Backwards towards the goal zone
    while (b_left_mtr.get_position() > 1300)
        autonStraightDrive(-8000);
    // Slow down when it's close to stop more accurately
    while (b_left_mtr.get_position() > 1000)
        autonStraightDrive(-4000);

    // Turn ~135 degrees left
    autonDriveLeftRight(-4000, 4000);
    pros::delay(1200);

    // Forwards into the goal zone (Times out after 2 sec if it's stuck)
    int initPos = b_left_mtr.get_position();
    for (int i = 0; b_left_mtr.get_position() < initPos + 500; i++) {
        autonStraightDrive(8000);
        pros::delay(20);
    }
    autonDriveStop();
    pros::delay(250);

    // Tray up, ~4800 is all the way up
    while (tray_mtr.get_position() < 4800)
        tray_mtr.move_voltage(9000);
    tray_mtr.move_voltage(0);

    // Nudge the stack, pretty slow to be careful
    autonStraightDrive(3000);
    pros::delay(500);

    // Back out time
    autonStraightDrive(-8000);
    pros::delay(500);
    autonDriveStop();
    
    // Tray back down
    for (int i = 0; i <= 150 && tray_mtr.get_position() > TRAY_STOP; i++) {
        tray_mtr.move_voltage(-12000);
        pros::delay(20);
    }
    tray_mtr.move_voltage(0);
    lift_mtr.move_voltage(0);
}

void redUnprotAuton()
{
    deploy(DONT_DEPLOY_ANTI_TIPS);
    pros::delay(1000);
    tareDrive();
    lift_mtr.move_voltage(-4000);

    // Move forward to get the 5 cubes
    autonIntakes(MAX_FORWARD);
    while(b_left_mtr.get_position() < 2900)
        autonStraightDrive(4000);
    autonDriveStop();
    pros::delay(500);

    autonIntakes(0);
    // Backwards towards the goal zone
    while (b_left_mtr.get_position() > 1300)
        autonStraightDrive(-8000);
    // Slow down when it's close to stop more accurately
    while (b_left_mtr.get_position() > 1000)
        autonStraightDrive(-4000);

    // Turn ~135 degrees right
    autonDriveLeftRight(4000, -4000);
    pros::delay(1200);

    // Forwards into the goal zone (Times out after 2 sec if it's stuck)
    int initPos = b_left_mtr.get_position();
    for (int i = 0; b_left_mtr.get_position() < initPos + 500; i++) {
        autonStraightDrive(8000);
        pros::delay(20);
    }
    autonDriveStop();
    pros::delay(250);

    // Tray up, ~4800 is all the way up
    while (tray_mtr.get_position() < 4800)
        tray_mtr.move_voltage(9000);
    tray_mtr.move_voltage(0);

    // Nudge the stack, pretty slow to be careful
    autonStraightDrive(3000);
    pros::delay(500);

    // Back out time
    autonStraightDrive(-8000);
    pros::delay(500);
    autonDriveStop();
    
    // Tray back down
    for (int i = 0; i <= 150 && tray_mtr.get_position() > TRAY_STOP; i++) {
        tray_mtr.move_voltage(-12000);
        pros::delay(20);
    }
    tray_mtr.move_voltage(0);
    lift_mtr.move_voltage(0);
}

void onePointAuton()
{
    deploy(DEPLOY_ANTI_TIPS);
    // Slow backwards the get the cube in
    autonStraightDrive(MAX_BACKWARD/2);
    pros::delay(1800);
    // Slow forwards to make sure robot's not touching
    autonStraightDrive(MAX_FORWARD/2);
    pros::delay(850);
    autonDriveStop();
}

void deploy(bool deployAntiTips)
{
    if (FLIPPY_THING_ON) {
        left_intake_mtr.move_voltage(MAX_BACKWARD);
        right_intake_mtr.move_voltage(MAX_BACKWARD);
        
        // Lift up, tray up until its out of the lift's way then stop
        // lift_mtr.move_voltage(8000);
        // pros::delay(50);
        for (int i = 0; i < 100 && tray_mtr.get_position() < 400; i++) {
            tray_mtr.move_voltage(MAX_FORWARD);
            pros::delay(20);
        }
        for (int i = 0; i < 100 && tray_mtr.get_position() < 1300; i++) {
            tray_mtr.move_voltage(MAX_FORWARD);
            lift_mtr.move_voltage(8000); 
        }
        lift_mtr.move_voltage(0);
        tray_mtr.move_voltage(0);
        pros::delay(100);

        // Lift back down
        for (int i = 0; i < 100 && lift_mtr.get_position() > 10; i++) {
            lift_mtr.move_voltage(MAX_BACKWARD);
            pros::delay(20);
        }
        lift_mtr.move_voltage(0);

        // Tray back down
        for (int i = 0; i < 100 && tray_mtr.get_position() > TRAY_STOP; i++) {
            tray_mtr.move_voltage(MAX_BACKWARD);
            pros::delay(20);
        }
        tray_mtr.move_voltage(0);

        if (deployAntiTips) {
            f_right_mtr.move_voltage(8000);
            b_right_mtr.move_voltage(8000);
            f_left_mtr.move_voltage(8000);
            b_left_mtr.move_voltage(8000);
            pros::delay(200);
            f_right_mtr.move_voltage(0);
            b_right_mtr.move_voltage(0);
            f_left_mtr.move_voltage(0);
            b_left_mtr.move_voltage(0);
            left_intake_mtr.move_voltage(0);
            right_intake_mtr.move_voltage(0);
        }
    } else {
        left_intake_mtr.move_voltage(MAX_BACKWARD);
        right_intake_mtr.move_voltage(MAX_BACKWARD);
        
        // Lift up, tray up until its out of the lift's way then stop
        lift_mtr.move_voltage(12000);
        for (int i = 0; i < 100 && tray_mtr.get_position() < 1300; i++) {
            tray_mtr.move_voltage(MAX_FORWARD);
            pros::delay(20);
        }
        tray_mtr.move_voltage(0);
        pros::delay(100);

        // Lift back down
        for (int i = 0; i < 100 && lift_mtr.get_position() > 10; i++) {
            lift_mtr.move_voltage(MAX_BACKWARD);
            pros::delay(20);
        }
        lift_mtr.move_voltage(0);

        // Tray back down
        for (int i = 0; i < 100 && tray_mtr.get_position() > TRAY_STOP; i++) {
            tray_mtr.move_voltage(MAX_BACKWARD);
            pros::delay(20);
        }
        tray_mtr.move_voltage(0);

        if (deployAntiTips) {
            f_right_mtr.move_voltage(8000);
            b_right_mtr.move_voltage(8000);
            f_left_mtr.move_voltage(8000);
            b_left_mtr.move_voltage(8000);
            pros::delay(200);
            f_right_mtr.move_voltage(0);
            b_right_mtr.move_voltage(0);
            f_left_mtr.move_voltage(0);
            b_left_mtr.move_voltage(0);
            left_intake_mtr.move_voltage(0);
            right_intake_mtr.move_voltage(0);
        }
    }
}

// void autonSwitcherTask(void * a)
// {
//     int i = 0;
//     while (true) {
//         if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
//             mutexControllerScreen.take(TIMEOUT_MAX);
//             controller.clear();
//             pros::delay(50);
//             controller.print(2, 0, "Auton: %s", autons[autonSelection].code);
//             pros::delay(50);
//             i++;
//             mutexControllerScreen.give();
//             // autonSelection = 0;
//             // if (!pros::competition::is_connected())
//             autonSelection = (autonSelection == autons.size() - 1) ? 0 : autonSelection + 1;
//             // else {
//             //     pros::delay(500);
//             //     if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP))
//             //         autonSelection = (autonSelection == 2) ? 0 : autonSelection + 1;
            
//             while (controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {}
//             contScreen.notify();
//         }
//         pros::delay(250);
//     }
// }