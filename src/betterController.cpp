#include "betterController.hpp"
// #include "general.hpp"

betterController::betterController(pros::controller_id_e_t id) : pros::Controller::Controller(id) {}

void betterController::set_text(std::uint8_t line, std::uint8_t col, const char* str)
{
    contQueue.push(std::make_tuple(line, col, str));
}

void betterController::rumble(const char* rumble_pattern)
{
    contQueue.push(std::make_tuple(3, 0, rumble_pattern));
}

void betterController::clear()
{
    contQueue.push(std::make_tuple(-1, 0, ""));
}

void betterController::clear_line(std::uint8_t line)
{
    contQueue.push(std::make_tuple(line, 0, "               "));
}

void bContPrintTask(void* cont)
{
    betterController contr = *(betterController *)cont;
    std::tuple<int, int, const char*> args;
    while (true) {
        if (!contr.contQueue.empty()) {
            args = contr.contQueue.front();
            contr.contQueue.pop();
            contr.set_text(std::get<0>(args), std::get<1>(args), std::get<2>(args));
        }
        pros::delay(50);
    }
}

