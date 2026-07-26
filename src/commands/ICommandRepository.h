#pragma once
#include "Command.h"
#include <optional>
#include <string>
#include <vector>

class ICommandRepository {
public:
    virtual ~ICommandRepository() = default;

    virtual void save(const Command& command) = 0;
    virtual std::optional<Command> findById(const std::string& id) = 0;
    virtual void update(const Command& command) = 0;
    virtual std::vector<Command> findAll() = 0;
};
