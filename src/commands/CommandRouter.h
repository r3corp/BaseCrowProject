#pragma once
#include "Command.h"
#include "CommandService.h"
#include "crow.h"

class CommandRouter {
public:
    static void setup(crow::SimpleApp& app, CommandService& service);
    static crow::json::wvalue toJson(const Command& command);
};
