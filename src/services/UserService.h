#include "../db/UserRepository.h"
#include <string>

// src/services/UserService.h
class UserService {
  public:
      UserService(std::shared_ptr<UserRepository> repo);
      std::string getAllUsersJson();
  private:
      std::shared_ptr<UserRepository> repository;
  };