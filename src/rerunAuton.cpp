#include "general.hpp"
#include "controllerScreen.hpp"
#include <fstream>
#include <iostream>
#include "string"

typedef enum {
    BLEFT_MTR_INDEX = 0,
    FLEFT_MTR_INDEX,
    BRIGHT_MTR_INDEX,
    BLEFT_MTR_INDEX,
    LIFT_MTR_INDEX,
    TRAY_MTR_INDEX,
    L_INTAKE_MTR_INDEX,
    R_INTAKE_MTR_INDEX
} allMotorIndexEnum;

typedef struct {
    int sign;
    int voltage;
} rerunVoltageFmt;

typedef std::vector<int> allMotorVoltageFmt;

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
        if (motorData.is_open()) {
            std::vector<allMotorVoltageFmt> allMotorVoltages;
            std::string motorDataString;
            motorData >> motorDataString;
            for (int i = 0; i < motorData.gcount(); i += 56) {
                std::string currentLine = motorDataString.substr(i, 56);
                std::vector<rerunVoltageFmt> currentLineVoltages;
                allMotorVoltageFmt allLineMotorVoltages;
                for (int j = 0; j < 34; j += 7) {
                    std::string vltStr = currentLine.substr(j, 6);
                    rerunVoltageFmt vlt;
                    vlt.sign = vltStr[0] == '+' ? 1 : -1;
                    vlt.voltage = std::stoi(vltStr.substr(1, 5));
                    currentLineVoltages.insert(currentLineVoltages.end(), vlt);
                }
                for (auto& i: currentLineVoltages) {
                    allLineMotorVoltages.insert(allLineMotorVoltages.end(), i.sign * i.voltage);
                }
                allMotorVoltages.insert(allMotorVoltages.end(), allLineMotorVoltages);
            }
            motorData.close();
            for (auto& line: allMotorVoltages) {
                for (int i = 0; i < line.size(); i++) {
                    motorCodes[i].motor->move_voltage(line[i]);
                }
                pros::delay(20);
            }
        }
    }
}