#include "motorTemps.hpp"
#include "general.hpp"

pros::Mutex mutexControllerScreen;
char blank[] = "                 ";
constexpr int tempLimit = 55;
const std::vector<motorCode> motorCodes = 
        {
            {&b_left_mtr, "BL"}, {&f_left_mtr, "FL"},
            {&b_right_mtr, "BR"}, {&f_right_mtr, "FR"},
            {&lift_mtr, "LT"}, {&tray_mtr, "TR"},
            {&left_intake_mtr, "LI"}, {&right_intake_mtr, "RI"}
        };

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
        controller.set_text(0, 0, blank);
        pros::delay(50);
        controller.set_text(1, 0, blank);
        pros::delay(50);
        controller.set_text(2, 0, blank);
        pros::delay(50);
        std::vector<std::string> buffer;
        for (auto& i : overTempMotors)
            buffer.insert(buffer.end(), i.code + " - " + std::to_string(i.motor->get_temperature()));

        if (buffer.size() % 2 == 0 && buffer.size() <= 6) {
            for (int i = 0; i < buffer.size(); i += 2) {
                std::string str = buffer[i] + ' ' + buffer[i+1];
                controller.set_text(i / 2, 0, str.c_str());
                pros::delay(50);
            }
        } else if (buffer.size() <= 5) {
            for (int i = 0; i < buffer.size() - 1; i += 2) {
                std::string str = buffer[i] + ' ' + buffer[i+1];
                controller.set_text(i / 2, 0, str.c_str());
                pros::delay(50);
            }
            controller.set_text((buffer.size()-1) / 2, 0, buffer.back().c_str());
            pros::delay(50);
        } else
            controller.set_text(1, 0, "ALL MOTORS DEAD");
            pros::delay(50);
    }
    mutexControllerScreen.give();
}

void OTWarning_task(void * a) {
    while (true) {
        overTempWarning();
        pros::delay(500);
    }
}

void showTemps(void * a) {
    int count;
    bool released;
    while (true) {
        count = 0;
        released = false;
        mutexControllerScreen.take(TIMEOUT_MAX);
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
            controller.print(2, 0, "LI - %d RT - %d",
                    (int)left_intake_mtr.get_temperature(), (int)right_intake_mtr.get_temperature());
                pros::delay(75);
            pros::delay(500);
            released = true;
        }
        if (released){
            controller.set_text(0, 0, blank);
            pros::delay(50);
            controller.set_text(1, 0, blank);
            pros::delay(50);
            controller.set_text(2, 0, blank);
            pros::delay(50);
        }
        controller.print(0, 0, "Battery - %d%%", (int)pros::battery::get_capacity());
        pros::delay(50);
        controller.print(1, 0, "Cont Bat - %d%%", controller.get_battery_capacity());
        pros::delay(50);
        controller.set_text(2, 3, "Hi Thomas!!");
        mutexControllerScreen.give();
        pros::delay(350);
    }
}