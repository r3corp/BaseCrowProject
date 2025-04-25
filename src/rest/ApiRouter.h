#pragma once
#include "crow.h"
#include "../services/UserService.h"
#include <filesystem>
#include <string>


class ApiRouter {
public:
    static void setup(crow::SimpleApp& app, UserService& service);
};