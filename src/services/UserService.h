#pragma once
#include "../db/UserRepository.h"
#include <string>
#include "crow.h"
#include "crow/json.h"


// src/services/UserService.h
class UserService {
  public:
      UserService(std::shared_ptr<UserRepository> repo);
      std::string getAllUsersJson();
      std::string getUserJson(int id);
      std::string createUser(const crow::json::wvalue& userJson);
      std::string updateUser(int id, const std::string& userJson);
      void deleteUser(int id);
  private:
      std::shared_ptr<UserRepository> repository;
  };