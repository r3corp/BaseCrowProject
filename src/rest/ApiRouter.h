#pragma once
#include "crow.h"
#include "../services/UserService.h"

class ApiRouter {
public:
    static void setup(crow::SimpleApp& app, UserService& service);
};