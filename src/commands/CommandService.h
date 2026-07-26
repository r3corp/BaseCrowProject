#pragma once
#include "CommandQueue.h"
#include "ICommandRepository.h"
#include <memory>
#include <optional>
#include <string>
#include <vector>

class CommandService {
public:
    CommandService(std::shared_ptr<ICommandRepository> repository, CommandQueue& queue);

    // Idempotente: se `id` ja existir, devolve o registro existente sem
    // reenfileirar. Caso contrario, cria o comando (status PENDING), salva e
    // enfileira para processamento.
    Command submit(const std::string& id, const std::string& type, const std::string& payload);

    std::optional<Command> getStatus(const std::string& id);
    std::vector<Command> listAll();

private:
    std::shared_ptr<ICommandRepository> repository_;
    CommandQueue& queue_;
};
