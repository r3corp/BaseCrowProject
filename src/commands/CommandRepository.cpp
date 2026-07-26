#include "CommandRepository.h"

void CommandRepository::save(const Command& command) {
    std::lock_guard<std::mutex> lock(mutex_);
    commands_[command.id] = command;
}

std::optional<Command> CommandRepository::findById(const std::string& id) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = commands_.find(id);
    if (it == commands_.end()) {
        return std::nullopt;
    }
    return it->second;
}

void CommandRepository::update(const Command& command) {
    std::lock_guard<std::mutex> lock(mutex_);
    commands_[command.id] = command;
}

std::vector<Command> CommandRepository::findAll() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<Command> result;
    result.reserve(commands_.size());
    for (const auto& [id, command] : commands_) {
        result.push_back(command);
    }
    return result;
}
