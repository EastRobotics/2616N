#pragma once
#define B_CONTROLLER_HPP
#ifndef GENERAL_HPP
#include "main.h"
#endif
#include <queue>
#include <tuple>

class betterController : public pros::Controller {
    private:
        typedef std::tuple<int, int, const char*> contPrintFmt;
        std::queue<contPrintFmt> contQueue;
    public:
        betterController (pros::controller_id_e_t id) : pros::Controller::Controller(id) {}

        template <typename... Params>
        void print(std::uint8_t line, std::uint8_t col, const char* fmt, Params... args);

        void set_text(std::uint8_t line, std::uint8_t col, const char* str);
        void rumble(const char* rumble_pattern);
        void clear(void);
        void clear_line(std::uint8_t line);

        void printTask(void* a);
};

extern betterController controller;