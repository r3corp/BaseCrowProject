#pragma once
#include "../interfaces/IUserRepository.h"
#include "../models/User.h"
#include <sqlite3.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <optional>

class UserRepository : public IUserRepository {
  sqlite3* db;

public:
  UserRepository() ;

  ~UserRepository() {
      sqlite3_close(db);
  }

  void addUser(const User& user) override;


  User getUser(int id) override ;

  std::optional<User> findById(int id) override;

  std::optional<User> findUserByName(const std::string& userName) override;

  void save(const User& user) override;

  std::vector<User> getAll(void) override;

  void remove(int id) override;

  void update(const User& user) override;

};
