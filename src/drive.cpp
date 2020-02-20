#include "drive.hpp"

void joystickDataFixer(int &x, int &y) {
        double r = sqrt(pow(x, 2) + pow(y, 2));
        if (r < DEADZONE_RADIUS)
        {
            x = 0;
            y = 0;
            return;
        }

        r = (r - DEADZONE_RADIUS) * 128 / (128.0 - DEADZONE_RADIUS);

        double theta = (x < 0) * 180 + (180 / M_PI) * atan(y / (x * 1.0));
        if (x == 0) 
            theta = 90 + 180 * (y < 0);
        

        if (theta < 0)
            theta += 360;
        
        if(fabs(fabs(round(theta/90))-fabs(theta/90)) < ANGLE_TOLERANCE/90.0) {
            theta = round(theta/90.0)*90;
        }

        x = round(r * cos(M_PI / 180 * theta));
        y = round(r * sin(M_PI / 180 * theta));
}

void drive()
{
    motorSlowdown = controller.get_digital(pros::E_CONTROLLER_DIGITAL_A) ? 2 : 1;

    int y = 0;
    int x = 0;
    int left = 0;
    int right = 0;
    switch(DRIVE_MODE) {
        case SINGLE_STICK_ARCADE:
            y = controller.get_analog(ANALOG_LEFT_Y);
            x = controller.get_analog(ANALOG_LEFT_X);
            joystickDataFixer(x, y);

            f_left_mtr = (y + x)/motorSlowdown;
            f_right_mtr = (y - x)/motorSlowdown;
            b_left_mtr = (y + x)/motorSlowdown;
            b_right_mtr = (y - x)/motorSlowdown;
            break;
        case DOUBLE_STICK_ARCADE:
            y = controller.get_analog(ANALOG_LEFT_Y);
            x = controller.get_analog(ANALOG_RIGHT_X);

            f_left_mtr = (y + x)/motorSlowdown;
            f_right_mtr = (y - x)/motorSlowdown;
            b_left_mtr = (y + x)/motorSlowdown;
            b_right_mtr = (y - x)/motorSlowdown;
            break;
        case DOUBLE_STICK_TANK:
            left = controller.get_analog(ANALOG_LEFT_Y);
            right = controller.get_analog(ANALOG_RIGHT_Y);

            f_left_mtr = left/motorSlowdown;
            f_right_mtr = right/motorSlowdown;
            b_left_mtr = left/motorSlowdown;
            b_right_mtr = right/motorSlowdown;
            break;
    }
}
