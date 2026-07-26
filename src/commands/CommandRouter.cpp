#include "CommandRouter.h"
#include "../logging/TimerTicker.hpp"

crow::json::wvalue CommandRouter::toJson(const Command& command) {
    crow::json::wvalue json;
    json["id"] = command.id;
    json["type"] = command.type;
    json["status"] = toString(command.status);
    json["payload"] = command.payload;
    json["result"] = command.result;
    json["error"] = command.error;
    json["createdAt"] = static_cast<int64_t>(command.createdAt);
    json["updatedAt"] = static_cast<int64_t>(command.updatedAt);
    return json;
}

void CommandRouter::setup(crow::SimpleApp& app, CommandService& service) {
    TimerTicker ticker("CommandRouter::setup");

    CROW_ROUTE(app, "/comandos").methods("POST"_method)([&service](const crow::request& req) {
        auto parsed = crow::json::load(req.body);
        if (!parsed || !parsed.has("id") || !parsed.has("type") || !parsed.has("payload")) {
            return crow::response{400, "{\"error\":\"id, type e payload sao obrigatorios\"}"};
        }

        std::string id = parsed["id"].s();
        std::string type = parsed["type"].s();
        crow::json::wvalue payloadCopy = parsed["payload"];
        std::string payloadStr = payloadCopy.dump();

        bool alreadyExisted = service.getStatus(id).has_value();
        Command command = service.submit(id, type, payloadStr);

        int statusCode = alreadyExisted ? 200 : 202;
        return crow::response{statusCode, CommandRouter::toJson(command).dump()};
    });

    CROW_ROUTE(app, "/comandos/<string>")([&service](const std::string& id) {
        auto command = service.getStatus(id);
        if (!command) {
            return crow::response{404, "{\"error\":\"comando nao encontrado\"}"};
        }
        return crow::response{200, CommandRouter::toJson(*command).dump()};
    });

    CROW_ROUTE(app, "/comandos")([&service]() {
        auto commands = service.listAll();
        std::vector<crow::json::wvalue> items;
        items.reserve(commands.size());
        for (const auto& command : commands) {
            items.push_back(CommandRouter::toJson(command));
        }
        crow::json::wvalue result = std::move(items);
        return crow::response{result.dump()};
    });
}
