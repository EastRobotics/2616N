#include "rerunAuton.hpp"

void recordRerun(void* a)
{
    if (pros::usd::is_installed() && !pros::competition::is_connected()) {
        while (true) {
            if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT) && !pros::competition::is_connected()) {
                std::ofstream voltageData  ("/usd/voltageData.txt", std::ios::out | std::ios::trunc);
                std::ofstream positionData ("/usd/positionData.txt", std::ios::out | std::ios::trunc);
                std::ofstream velocityData ("/usd/velocityData.txt", std::ios::out | std::ios::trunc);

                if (voltageData.is_open() && positionData.is_open()) {
                    for (auto& i: motorCodes)
                        i.motor->tare_position();

                    while (controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT));
                    uint32_t startTime = pros::millis();
                    int counter = 0;
                    while (!controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
                        counter++;
                        for (auto& i: motorCodes) {
                            char buffer[7];
                            // Format: ±XXXXX, ie. 732 -> +00732, -3321 -> -03321
                            sprintf(buffer, "%+06d", i.motor->get_voltage());
                            std::cout << buffer << ' ';
                            voltageData << buffer << ' ';
                            sprintf(buffer, "%+06d", i.motor->get_position());
                            positionData << buffer << ' ';
                            sprintf(buffer, "%+06d", i.motor->get_actual_velocity());
                            velocityData << buffer << ' ';
                        }
                        voltageData << std::endl;
                        pros::delay(20);
                    }

                    int timeSpent = pros::millis() - startTime;
                    std::cout << "Average duration of 1 record cycle: " <<
                                  double(timeSpent) / double(counter) <<
                                 " milliseconds (should be 20)";
                    voltageData.close();
                }
            }
            pros::delay(350);
        }
    }
}

void replayRerun()
{   
    constexpr int ONE_LINE_LENGTH = 57;
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
            
            int counter = 0;
            uint32_t startTime = pros::millis();

            for(int i = 0; i < motorDataString.length()-ONE_LINE_LENGTH; i += ONE_LINE_LENGTH) {
                counter++;

                std::string currentLine = motorDataString.substr(i, ONE_LINE_LENGTH);
                std::cout << "------" << "\n";
                std::cout << currentLine << "\n";
                std::cout << "------" << "\n";
                for (int j = 0, k = 0; j < ONE_LINE_LENGTH-1; j += ONE_VOLTAGE_LENGTH, k++) {
                    std::cout << k << "\n";
                    std::cout << currentLine.substr(j, ONE_NUM_LENGTH+1) << "\n";
                    std::cout << std::stoi(currentLine.substr(j, ONE_NUM_LENGTH+1)) << "\n";
                    motorCodes[k].motor->move_voltage(std::stoi(currentLine.substr(j, ONE_NUM_LENGTH+1)));
                }
                pros::delay(20);
            }

            int32_t timeSpent = pros::millis() - startTime;
            std::cout << "Average duration of 1 replay cycle: " <<
                          double(timeSpent) / double(counter) <<
                         " milliseconds (should be 20)";
        }
    }
}