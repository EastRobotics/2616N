#include "controllerScreen.hpp"
#include "tasks.hpp"

// NOTE: Not currently in use, it causes big big issues for some reason
void overTempWarning()
{
    std::vector<motorCode> overTempMotors;
    bool overTemp = false;
    static bool overTempStatic;

    for (auto& i: motorCodes) {
        if (i.motor->get_temperature() >= tempLimit) {
            if (!overTempStatic)
                controller.rumble("...");
            overTempMotors.insert(overTempMotors.end(), i);
            overTempStatic = true;
            overTemp = true;
        }
    }
    overTempStatic = overTemp;

    mutexControllerScreen.take(TIMEOUT_MAX);
    if (overTemp){
        controller.clear();
        pros::delay(50);
        std::vector<std::string> buffer;
        for (auto& i : overTempMotors)
            buffer.insert(buffer.end(), i.code + " - " + std::to_string(i.motor->get_temperature()));

        if (buffer.size() <= 6) {
            if (buffer.size() % 2 == 0) {
                for (int i = 0; i < buffer.size(); i += 2) {
                    std::string str = buffer[i] + ' ' + buffer[i+1];
                    controller.set_text(i / 2, 0, str.c_str());
                    pros::delay(50);
                }
            } else {
                for (int i = 0; i < buffer.size() - 1; i += 2) {
                    std::string str = buffer[i] + ' ' + buffer[i+1];
                    controller.set_text(i / 2, 0, str.c_str());
                    pros::delay(50);
                }
                controller.set_text(((buffer.size() - 1) / 2), 0, buffer[4].c_str());
                pros::delay(50);
            }
        } else {
            for (int i = 0; i < 6; i += 2) {
                std::string str = buffer[i] + ' ' + buffer[i+1];
                controller.set_text(i / 2, 0, str.c_str());
                pros::delay(50);
            }
        }
    }
    mutexControllerScreen.give();
}

void OTWarning_task(void * a)
{
    while (true) {
        overTempWarning();
        pros::delay(500);
    }
}

void showTemps()
{
    int count = 0;
    bool released = false;
    mutexControllerScreen.take(TIMEOUT_MAX);
    while (controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
        count++;
        if (count % 4 <= 1) {
            controller.print(0, 0, "BL - %d FL - %d", 
                (int)b_left_mtr.get_temperature(), (int)f_left_mtr.get_temperature());
            pros::delay(50);
        } else {
            controller.print(0, 0, "BR - %d FR - %d",
                (int)b_right_mtr.get_temperature(), (int)f_right_mtr.get_temperature());
            pros::delay(50);
        }
        controller.print(1, 0, "TR - %d LT - %d",
                (int)tray_mtr.get_temperature(), (int)lift_mtr.get_temperature());
        pros::delay(50);
        controller.print(2, 0, "LI - %d RI - %d",
                (int)left_intake_mtr.get_temperature(), (int)right_intake_mtr.get_temperature());
        pros::delay(500);
    }
    controller.clear();
    pros::delay(50);
    mutexControllerScreen.give();
    passiveScreen()
}

void contScreenTask(void * a)
{
    int lastBattCapacity = (int)pros::battery::get_capacity();
    int lastContBatt = controller.get_battery_capacity();
    int lastAuton = autonSelection;
    passiveScreen();
    while (true) {

        if (contScreen.notify_take(true, 100) ||
            (int)pros::battery::get_capacity() != lastBattCapacity ||
            controller.get_battery_capacity() != lastContBatt) {
            passiveScreen();
        }
        showTemps();
        controller.clear();

        lastBattCapacity = (int)pros::battery::get_capacity();
        lastContBatt = controller.get_battery_capacity();
        lastAuton = autonSelection;
        pros::delay(150);
    }
}

void passiveScreen()
{
    mutexControllerScreen.take(TIMEOUT_MAX);
    pros::delay(50);
    controller.clear();
    pros::delay(50);
    controller.print(0, 0, "Battery - %d%%", (int)pros::battery::get_capacity());
    pros::delay(50);
    controller.print(1, 0, "Cont Bat - %d%%", controller.get_battery_capacity());
    pros::delay(50);
    controller.print(2, 0, "Auton: %s", autons[autonSelection].code);
    pros::delay(50);
    mutexControllerScreen.give();
}