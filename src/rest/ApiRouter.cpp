#include "ApiRouter.h"

crow::json::wvalue ApiRouter::parseUrlEncodedToJson(const std::string& body) {
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

void ApiRouter::setup(crow::SimpleApp& app, UserService& service) {

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

  CROW_ROUTE(app, "/users/save").methods("POST"_method)([&service](const crow::request& req) {
        auto user_data = ApiRouter::parseUrlEncodedToJson(req.body);
        /*if (!user_data) {
            return crow::response{400}; // Bad Request
        }*/
        auto users = service.createUser(user_data);
        return crow::response{users};
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
}