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
                std::ofstream motorData ("/usd/motorData.txt", std::ios::out | std::ios::trunc);
                if (motorData.is_open()) {
                    while (!controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
                        std::vector<std::string> voltages;
                        for (auto& i: motorCodes) {
                            std::string buffer;
                            buffer = "00000" + std::to_string(abs(i.motor->get_voltage()));
                            buffer = buffer.substr(buffer.size() - 5, 5);
                            buffer.insert(buffer.begin(), (i.motor->get_voltage() > 0 ? '+' : '-'));
                            voltages.insert(voltages.end(), buffer);
                        }
                        std::string buffer;
                        for (auto& i: voltages)
                            buffer.append(i + ' ');

                        motorData << buffer << std::endl;
                        pros::delay(20);
                    }
                    motorData.close();
                }
            }
            pros::delay(350);
        }
    }
}

void replayRerun()
{
    if (pros::usd::is_installed()) {
        std::ifstream motorData ("/usd/motorData.txt", std::ios::in);
        
    }
}