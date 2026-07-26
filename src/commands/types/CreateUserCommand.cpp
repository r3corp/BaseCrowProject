#include "CreateUserCommand.h"
#include "../../models/User.h"
#include "crow.h"

CreateUserCommand::CreateUserCommand(std::shared_ptr<UserRepository> repository)
    : repository_(std::move(repository)) {}

CommandStatus CreateUserCommand::execute(const CommandExecutionContext& context, std::string& resultOrError) {
    auto data = crow::json::load(context.payload);
    if (!data || !data.has("name") || !data.has("email")) {
        resultOrError = "payload invalido: 'name' e 'email' sao obrigatorios";
        return CommandStatus::FAILED;
    }

    User user;
    user.name = data["name"].s();
    user.email = data["email"].s();
    repository_->save(user);

    crow::json::wvalue result;
    result["name"] = user.name;
    result["email"] = user.email;
    resultOrError = result.dump();
    return CommandStatus::COMPLETED;
}
