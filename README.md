# Custom Logging


* A framework which allows to write logs from a single source which will be send to multiple destination/sink
* Destinations can be console, file, remote server etc. which can be added/removed in the runtime
* User friendly MACROS (LOG_DEBUG, LOG_INFO, LOG_CRITICAL, LOG_ERROR) provided to denote the severity
* Only those logs will get considered whose log-level has higher value than currenty configured log-level  
* Variable argument list suppoted in the log message, like std::cout
* Log rotation feature, i.e. when the log file size crosses configured limit that file will be compressed and a new log file will be created for log writing
* Automatic deletion of old logs to avoid logs flooding and free the disk space 
* Current log format contains - timestamp, log-level, log-message, file and line no, process id, thread id

## How to use

* Clone the repo
```sh
    git clone https://github.com/procoder002/custom-logging.git
```
* Run configure script which will generate the 'clogger' library and a sample binary 'client_app' (which uses that library) under build directory
```sh
    chmod +x configure
    ./configure
```

* In application code use the following macros to write logs, see the example - app/client.cpp
```sh
    LOG_DEBUG("This is a sample log from ", userName)                               // 'userName' is a string
    LOG_INFO("User Id: ", userID, " Email: " , userEmail)                           // 'userID' is int and 'userEmail' is a string
    LOG_ERROR("Error Code: ", errCode, " flag: ", errFlag, " message: ", errMsg)    // 'errCode' is int, 'errFlag' is bool, 'errMsg' is string 
```

* Sample log looks like following,
```sh   
    2025-01-26T01:25:45.354551+0530 [ERROR] This is an error message (logger.h: 91)  { ProcessID: 39570, ThreadID: 39570 }
```

* Log file will be generated from the location where binary is running. Rotated log file name will be actual file name + timestamp,
```sh
    app.log.2025-05-12T16:01:50.729533+0530
    app.log.2025-05-12T16:01:54.638783+0530
    app.log.2025-05-12T16:01:58.742560+0530
    app.log
```

### Build Requirement:
* g++ compiler(--std=c++11)
* cmake >= 2.8

### Future Enhancement:
* Support for different format JSON,XML, protobuf etc.
* Asynchronous logging to boost performance

### Reference
* https://www.geeksforgeeks.org/logging-system-in-cpp/
* https://stackoverflow.com/questions/1736295/c-logging-framework-suggestions
* https://www.reddit.com/r/cpp/comments/18enjz2/easy_logging_a_logging_system_for_c20/
* https://leo88.medium.com/devlog-writing-a-logging-library-in-c-9601ec3699eb
