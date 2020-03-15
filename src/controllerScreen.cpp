#include "controllerScreen.hpp"
#include "tasks.hpp"

void contScreenTask(void * a)
{
    int count;
    bool released;
    while (true) {
        count = 0;
        released = false;
        controller.clear();
        pros::delay(50);
        while (controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
            count++;
            if (count % 4 <= 1) {
                controller.print(0, 0, "BL - %d FL - %d", 
                    (int)b_left_mtr.get_temperature(), (int)f_left_mtr.get_temperature());
                pros::delay(75);
            } else {
                controller.print(0, 0, "BR - %d FR - %d",
                    (int)b_right_mtr.get_temperature(), (int)f_right_mtr.get_temperature());
                pros::delay(75);
            }
            controller.print(1, 0, "TR - %d LT - %d",
                    (int)tray_mtr.get_temperature(), (int)lift_mtr.get_temperature());
            pros::delay(75);
            controller.print(2, 0, "LI - %d RI - %d",
                    (int)left_intake_mtr.get_temperature(), (int)right_intake_mtr.get_temperature());
            pros::delay(500);
            released = true;
        }
        if (released){
            controller.clear();
            pros::delay(50);
        }
        controller.print(0, 0, "Battery - %d%%", (int)pros::battery::get_capacity());
        pros::delay(50);
        controller.print(0, 0, "Cont Bat - %d%%", controller.get_battery_capacity());
        pros::delay(50);
        controller.print(0, 0, "Auton: %s", autons[autonSelection].code);
        pros::delay(350);
    }
}