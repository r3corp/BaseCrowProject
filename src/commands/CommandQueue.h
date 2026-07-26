#pragma once
#include "CommandFactory.h"
#include "ICommandRepository.h"
#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

class CommandQueue {
public:
    CommandQueue(std::shared_ptr<ICommandRepository> repository, const CommandFactory& factory);
    ~CommandQueue();

    CommandQueue(const CommandQueue&) = delete;
    CommandQueue& operator=(const CommandQueue&) = delete;

    // Agenda o comando (ja salvo no repositorio) para processamento pelo worker.
    void enqueue(const std::string& id);

private:
    void workerLoop();
    void processCommand(const std::string& id);

    std::shared_ptr<ICommandRepository> repository_;
    const CommandFactory& factory_;

    std::mutex mutex_;
    std::condition_variable cv_;
    std::queue<std::string> pending_;
    std::atomic<bool> running_;
    std::thread worker_;
};
