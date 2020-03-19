#include "rerunAuton.hpp"

void recordRerun(void* a)
{
    using namespace pros::c;
    if (pros::usd::is_installed() && !pros::competition::is_connected()) {
        std::array<loggingStruct, 5> loggingFileArr = {{
            {std::ofstream ("/usd/voltage.txt",      std::ios::trunc),  motor_get_voltage,        NULL,  1},
            {std::ofstream ("/usd/currentData.txt",  std::ios::trunc),  motor_get_current_draw,   NULL,  1},
            {std::ofstream ("/usd/positionData.txt", std::ios::trunc), NULL, motor_get_position,        1},
            {std::ofstream ("/usd/velocityData.txt", std::ios::trunc), NULL, motor_get_actual_velocity, 1},
            {std::ofstream ("/usd/powerData.txt",    std::ios::trunc), NULL, motor_get_power,           1000}
        }};
        std::ofstream controllerData ("/usd/controllerData.txt", std::ios::trunc);
        while (!pros::competition::is_connected()) {
            if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
                if (loggingFileArr[0].file.is_open()) {
                    for (auto& i: motorCodes)
                        i.motor->tare_position();

                    while (controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT));
                    uint32_t startTime = pros::millis();
                    int counter = 0;
                    while (!controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
                        uint32_t runStartTime = pros::millis();
                        counter++;
                        char buffer[7];
                        for (loggingStruct& i: loggingFileArr) {
                            for (uint8_t port: motorPortArray) {
                                if (i.functionInt != NULL){
                                    snprintf(buffer, 7, "%+06d", i.functionInt(port)*i.multiplier);
                                } else {
                                    snprintf(buffer, 7, "%+06d", (int)(i.functionDouble(port)*i.multiplier));
                                }
                                i.file << buffer << ' ';
                            }
                            i.file << std::endl;
                        }

                        for (auto& i: joystickAxesArray) {
                            sprintf(buffer, "%+03d", controller.get_analog(i));
                            controllerData << buffer << ' ';
                        }
                        for (auto& i: controllerButtonsArray)
                            controllerData << (int)controller.get_digital(i) << ' ';
                        controllerData << std::endl;

                        pros::Task::delay_until(&runStartTime, 20);
                    }

                    int timeSpent = pros::millis() - startTime;
                    std::cout << "Average duration of 1 record cycle: " <<
                                  double(timeSpent) / double(counter) <<
                                 " milliseconds (should be 20)";
                    std::cout << "." << std::endl;
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
        std::ifstream motorData ("/usd/voltageData.txt", std::ios::in);
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
                // std::cout << "------" << "\n";
                // std::cout << currentLine << "\n";
                // std::cout << "------" << "\n";
                for (int j = 0, k = 0; j < ONE_LINE_LENGTH-1; j += ONE_VOLTAGE_LENGTH, k++) {
                    // std::cout << k << "\n";
                    // std::cout << currentLine.substr(j, ONE_NUM_LENGTH+1) << "\n";
                    // std::cout << std::stoi(currentLine.substr(j, ONE_NUM_LENGTH+1)) << "\n";
                    motorCodes[k].motor->move_voltage(std::stoi(currentLine.substr(j, ONE_NUM_LENGTH+1)));
                }
                pros::delay(20);
            }

            int32_t timeSpent = pros::millis() - startTime;
            std::cout << "Average duration of 1 replay cycle: " <<
                          double(timeSpent) / double(counter) <<
                         " milliseconds (should be 20)";
            std::cout << "." << std::endl;
        }
    }
}