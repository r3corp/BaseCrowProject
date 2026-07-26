#pragma once
#include "ICommand.h"
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

class CommandFactory {
public:
    using Builder = std::function<std::unique_ptr<ICommand>()>;

    void registerType(const std::string& type, Builder builder);
    std::unique_ptr<ICommand> create(const std::string& type) const;

private:
    std::unordered_map<std::string, Builder> builders_;
};
