#include "log_destination.h"
#include "logger.h"

FileLogDestination::FileLogDestination(const std::string& filename, uint32_t maxLogFileSizeLimit, uint8_t maxOldLogFileCount) {
    log_file_name = filename;
    file.open(filename, std::ios::out | std::ios::app);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open log file.");
    }

    max_log_file_size_limit = maxLogFileSizeLimit;
    max_old_log_file_count = maxOldLogFileCount;
}

void FileLogDestination::log(const std::string& message) {
    // check if the log file exceeds the size limit
    if(getFileSize() > max_log_file_size_limit) {
        rotateLogFile();
    }
    file << message << std::endl;
}

void FileLogDestination::rotateLogFile() {
    if (file.is_open()) {
        file.close();
    }
    std::string timestamp = Logger::getInstance().getCurrentTime();
    std::string archivedLogFileName = log_file_name + "." + timestamp;
    if (rename(log_file_name.c_str(), archivedLogFileName.c_str()) == 0) {
        old_log_files.push(archivedLogFileName);
        std::cout << "Log file rotated. Archived as: " << archivedLogFileName << std::endl;
        truncateOldLogs();
    } else {
        std::cerr << "Error renaming file: " << archivedLogFileName << std::endl;
    }

    file.open(log_file_name, std::ios::out | std::ios::app);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open log file.");
    }
}

void FileLogDestination::truncateOldLogs() {
    if(old_log_files.size() > max_old_log_file_count) {
        std::string filename = old_log_files.front();
        old_log_files.pop();

        if (remove(filename.c_str()) == 0) {
            std::cout << filename << " File deleted successfully" << std::endl;
        } else {
            std::cerr << "Error deleting file: " << filename << std::endl;
        }
    }
}