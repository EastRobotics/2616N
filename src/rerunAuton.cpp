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
                        for (auto& i: motorCodes) {
                            char buffer[7];
                            // Format: ±XXXXX, ie. 732 -> +00732, -3321 -> -03321
                            sprintf(buffer, "%+06d", i.motor->get_voltage());
                            motorData << buffer << ' ';
                        }
                        motorData << std::endl;
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
    constexpr int ONE_LINE_LENGTH = 56;
    constexpr int ONE_VOLTAGE_LENGTH = 7;
    constexpr int ONE_NUM_LENGTH = 5;
    if (pros::usd::is_installed()) {
        std::ifstream motorData ("/usd/motorData.txt", std::ios::in);
        if (motorData.is_open()) {
            // Finding the length of the file in chars
            motorData.seekg(0, motorData.end);
            int fileLength = motorData.tellg();
            motorData.seekg(0, motorData.beg);

            // Dumping the file into a string
            // Complicated bc operator<< doesn't work w/ std::string for some reason
            char* motorDataCStr = new char [fileLength + 1];
            motorData.read(motorDataCStr, fileLength);
            std::string motorDataString (motorDataCStr);
            motorData.close();
            delete[] motorDataCStr;

            for(int i = 0; i < motorDataString.length(); i += ONE_LINE_LENGTH) {
                std::string currentLine = motorDataString.substr(i, ONE_LINE_LENGTH);
                for (int j = 0; j < ONE_LINE_LENGTH; j += ONE_VOLTAGE_LENGTH) {
                    motorCodes[j].motor->move_voltage(std::stoi(currentLine.substr(j+1, ONE_NUM_LENGTH)));
                }
            pros::delay(20);
            }
        }
    }
}