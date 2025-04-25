#pragma once
#include "../models/User.h"
#include <optional>
#include <vector>
#include <string>

class IUserRepository {
public:
    virtual ~IUserRepository() = default;
    virtual void addUser(const User& user) = 0;
    virtual User getUser(int id) = 0;

    virtual std::optional<User> findById(int id) = 0;

    virtual std::optional<User> findUserByName(const std::string& userName) = 0;

    virtual void save(const User& user) = 0;

    virtual std::vector<User> getAll(void) = 0;
};
