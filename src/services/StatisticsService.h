#ifndef STATISTICS_SERVICE_H
#define STATISTICS_SERVICE_H

#include <string>
#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>

class StatisticsService {
public:
    static StatisticsService& getInstance();

    std::string getOSName();
    std::string getKernelVersion();
    std::string getCPUUsage();       // Retorna como string com %
    std::string getTotalMemory();    // Retorna como GiB
    std::string getUsedMemory();     // Retorna como GiB
    std::string getUptime();         // Retorna como dias:horas:minutos:segundos

private:
    StatisticsService();
    ~StatisticsService();
    StatisticsService(const StatisticsService&) = delete;
    StatisticsService& operator=(const StatisticsService&) = delete;

    void refreshLoop();
    void refreshData();
    std::string formatMemory(uint64_t bytes);
    std::string formatUptime(uint64_t seconds);

    std::mutex dataMutex;
    std::atomic<bool> running;
    std::thread refreshThread;

    std::string osName;
    std::string kernelVersion;
    float cpuUsage;
    uint64_t totalMemory;
    uint64_t usedMemory;
    uint64_t uptime;
};

#endif // STATISTICS_SERVICE_H
