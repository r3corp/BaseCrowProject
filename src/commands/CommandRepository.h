#pragma once
#include "ICommandRepository.h"
#include <mutex>
#include <unordered_map>

class CommandRepository : public ICommandRepository {
public:
    void save(const Command& command) override;
    std::optional<Command> findById(const std::string& id) override;
    void update(const Command& command) override;
    std::vector<Command> findAll() override;

private:
    std::mutex mutex_;
    std::unordered_map<std::string, Command> commands_;
};
