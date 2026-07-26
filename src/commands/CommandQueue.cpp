#include "CommandQueue.h"
#include "../logging/Logger.hpp"
#include <ctime>

CommandQueue::CommandQueue(std::shared_ptr<ICommandRepository> repository, const CommandFactory& factory)
    : repository_(std::move(repository)), factory_(factory), running_(true) {
    worker_ = std::thread(&CommandQueue::workerLoop, this);
}

CommandQueue::~CommandQueue() {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        running_ = false;
    }
    cv_.notify_all();
    if (worker_.joinable()) {
        worker_.join();
    }
}

void CommandQueue::enqueue(const std::string& id) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        pending_.push(id);
    }
    cv_.notify_one();
}

void CommandQueue::workerLoop() {
    while (true) {
        std::string id;
        {
            std::unique_lock<std::mutex> lock(mutex_);
            cv_.wait(lock, [this] { return !pending_.empty() || !running_; });
            if (!running_ && pending_.empty()) {
                return;
            }
            id = pending_.front();
            pending_.pop();
        }
        processCommand(id);
    }
}

void CommandQueue::processCommand(const std::string& id) {
    auto commandOpt = repository_->findById(id);
    if (!commandOpt) {
        return;
    }
    Command command = *commandOpt;
    command.status = CommandStatus::PROCESSING;
    command.updatedAt = std::time(nullptr);
    repository_->update(command);

    CommandExecutionContext context{command.id, command.type, command.payload};
    std::string resultOrError;
    CommandStatus newStatus;
    try {
        auto impl = factory_.create(command.type);
        newStatus = impl->execute(context, resultOrError);
    } catch (const std::exception& e) {
        newStatus = CommandStatus::FAILED;
        resultOrError = e.what();
    }

    command.status = newStatus;
    command.updatedAt = std::time(nullptr);
    if (newStatus == CommandStatus::FAILED) {
        command.error = resultOrError;
    } else {
        command.result = resultOrError;
    }
    repository_->update(command);

    Logger::getInstance().info("Command " + id + " (" + command.type + ") -> " + toString(newStatus));

    if (newStatus == CommandStatus::PARTIAL) {
        enqueue(id);
    }
}
