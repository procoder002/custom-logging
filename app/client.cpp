#include <iostream>
#include "logger.h"

#define THREAD_COUNT 10

std::vector<std::string> logLevel = {"debug", "info", "warn", "error", "fatal"};

void threadFunction(int i) {

    while(true) {
        int n = rand() % 5;
        LOG_INFO("This is an ", logLevel[n]," message from thread-", i);
        usleep(200);
    }
}

int main() {
    // Get the singleton instance
    Logger& logger = Logger::getInstance();

    logger.addLogDestination(new ConsoleLogDestination());  // Log to console

    /*  start writing the logs into app.log
     *  this log file will get rotated once file size reaches to 2 MB
     *  keep 3 such rotated logs and delete the older logs 
     */ 
    logger.addLogDestination(new FileLogDestination("app.log", 2*1024*1024, 3));  // Log to file

    // logger.addLogDestination(new NetworkLogDestination());  // Simulate network logging

    // Set log level to INFO (will log messages of level INFO and higher)
    logger.setLogLevel(LogLevel::INFO);

    std::vector<std::thread> t;
    for (size_t i = 0; i < THREAD_COUNT; i++)
    {
        t.emplace_back(threadFunction, i);
    }

    for (size_t i = 0; i < THREAD_COUNT; i++)
    {
        t[i].join();
    }

    return 0;
}
