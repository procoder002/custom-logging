cmake_minimum_required(VERSION 2.8)

project(custom_logging)

# Compiler and flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g") # Add -Wall and -Wextra

# Include directory
include_directories(./include)

# Source files
set(SRCS
    src/log_destination.cpp
    src/logger.cpp
)

# Static library name
set(LIB_NAME clogger)  # Just the name, CMake adds the prefix and extension

# Output binary (if you want to create an executable as well)
set(BINARY client_app)

# Create the static library
add_library(${LIB_NAME} STATIC ${SRCS})

# Create the executable (optional)
add_executable(${BINARY} app/client.cpp) # Assuming you have a main.cpp file
target_link_libraries(${BINARY} ${LIB_NAME} pthread) # Link the executable with the library
