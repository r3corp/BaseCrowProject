#pragma once

#include <string>
#include <iostream>
#include <memory>
#include <mutex>

class Logger
{
public:
    static Logger& getInstance() {
      //Meyers Singleton pattern with thread safety
        static Logger _logger;
        return _logger;
    }

    void info(const std::string& message) {
        // Implement logging logic here, e.g., write to console or file
        std::cout << "[INFO] " << message << std::endl;
    }
    // You can add more logging levels (e.g., debug, error) as needed

    Logger(const Logger&) = delete; // Delete copy constructor
    Logger& operator=(const Logger&) = delete; // Delete copy assignment operator
    Logger(const Logger&&) = delete; // Delete move constructor
    Logger& operator=(const Logger&&) = delete; // Delete move assignment operator

private:
    Logger() = default; // Private constructor for singleton pattern
    ~Logger() = default; // Default destructor
};
