#include "Logger.hpp"
#include <iostream>

constexpr bool HasDebugStreamEnabled = true;

Logger debugstream(std::clog, "DEBG", HasDebugStreamEnabled);
Logger infostream(std::clog, "INFO");
Logger warningstream(std::cerr, "WARN");
Logger errorstream(std::cerr, "ERRO");
