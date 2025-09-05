#include "crow.h"

#include "rest/ApiRouter.h"
#include "services/UserService.h"
#include "services/StatisticsService.h"
#include "db/UserRepository.h"


int main() {
    crow::SimpleApp app;
    auto repo = std::make_shared<UserRepository>();
    crow::logger::setLogLevel(crow::LogLevel::Info);
    UserService service(repo);
    ApiRouter::setup(app, service);
    app.port(20180).multithreaded().run();
}