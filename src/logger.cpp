#include "logger.h"
#include <sstream>

thread_local int tid = -1;

Logger::~Logger() {
    for (auto& dest : _log_destinations) {
	LogDestination* destination = dest.second;
        if(destination) delete destination;
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

LogLevel Logger::stringToLoglevel(std::string level) {
	if(level == "DEBUG") return LogLevel::DEBUG;
	else if(level == "INFO") return LogLevel::INFO;
	else if(level == "WARNING") return LogLevel::WARNING;
	else if(level == "ERROR") return LogLevel::ERROR;
	else if(level == "FATAL") return LogLevel::FATAL;
	else return LogLevel::UNKNOWN;
}

void Logger::addLogDestination(std::string destname, LogDestination* destination) 
{ 
	if(_log_destinations.find(destname) != _log_destinations.end()) {
		std::cout << "Log destionation already present for " << destname << std::endl;
	}
	else {
		_log_destinations[destname] = destination; 
	}
}

void Logger::removeLogDestination(std::string destname) 
{ 
	if(_log_destinations.find(destname) == _log_destinations.end()) {
		std::cout << "Log destionation not found for " << destname  << std::endl;
		return;
	}

	LogDestination* destination = _log_destinations[destname];
	delete destination;
	destination = NULL;
	_log_destinations.erase(destname); 
}

void Logger::setLogLevel(std::string levelstr)
{
	LogLevel level = stringToLoglevel(levelstr);
	if(level == LogLevel::UNKNOWN) {
		std::cerr << "log level not found" << std::endl;
		return;
	}
	setLogLevel(level);
}

std::string Logger::getLogLevel() 
{
	return logLevelToString(_current_log_level);
}

std::vector<std::string> Logger::getDestinations()
{
	std::vector<std::string> dests;
	for (auto& dest : _log_destinations) {
		dests.push_back(dest.first);
	}
	return dests;
}
