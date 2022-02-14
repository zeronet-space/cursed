#define CURSED_DEBUG
#define BUFFER_SIZE 1024
#include "stdafx.h"

#include "cursed.h"
#include "loadfunc.h"
#include "security.h"

int main() {
    spdlog::set_pattern("[%^%8l%$] %v");
#ifdef CURSED_DEBUG
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::set_level(spdlog::level::info);
#endif
    setlocale(LC_ALL, "Russian");
    SetConsoleOutputCP(866);

    spdlog::debug("running security::protect");
    security::protect();

    spdlog::debug("everything is OK");

    SOCKET server = cursed::connect("127.0.0.1", 5555);

    while (auto data = cursed::c_recv(&server)) {
        spdlog::debug(data);
    }

    cursed::destroy(&server);
}
