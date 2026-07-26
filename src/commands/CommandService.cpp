#include "CommandService.h"
#include <ctime>

CommandService::CommandService(std::shared_ptr<ICommandRepository> repository, CommandQueue& queue)
    : repository_(std::move(repository)), queue_(queue) {}

Command CommandService::submit(const std::string& id, const std::string& type, const std::string& payload) {
    auto existing = repository_->findById(id);
    if (existing) {
        return *existing;
    }

    Command command;
    command.id = id;
    command.type = type;
    command.payload = payload;
    command.status = CommandStatus::PENDING;
    command.createdAt = std::time(nullptr);
    command.updatedAt = command.createdAt;

    repository_->save(command);
    queue_.enqueue(id);
    return command;
}

std::optional<Command> CommandService::getStatus(const std::string& id) {
    return repository_->findById(id);
}

std::vector<Command> CommandService::listAll() {
    return repository_->findAll();
}
