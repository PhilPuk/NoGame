/*
# ----------------------------------------------
# File: simple_logger.cpp
# Author: https://github.com/Pilzschaf
# Description: This file was not coded by me, but by another author.
# Current Version: Adaption from original.
# ----------------------------------------------
*/

#include <iostream>

void initLogger() {
    // Nothing to do
}

void exitLogger() {
    // Just to make sure. But shouldn't be necessary as std::endl already flushes
    std::cout << std::flush;
}

void logInfo(size_t lineNumber, const char* file, const char* message, size_t messageLength) {
    std::cout << '[' << file << ':' << lineNumber << "]" << "[INFO]" << ": " << message << std::endl;
}

void logWarn(size_t lineNumber, const char* file, const char* message, size_t messageLength) {
    std::cout << "\033[33m[" << file << ':' << lineNumber << "]" << "[WARN]" << ": " << message << "\033[0m" << std::endl;
}

void logError(size_t lineNumber, const char* file, const char* message, size_t messageLength) {
    std::cout << "\033[31m[" << file << ':' << lineNumber << "]" << "[ERR]" << ": " << message << "\033[0m" << std::endl;
}