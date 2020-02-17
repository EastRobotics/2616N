#include "general.hpp"
#include "controllerScreen.hpp"
#include <fstream>
#include <iostream>
#include "string"

void recordRerun()
{
    if (pros::usd::is_installed()) {
        while (true) {
            if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B) && !pros::competition::is_connected()) {
                std::fstream motorData ("/usd/motorData.txt", std::ios::out | std::ios::trunc);
                while (!controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
                    std::vector<std::string> voltages;
                    for (auto& i: motorCodes)
                        voltages.insert(voltages.end(), std::to_string(i.motor->get_voltage()));

                    std::string buffer;
                    for (auto& i: voltages)
                        buffer.append(i + ' ');

                    motorData << buffer << std::endl;
                    pros::delay(20);
                }
                motorData.close();
            }
            pros::delay(350);
        }
    }
}