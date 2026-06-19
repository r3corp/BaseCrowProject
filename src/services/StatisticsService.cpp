#include "StatisticsService.h"
#include <fstream>
#include <sstream>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <iomanip>
#include <cmath>

StatisticsService::StatisticsService() : running(true), cpuUsage(0.0f), totalMemory(0), usedMemory(0), uptime(0) {
    refreshData();
    refreshThread = std::thread(&StatisticsService::refreshLoop, this);
}

StatisticsService::~StatisticsService() {
    running = false;
    if (refreshThread.joinable()) {
        refreshThread.join();
    }
}

StatisticsService& StatisticsService::getInstance() {
    static StatisticsService instance;
    return instance;
}

void StatisticsService::refreshLoop() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        refreshData();
    }
}

void StatisticsService::refreshData() {
    std::lock_guard<std::mutex> lock(dataMutex);

    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        totalMemory = info.totalram;
        usedMemory = info.totalram - info.freeram;
        uptime = info.uptime;
    }

    std::ifstream osFile("/etc/os-release");
    std::string line;
    while (std::getline(osFile, line)) {
        if (line.find("PRETTY_NAME=") == 0) {
            osName = line.substr(13, line.length() - 14); // Remove aspas
            break;
        }
    }

    std::ifstream versionFile("/proc/version");
    std::getline(versionFile, kernelVersion);

    std::ifstream statFile("/proc/stat");
    std::string cpuLine;
    std::getline(statFile, cpuLine);
    std::istringstream ss(cpuLine);
    std::string cpu;
    uint64_t user, nice, system, idle;
    ss >> cpu >> user >> nice >> system >> idle;
    cpuUsage = 100.0f * (user + nice + system) / (user + nice + system + idle);
}

std::string StatisticsService::getOSName() {
    std::lock_guard<std::mutex> lock(dataMutex);
    return osName;
}

std::string StatisticsService::getKernelVersion() {
    std::lock_guard<std::mutex> lock(dataMutex);
    return kernelVersion;
}

std::string StatisticsService::getCPUUsage() {
    std::lock_guard<std::mutex> lock(dataMutex);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << cpuUsage << "%";
    return oss.str();
}

std::string StatisticsService::getTotalMemory() {
    std::lock_guard<std::mutex> lock(dataMutex);
    return formatMemory(totalMemory);
}

std::string StatisticsService::getUsedMemory() {
    std::lock_guard<std::mutex> lock(dataMutex);
    return formatMemory(usedMemory);
}

std::string StatisticsService::getUptime() {
    std::lock_guard<std::mutex> lock(dataMutex);
    return formatUptime(uptime);
}

std::string StatisticsService::formatMemory(uint64_t bytes) {
    double gib = static_cast<double>(bytes) / (1024 * 1024 * 1024);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3) << gib;
    return oss.str();
}

std::string StatisticsService::formatUptime(uint64_t seconds) {
    uint64_t days = seconds / 86400;
    seconds %= 86400;
    uint64_t hours = seconds / 3600;
    seconds %= 3600;
    uint64_t minutes = seconds / 60;
    seconds %= 60;

    std::ostringstream oss;
    oss << days << "d:"
        << std::setw(2) << std::setfill('0') << hours << "h:"
        << std::setw(2) << std::setfill('0') << minutes << "m:"
        << std::setw(2) << std::setfill('0') << seconds << "s";
    return oss.str();
}
