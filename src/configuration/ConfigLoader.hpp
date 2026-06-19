#pragma once
#include <string>


class ConfigLoader {
public:
    static ConfigLoader& getInstance() {
        static ConfigLoader instance;
        return instance;
    }

    void loadFromFile(const std::string& filename) {
        // Implement logic to load configuration from a file
    }

private:
    ConfigLoader() {} // Private constructor for singleton pattern
    ConfigLoader(const ConfigLoader&) = delete; // Delete copy constructor
    ConfigLoader& operator=(const ConfigLoader&) = delete; // Delete copy assignment operator
};