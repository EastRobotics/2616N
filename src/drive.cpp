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
        if (x == 0) {
            theta = 90 + 180 * (y < 0);
            return;
        }

        if (theta < 0)
            theta += 360;

        x = round(r * cos(M_PI / 180 * theta));
        y = round(r * sin(M_PI / 180 * theta));
}

void drive()
{
    int y = 0;
    int x = 0;
    int left = 0;
    int right = 0;
    switch(DRIVE_MODE) {
        case SINGLE_STICK_ARCADE:
            y = controller.get_analog(ANALOG_LEFT_Y)/motorSlowdown;
            x = controller.get_analog(ANALOG_LEFT_X)/motorSlowdown;
            joystickDataFixer(x, y);

            f_left_mtr = y + x;
            f_right_mtr = y - x;
            b_left_mtr = y + x;
            b_right_mtr = y - x;
            break;
        case DOUBLE_STICK_ARCADE:
            y = controller.get_analog(ANALOG_LEFT_Y)/motorSlowdown;
            x = controller.get_analog(ANALOG_RIGHT_X)/motorSlowdown;

            f_left_mtr = y + x;
            f_right_mtr = y - x;
            b_left_mtr = y + x;
            b_right_mtr = y - x;
            break;
        case DOUBLE_STICK_TANK:
            left = controller.get_analog(ANALOG_LEFT_Y)/motorSlowdown;
            right = controller.get_analog(ANALOG_RIGHT_Y)/motorSlowdown;

            f_left_mtr = left;
            f_right_mtr = right;
            b_left_mtr = left;
            b_right_mtr = right;
            break;
    }
}
