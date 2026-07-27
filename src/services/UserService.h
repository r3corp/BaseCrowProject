#pragma once
#include "../db/UserRepository.h"
#include "../logging/TimerTicker.hpp"
#include "../models/User.h"
#include <optional>
#include <string>
#include <vector>
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

      // Passthroughs pra camada HTML (mustache), que precisa dos dados
      // como User de verdade, nao como string JSON ja serializada.
      std::vector<User> getAllUsers();
      std::optional<User> getUser(int id);
  private:
      std::shared_ptr<UserRepository> repository;
  };