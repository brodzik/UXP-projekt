#pragma once

#include <ctime>
#include <optional>
#include <string>
union IntBytes {
    int integer;
    char bytes[4];
};

enum LindaOperation {
    OUTPUT,
    INPUT,
    READ
};

struct LindaCommand {
    LindaOperation op;
    std::string data;
    int timeout;
};

std::optional<LindaCommand> GetLindaCommand(std::string raw);

bool operator<(timespec &lhs, timespec &rhs);
