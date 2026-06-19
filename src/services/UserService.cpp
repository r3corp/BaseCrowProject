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

std::string UserService::getUserJson(int id) {
    auto user = repository->findById(id);
    if (user) {
        crow::json::wvalue user_json;
        user_json["id"] = user->id;
        user_json["name"] = user->name;
        user_json["email"] = user->email;
        return user_json.dump();
    }
    return "{}"; // Return empty JSON if not found
}

std::string UserService::createUser(const crow::json::wvalue& userJson) {
    User new_user;
//    new_user.name = userJson["username"].s();
//    new_user.email = userJson["email"].s();
    repository->save(new_user);
    return getAllUsersJson(); // Return all users after creation
}

std::string UserService::updateUser(int id, const std::string& userJson) {
    crow::json::rvalue user_data = crow::json::load(userJson);
    if (!user_data) {
        return "{}"; // Return empty JSON if invalid input
    }
    User updated_user;
    updated_user.id = id;
    updated_user.name = user_data["name"].s();
    updated_user.email = user_data["email"].s();
    repository->update(updated_user);
    return getUserJson(id); // Return the updated user
}

void UserService::deleteUser(int id) {
    repository->remove(id);
}