#include "crow.h"

#include "rest/ApiRouter.h"
#include "services/UserService.h"
#include "db/UserRepository.h"
#include "logging/Logger.hpp"
#include "logging/TimerTicker.hpp"


int main() {
    
    Logger::getInstance().info("Starting the application...");
    TimerTicker ticker("Application running");
    crow::SimpleApp app;
    auto repo = std::make_shared<UserRepository>();
    UserService service(repo);
    ApiRouter::setup(app, service);
    app.port(18080).multithreaded().run();
}