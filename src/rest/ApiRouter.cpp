#include "ApiRouter.h"
#include "../commands/CommandRouter.h"
#include <cstdlib>

namespace {

// application/x-www-form-urlencoded: '+' vira espaco, %XX vira o byte hex.
std::string urlDecode(const std::string& value) {
  std::string decoded;
  decoded.reserve(value.size());
  for (std::size_t i = 0; i < value.size(); ++i) {
      char c = value[i];
      if (c == '+') {
          decoded += ' ';
      } else if (c == '%' && i + 2 < value.size()) {
          std::string hex = value.substr(i + 1, 2);
          decoded += static_cast<char>(std::strtol(hex.c_str(), nullptr, 16));
          i += 2;
      } else {
          decoded += c;
      }
  }
  return decoded;
}

} // namespace

crow::json::wvalue ApiRouter::parseUrlEncodedToJson(const std::string& body) {
  TimerTicker ticker("ApiRouter::parseUrlEncodedToJson");
  crow::json::wvalue result;
  std::istringstream ss(body);
  std::string pair;

  while (std::getline(ss, pair, '&')) {
      auto pos = pair.find('=');
      if (pos != std::string::npos) {
          auto key = urlDecode(pair.substr(0, pos));
          auto value = urlDecode(pair.substr(pos + 1));
          result[key] = value;
      }
  }
  return result;
}

crow::json::wvalue ApiRouter::userToJson(const User& user) {
  crow::json::wvalue json;
  json["id"] = user.id;
  json["name"] = user.name;
  json["email"] = user.email;
  return json;
}

void ApiRouter::setup(crow::SimpleApp& app, UserService& service, CommandService& commandService) {
  TimerTicker ticker("ApiRouter::setup");

  CROW_ROUTE(app, "/")([]() {
    crow::mustache::context ctx;
    ctx["title"] = "Rodrigo Rocha";
    return crow::mustache::load("index.mustache").render(ctx);
  });

  CROW_ROUTE(app, "/users")([&service]() {
    return crow::response{service.getAllUsersJson()};
  });

  CROW_ROUTE(app, "/users/html")([&service]() {
      auto users = service.getAllUsers();
      crow::mustache::context ctx;
      ctx["title"] = "Usuários";
      std::vector<crow::json::wvalue> items;
      items.reserve(users.size());
      for (const auto& user : users) {
          items.push_back(ApiRouter::userToJson(user));
      }
      ctx["users"] = std::move(items);
      return crow::mustache::load("users.mustache").render(ctx);
  });

  CROW_ROUTE(app, "/users/<int>")([&service](int id) {
        return crow::response{service.getUserJson(id)};
  });

  CROW_ROUTE(app, "/users/<int>/row/html")([&service](int id) {
      auto user = service.getUser(id);
      crow::mustache::context ctx;
      if (user) {
          ctx = ApiRouter::userToJson(*user);
      }
      return crow::mustache::load("user_row.mustache").render(ctx);
  });

  CROW_ROUTE(app, "/users/<int>/edit/html")([&service](int id) {
      auto user = service.getUser(id);
      crow::mustache::context ctx;
      if (user) {
          ctx = ApiRouter::userToJson(*user);
      }
      return crow::mustache::load("user_row_edit.mustache").render(ctx);
  });

  CROW_ROUTE(app, "/users/<int>").methods("DELETE"_method)([&service](const crow::request& req, int id) {
        service.deleteUser(id);
        return crow::response{200};
  });

  CROW_ROUTE(app, "/users/save").methods("POST"_method)([&commandService](const crow::request& req) {
        // parseUrlEncodedToJson so produz um wvalue (write-only); dump+reload
        // para conseguir ler os campos de volta.
        auto formJson = ApiRouter::parseUrlEncodedToJson(req.body);
        auto formData = crow::json::load(formJson.dump());
        if (!formData || !formData.has("id")) {
            return crow::response{400, "{\"error\":\"campo 'id' (chave de idempotencia) e obrigatorio\"}"};
        }

        std::string id = formData["id"].s();
        crow::json::wvalue payload;
        payload["name"] = formData.has("name") ? std::string(formData["name"].s()) : std::string();
        payload["email"] = formData.has("email") ? std::string(formData["email"].s()) : std::string();

        bool alreadyExisted = commandService.getStatus(id).has_value();
        Command command = commandService.submit(id, "create_user", payload.dump());

        int statusCode = alreadyExisted ? 200 : 202;
        return crow::response{statusCode, CommandRouter::toJson(command).dump()};
  });

  CROW_ROUTE(app, "/users/save/html").methods("POST"_method)([&service, &commandService](const crow::request& req) {
        auto formJson = ApiRouter::parseUrlEncodedToJson(req.body);
        auto formData = crow::json::load(formJson.dump());
        if (!formData || !formData.has("id")) {
            return crow::response{400, "campo 'id' (chave de idempotencia) e obrigatorio"};
        }

        std::string id = formData["id"].s();
        crow::json::wvalue payload;
        payload["name"] = formData.has("name") ? std::string(formData["name"].s()) : std::string();
        payload["email"] = formData.has("email") ? std::string(formData["email"].s()) : std::string();

        Command command = commandService.submit(id, "create_user", payload.dump());
        return crow::response{CommandRouter::renderStatus(command, service)};
  });

  CROW_ROUTE(app, "/users/<int>").methods("PUT"_method)([&service](const crow::request& req, int id) {
        auto user = service.updateUser(id, req.body);
        return crow::response{user};
  });

  CROW_ROUTE(app, "/users/<int>/html").methods("PUT"_method)([&service](const crow::request& req, int id) {
      auto formJson = ApiRouter::parseUrlEncodedToJson(req.body);
      service.updateUser(id, formJson.dump());

      auto updated = service.getUser(id);
      crow::mustache::context ctx;
      if (updated) {
          ctx = ApiRouter::userToJson(*updated);
      }
      return crow::mustache::load("user_row.mustache").render(ctx);
  });

  CROW_ROUTE(app, "/users/new")([]() {
      crow::mustache::context ctx;
      ctx["title"] = "Novo usuário";
      return crow::mustache::load("new_user_form.mustache").render(ctx);
  });

  CROW_ROUTE(app, "/shutdown")([&]() {
      app.stop();
      return crow::response{200};
  });
}
