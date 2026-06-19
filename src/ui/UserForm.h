// src/ui/UserController.cpp
#include "crow.h"
#include "../services/UserService.h"

void setupRoutes(crow::SimpleApp& app, UserService& service) {
    CROW_ROUTE(app, "/users")([&service](){
        return crow::response{service.getAllUsersJson()};
    });
}