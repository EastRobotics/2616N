#pragma once
#include "main.h"
#include <queue>
#include <tuple>

class betterController : public pros::Controller {
    public:
        betterController (pros::controller_id_e_t id);

        // This definition has to be here or else it just can't compile because yay templates
        template <typename... Params>
        void print(std::uint8_t line, std::uint8_t col, const char* fmt, Params... args) {
            char * str;
            sprintf(str, fmt, args...);
            contQueue.push(std::make_tuple(line, col, str));
        }

        void set_text(std::uint8_t line, std::uint8_t col, const char* str);
        void rumble(const char* rumble_pattern);
        void clear(void);
        void clear_line(std::uint8_t line);

        std::queue<std::tuple<int, int, const char*>> contQueue;
};
void bContPrintTask(void* cont);

extern betterController controller;
