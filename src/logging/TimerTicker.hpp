#pragma once
#include <chrono>
#include <string>
#include <iostream>

#include "Logger.hpp"

class TimerTicker {
public:
    TimerTicker( std::string name) : start_time(std::chrono::steady_clock::now()), name(name) {}

    ~TimerTicker() {
        auto end_time = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        // Log the duration or perform any necessary actions
        Logger::getInstance().info("Duration for " + name + ": " + std::to_string(duration) + " ms");
        std::cout << "Duration for " << name << ": " << duration << " ms" << std::endl;
    }
private:
    std::chrono::steady_clock::time_point start_time;
    std::string name;
};