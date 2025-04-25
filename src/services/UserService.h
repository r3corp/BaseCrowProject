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
  private:
      std::shared_ptr<UserRepository> repository;
  };