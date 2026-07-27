#include "crow.h"

#include "rest/ApiRouter.h"
#include "services/UserService.h"
#include "db/UserRepository.h"
#include "logging/Logger.hpp"
#include "logging/TimerTicker.hpp"

#include "commands/CommandFactory.h"
#include "commands/CommandQueue.h"
#include "commands/CommandRepository.h"
#include "commands/CommandRouter.h"
#include "commands/CommandService.h"
#include "commands/types/CreateUserCommand.h"


int main() {
    Logger::getInstance().info("Starting the application...");
    TimerTicker ticker("Application running");
    crow::SimpleApp app;
    auto repo = std::make_shared<UserRepository>();
    UserService service(repo);

    auto commandRepository = std::make_shared<CommandRepository>();
    CommandFactory commandFactory;
    commandFactory.registerType("create_user", [repo]() {
        return std::make_unique<CreateUserCommand>(repo);
    });
    CommandQueue commandQueue(commandRepository, commandFactory);
    CommandService commandService(commandRepository, commandQueue);

    ApiRouter::setup(app, service, commandService);
    CommandRouter::setup(app, commandService, service);

    app.port(18080).multithreaded().run();
}