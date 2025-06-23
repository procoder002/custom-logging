#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <sstream>
#include <string>
#include <mutex>
#include <thread>
#include <chrono>
#include <map>
#include <unistd.h>
#include <sys/syscall.h>
#include <libgen.h>
#include "log_destination.h"

#define __FILENAME__ basename(const_cast<char*>(__FILE__))

#define LOG_DEBUG(...) \
    Logger::getInstance().log(LogLevel::DEBUG, __FILENAME__, __LINE__, __VA_ARGS__)

#define LOG_INFO(...) \
    Logger::getInstance().log(LogLevel::INFO, __FILENAME__, __LINE__, __VA_ARGS__)

#define LOG_WARN(...) \
    Logger::getInstance().log(LogLevel::WARNING, __FILENAME__, __LINE__, __VA_ARGS__)

#define LOG_ERROR(...) \
    Logger::getInstance().log(LogLevel::ERROR, __FILENAME__, __LINE__, __VA_ARGS__)

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL,
    UNKNOWN
};

class Logger {
public:
    static Logger& getInstance();

    std::string getLogLevel();

    void setLogLevel(LogLevel level) { _current_log_level = level; }
    void setLogLevel(std::string levelstr);

    void addLogDestination(std::string destname, LogDestination* destination);
    void removeLogDestination(std::string destname); 

    std::vector<std::string> getDestinations();

    // Helper functions
    std::string getCurrentTime();

    // Log to console and file
    template <typename... Args>
    void log(LogLevel level, char* filename, int lineno, Args... args);

    void concatenateArgs(std::ostringstream& log) {
        // Base case for recursion, no more arguments to process
    }

    template <typename T, typename... Args>
    void concatenateArgs(std::ostringstream& log, T&& first, Args&&... args) {
        log << std::forward<T>(first);  // Add the first argument
        concatenateArgs(log, std::forward<Args>(args)...);  // Recurse for the remaining arguments
    }

    // Destructor to clean up
    ~Logger();

private:
    Logger() : _current_log_level(LogLevel::DEBUG) { pid = getpid(); }

    std::string logLevelToString(LogLevel level);
    LogLevel    stringToLoglevel(std::string level);

    LogLevel                        _current_log_level;
    std::map<std::string, LogDestination*>    _log_destinations;
    std::mutex          mtx;
    int                 pid;
};

extern thread_local int tid;

template <typename... Args>
void Logger::log(LogLevel level, char* filename, int lineno, Args... args) {
    if (level < _current_log_level) return;
    std::lock_guard<std::mutex> lock(mtx);// Ensure thread safety

    if(tid == -1) { tid = syscall(SYS_gettid); }

    std::ostringstream log_stream;
    log_stream << getCurrentTime() << " [" << logLevelToString(level) << "] ";

    // Recursively append the remaining arguments
    concatenateArgs(log_stream, args...);

    log_stream << " (" << filename << ": " << lineno << ")" << "  { ProcessID: " << pid << ", ThreadID: " << tid << " }";

    // Forward log message to each destination
    for (auto& destination : _log_destinations) {
        destination.second->log(log_stream.str());
    }
}

#endif // LOGGER_H
