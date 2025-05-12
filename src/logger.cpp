#include "logger.h"
#include <sstream>

thread_local int tid = -1;

Logger::~Logger() {
    for (auto* destination : _log_destinations) {
        delete destination;
    }
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

std::string Logger::getCurrentTime() {
    char fmt[64], buf[64];
    struct timeval  tv;

    gettimeofday(&tv, NULL);
    const struct tm tm = *localtime(&tv.tv_sec);

    strftime(fmt, sizeof(fmt), "%Y-%m-%dT%H:%M:%S.%%06u%z", &tm);
    snprintf(buf, sizeof(buf), fmt, tv.tv_usec);
    return buf;
}

std::string Logger::logLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}