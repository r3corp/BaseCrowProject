#pragma once
#include <string>

struct CommandExecutionContext {
    std::string id;
    std::string type;
    std::string payload;
};
