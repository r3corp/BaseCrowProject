#pragma once
#include "Command.h"
#include "CommandService.h"
#include "../services/UserService.h"
#include "crow.h"

class CommandRouter {
public:
    static void setup(crow::SimpleApp& app, CommandService& service, UserService& userService);
    static crow::json::wvalue toJson(const Command& command);

    // Fragmento HTML (mustache) do status do comando, usado tanto pela
    // resposta inicial de POST /users/save/html quanto pelo poll em
    // GET /comandos/<id>/html. Quando COMPLETED, embute o swap
    // out-of-band da tabela de usuarios (por isso precisa do UserService).
    static std::string renderStatus(const Command& command, UserService& userService);
};
