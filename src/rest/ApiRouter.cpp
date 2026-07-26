#include "ApiRouter.h"
#include "../commands/CommandRouter.h"

crow::json::wvalue ApiRouter::parseUrlEncodedToJson(const std::string& body) {
  TimerTicker ticker("ApiRouter::parseUrlEncodedToJson");
  crow::json::wvalue result;
  std::istringstream ss(body);
  std::string pair;

  while (std::getline(ss, pair, '&')) {
      auto pos = pair.find('=');
      if (pos != std::string::npos) {
          auto key = pair.substr(0, pos);
          auto value = pair.substr(pos + 1);
          result[key] = value;
      }
  }
  return result;
}

void ApiRouter::setup(crow::SimpleApp& app, UserService& service, CommandService& commandService) {
  TimerTicker ticker("ApiRouter::setup");
  //std::string basePath = std::filesystem::current_path().string();
  //std::string viewsPath = basePath;
  //crow::mustache::set_base(viewsPath);

  CROW_ROUTE(app, "/")([&service]() {
    //crow::mustache::context ctx;
    return crow::mustache::load("index.mustache").render();//ctx);
  });

  CROW_ROUTE(app, "/users")([&service]() {
    return crow::response{service.getAllUsersJson()};
  });

  CROW_ROUTE(app, "/users/html")([&service]() {
      auto users = service.getAllUsersJson();
      crow::mustache::context ctx;
      ctx["users"] = users;
      return crow::mustache::load("users.mustache").render(ctx);
  });
  
  CROW_ROUTE(app, "/users/<int>")([&service](int id) {
        return crow::response{service.getUserJson(id)};
  });

  CROW_ROUTE(app, "/users/<int>/html")([&service](int id) {
      auto user = service.getUserJson(id);
      crow::mustache::context ctx;
      ctx["user"] = user;
      return crow::mustache::load("user.mustache").render(ctx);
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

  CROW_ROUTE(app, "/users/<int>").methods("PUT"_method)([&service](const crow::request& req, int id) {
        auto user = service.updateUser(id, req.body);
        return crow::response{user};
  });

  CROW_ROUTE(app, "/users/<int>/html").methods("PUT"_method)([&service](const crow::request& req, int id) {
      auto user = service.updateUser(id, req.body);
      crow::mustache::context ctx;
      ctx["user"] = user;
      return crow::mustache::load("user.mustache").render(ctx);
  });

  CROW_ROUTE(app, "/users/new")([]() {
      crow::mustache::context ctx;
      return crow::mustache::load("new_user_form.mustache").render(ctx);
  });

  CROW_ROUTE(app, "/shutdown")([&]() {
      app.stop();
      return crow::response{200};
  });
}