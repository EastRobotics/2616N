#include "motorTemps.hpp"
#include "general.hpp"

pros::Mutex mutexControllerScreen;

void overTempWarning()
{
    constexpr int tempLimit = 50;
    std::vector<motorCodeTemp> overTempMotors;
    int temp;
    static bool overTempStatic;
    bool overTemp = false;
    if ((temp = b_left_mtr.get_temperature()) >= tempLimit) {
        if (!overTempStatic)
            controller.rumble("...");
        overTempMotors.insert(overTempMotors.end(), {"BL", (int)b_left_mtr.get_temperature()});
        overTempStatic = true;
        overTemp = true;
    }
    if ((temp = f_left_mtr.get_temperature()) >= tempLimit) {
        if (!overTempStatic)
            controller.rumble("...");
        overTempMotors.insert(overTempMotors.end(), {"FL", (int)f_left_mtr.get_temperature()});
        overTempStatic = true;
        overTemp = true;
    }
    if ((temp = b_right_mtr.get_temperature()) >= tempLimit) {
        if (!overTempStatic)
            controller.rumble("...");
        overTempMotors.insert(overTempMotors.end(), {"BR", (int)b_right_mtr.get_temperature()});
        overTempStatic = true;
        overTemp = true;
    }
    if ((temp = f_right_mtr.get_temperature()) >= tempLimit) {
        if (!overTempStatic)
            controller.rumble("...");
        overTempMotors.insert(overTempMotors.end(), {"FR", (int)f_right_mtr.get_temperature()});
        overTempStatic = true;
        overTemp = true;
    }
    if ((temp = tray_mtr.get_temperature()) >= tempLimit) {
        if (!overTempStatic)
            controller.rumble("...");
        overTempMotors.insert(overTempMotors.end(), {"TR", (int)tray_mtr.get_temperature()});
        overTempStatic = true;
        overTemp = true;
    }
    if ((temp = lift_mtr.get_temperature()) >= tempLimit) {
        if (!overTempStatic)
            controller.rumble("...");
        overTempMotors.insert(overTempMotors.end(), {"LT", (int)lift_mtr.get_temperature()});
        overTempStatic = true;
        overTemp = true;
    }
    if ((temp = left_intake_mtr.get_temperature()) >= tempLimit) {
        if (!overTempStatic)
            controller.rumble("...");
        overTempMotors.insert(overTempMotors.end(), {"LI", (int)left_intake_mtr.get_temperature()});
        overTempStatic = true;
        overTemp = true;
    }
    if ((temp = right_intake_mtr.get_temperature()) >= tempLimit) {
        if (!overTempStatic)
            controller.rumble("...");
        overTempMotors.insert(overTempMotors.end(), {"RI", (int)right_intake_mtr.get_temperature()});
        overTempStatic = true;
        overTemp = true;
    }
    overTempStatic = overTemp;

    mutexControllerScreen.take(TIMEOUT_MAX);
    if (overTemp){
        controller.clear();
        pros::delay(75);
        std::vector<std::string> buffer;
        for (auto& i : overTempMotors)
            buffer.insert(buffer.end(), i.motorCode + " - " + std::to_string(i.temp));

        if (buffer.size() % 2 == 0 && buffer.size() <= 6) {
            for (int i = 0; i < buffer.size(); i += 2) {
                std::string str = buffer[i] + ' ' + buffer[i+1];
                controller.set_text(i / 2, 0, str.c_str());
                pros::delay(75);
            }
        } else if (buffer.size() <= 5) {
            for (int i = 0; i < buffer.size() - 1; i += 2) {
                std::string str = buffer[i] + ' ' + buffer[i+1];
                controller.set_text(i / 2, 0, str.c_str());
                pros::delay(75);
            }
            controller.set_text(2, 0, buffer.back().c_str());
            pros::delay(75);
        } else
            controller.set_text(1, 0, "ALL MOTORS DEAD");
            pros::delay(75);
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
    char blank[] = "                 ";
    bool released;
    while (true) {
        count = 0;
        released = false;
        while (controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
            count++;
            mutexControllerScreen.take(1000);
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
            mutexControllerScreen.give();
            pros::delay(500);
            released = true;
        }
        mutexControllerScreen.take(1000);
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
        controller.set_text(2, 0, blank);
        mutexControllerScreen.give();
        pros::delay(350);
    }
}