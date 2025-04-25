#include "ApiRouter.h"
#include "crow/mustache.h"


void ApiRouter::setup(crow::SimpleApp& app, UserService& service) {

  //std::string basePath = std::filesystem::current_path().string();
  //std::string viewsPath = basePath;
  //crow::mustache::set_base(viewsPath);

  CROW_ROUTE(app, "/users")([&service]() {
        return crow::response{service.getAllUsersJson()};
    });

  CROW_ROUTE(app, "/users/html")([&service]() {
      auto users = service.getAllUsersJson();
      crow::mustache::context ctx;
      ctx["users"] = users;
      return crow::mustache::load("users.mustache").render(ctx);
  });
}