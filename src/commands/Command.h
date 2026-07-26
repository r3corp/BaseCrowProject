#pragma once
#include "CommandStatus.h"
#include <ctime>
#include <string>

struct Command {
    std::string id;
    std::string type;
    std::string payload;
    CommandStatus status = CommandStatus::PENDING;
    std::string result;
    std::string error;
    std::time_t createdAt = 0;
    std::time_t updatedAt = 0;
};
