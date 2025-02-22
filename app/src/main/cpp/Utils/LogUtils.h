//
// Created by ljh on 2025/2/22.
//

#ifndef NATIVECPP_LOGUTILS_H
#define NATIVECPP_LOGUTILS_H
#include <android/log.h>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <mutex>
#include <sys/stat.h>
#include <deque>
#include <condition_variable>
#include <vector>
#include <thread>

class LogUtils {
private:
    static std::mutex logMutex;
    static std::string currentDate;
    static std::ofstream logFile;

    static std::mutex bufferMutex;
    static std::condition_variable cv;
    static std::deque<std::string> buffer;
    static std::atomic<bool> running;
    static std::thread writerThread;
    static const int MAX_BUFFER_SIZE = 1000;

    static std::string getCurrentDate();
    static void ensureLogFile();
    static void writerWorker();
    static std::string string_format(const char* format, ...);
    static void writeToFile(const char* level, const char* tag, const char* message);
    static void log_impl(int androidLevel, const char *levelStr, const char *tag, const char *format,
             va_list args);

public:
    static void init();
    static void shutdown();
    static void error(const char *tag, const std::exception &ex);
    static void verbose(const char *tag, const char *format, ...);
    static void debug(const char *tag, const char *format, ...);
    static void info(const char *tag, const char *format, ...);
    static void warn(const char *tag, const char *format, ...);
    static void error(const char *tag, const char *format, ...);
    static void fatal(const char *tag, const char *format, ...);
};


#endif //NATIVECPP_LOGUTILS_H