#include "CommandFactory.h"
#include <stdexcept>

void CommandFactory::registerType(const std::string& type, Builder builder) {
    builders_[type] = std::move(builder);
}

std::unique_ptr<ICommand> CommandFactory::create(const std::string& type) const {
    auto it = builders_.find(type);
    if (it == builders_.end()) {
        throw std::runtime_error("Unknown command type: " + type);
    }
    return it->second();
}
