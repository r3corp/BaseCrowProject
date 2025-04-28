#pragma once
#include "crow.h"
#include "crow/utility.h"
#include "crow/mustache.h"
#include "../services/UserService.h"
#include <filesystem>
#include <string>


class ApiRouter {
public:
    static void setup(crow::SimpleApp& app, UserService& service);
    static crow::json::wvalue parseUrlEncodedToJson(const std::string& body);
};