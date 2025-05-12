#ifndef LOG_DESTINATION_H
#define LOG_DESTINATION_H

#include <string>
#include <iostream>
#include <fstream>
#include <queue>
#include <sys/time.h>
#include <stdint.h>

class LogDestination {
public:
    virtual ~LogDestination() = default;
    virtual void log(const std::string& message) = 0;
};

class ConsoleLogDestination : public LogDestination {
public:
    void log(const std::string& message) override {
        std::cout << message << std::endl;
    }
};

class FileLogDestination : public LogDestination {
public:
    FileLogDestination(const std::string& filename, uint32_t maxLogFileSizeLimit, uint8_t maxOldLogFileCount);

    ~FileLogDestination() {
        if (file.is_open()) file.close();
    }

    void log(const std::string& message) override;

    void rotateLogFile();
    void truncateOldLogs();

    uint32_t getFileSize() { return file.tellp(); }

private:
    std::string                 log_file_name;
    std::ofstream               file;
    uint32_t                    max_log_file_size_limit;
    uint8_t                     max_old_log_file_count;
    std::queue<std::string>     old_log_files;
};

class NetworkLogDestination : public LogDestination {
public:
    void log(const std::string& message) override {
        // Simulate sending the log message over the network (e.g., HTTP, UDP, KAFKA, Remote Database etc.)
        std::cout << "[Network] " << message << std::endl;
    }
};

#endif // LOG_DESTINATION_H
