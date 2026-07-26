#pragma once
#include <string>

enum class CommandStatus {
    PENDING,
    PROCESSING,
    PARTIAL,
    COMPLETED,
    FAILED
};

inline std::string toString(CommandStatus status) {
    switch (status) {
        case CommandStatus::PENDING: return "PENDING";
        case CommandStatus::PROCESSING: return "PROCESSING";
        case CommandStatus::PARTIAL: return "PARTIAL";
        case CommandStatus::COMPLETED: return "COMPLETED";
        case CommandStatus::FAILED: return "FAILED";
    }
    return "UNKNOWN";
}
