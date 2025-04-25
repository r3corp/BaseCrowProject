// UserService.cpp - placeholder content
#include "UserService.h"


UserService::UserService(std::shared_ptr<UserRepository> repo) : repository(repo) {}
    

std::string UserService::getAllUsersJson() {
    auto users = repository->getAll();
    crow::json::wvalue result;
    for (auto& user : users) {
      crow::json::wvalue user_json;
      user_json["id"] = user.id;
      user_json["name"] = user.name;
      user_json["email"] = user.email;
      result["users"] = std::move(user_json);//({{"id", user.id}, {"name", user.name}, {"email", user.email}});
    }
    return result.dump();
}
