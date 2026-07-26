#pragma once
#include "CommandExecutionContext.h"
#include "CommandStatus.h"
#include <string>

class ICommand {
public:
    virtual ~ICommand() = default;

    // Executa (ou retoma) o comando. Retornar CommandStatus::PARTIAL sinaliza
    // que o comando ainda nao terminou e deve ser reenfileirado para nova
    // execucao; COMPLETED/FAILED sao terminais. resultOrError recebe o
    // resultado (quando COMPLETED/PARTIAL) ou a mensagem de erro (quando FAILED).
    virtual CommandStatus execute(const CommandExecutionContext& context, std::string& resultOrError) = 0;
};
