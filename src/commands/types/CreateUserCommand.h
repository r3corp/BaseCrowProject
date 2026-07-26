#pragma once
#include "../../db/UserRepository.h"
#include "../ICommand.h"
#include <memory>

class CreateUserCommand : public ICommand {
public:
    explicit CreateUserCommand(std::shared_ptr<UserRepository> repository);

    CommandStatus execute(const CommandExecutionContext& context, std::string& resultOrError) override;

private:
    std::shared_ptr<UserRepository> repository_;
};
