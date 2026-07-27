#pragma once
#include "crow.h"
#include "crow/utility.h"
#include "crow/mustache.h"
#include "../services/UserService.h"
#include "../commands/CommandService.h"
#include "../logging/TimerTicker.hpp"
#include "../models/User.h"

#include <filesystem>
#include <string>


class ApiRouter {
public:
    static void setup(crow::SimpleApp& app, UserService& service, CommandService& commandService);
    static crow::json::wvalue parseUrlEncodedToJson(const std::string& body);
    static crow::json::wvalue userToJson(const User& user);

};