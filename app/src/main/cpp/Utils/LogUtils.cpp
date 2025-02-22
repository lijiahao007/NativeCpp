//
// Created by ljh on 2025/2/22.
//

#include "LogUtils.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

std::mutex LogUtils::logMutex;
std::string LogUtils::currentDate;
std::ofstream LogUtils::logFile;
std::string LogUtils::logDir;

std::mutex LogUtils::bufferMutex;
std::condition_variable LogUtils::cv;
std::deque<std::string> LogUtils::buffer;
std::atomic<bool> LogUtils::running(false);
std::thread LogUtils::writerThread;


std::string LogUtils::getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y%m%d", std::localtime(&time));
    return buf;
}

void LogUtils::ensureLogFile() {
    std::string today = getCurrentDate();

    if (currentDate != today || !logFile.is_open()) {
        if (logFile.is_open()) {
            logFile.close();
        }

        std::string dirPath = logDir;
        if (mkdir(dirPath.c_str(), 0777) == 0) {
            __android_log_write(ANDROID_LOG_INFO, "LogUtils", "Log directory created successfully");
        } else if (errno != EEXIST) {
            __android_log_write(ANDROID_LOG_ERROR, "LogUtils", "Failed to create log directory");
            return;
        }

        std::string filePath = dirPath + "/log_" + today + ".log";
        logFile.open(filePath, std::ios::app);
        if (logFile.is_open()) {
            __android_log_write(ANDROID_LOG_INFO, "LogUtils", ("Log file created: " + filePath).c_str());
            currentDate = today;
        } else {
            __android_log_write(ANDROID_LOG_ERROR, "LogUtils", ("Failed to open log file: " + filePath).c_str());
        }
    }
}


void LogUtils::writerWorker() {
    std::deque<std::string> writeBuffer;
    while (running || !buffer.empty()) {
        {
            std::unique_lock<std::mutex> lock(bufferMutex);
            cv.wait_for(lock, std::chrono::seconds(1), [&]{
                return !buffer.empty() || !running;
            });

            if (!buffer.empty()) {
                buffer.swap(writeBuffer);
            }
        }

        if (!writeBuffer.empty()) {
            std::lock_guard<std::mutex> fileLock(logMutex);
            try {
                ensureLogFile();
                if (logFile.is_open()) {
                    for (const auto& msg : writeBuffer) {
                        logFile << msg;
                    }
                    logFile.flush();
                }
            } catch (...) {
                __android_log_write(ANDROID_LOG_ERROR, "LogUtils", "Batch write failed");
            }
            writeBuffer.clear();
        }
    }
}


void LogUtils::init(const std::string &fileDir) {
    logDir = fileDir;
    running = true;
    writerThread = std::thread(writerWorker);
}

void LogUtils::shutdown() {
    running = false;
    cv.notify_one();
    if (writerThread.joinable()) {
        writerThread.join();
    }
    std::lock_guard<std::mutex> lock(logMutex);
    if (logFile.is_open()) {
        logFile.close();
    }
}


void LogUtils::writeToFile(const char *level, const char *tag, const std::string &message) {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);


    std::string logEntry =
            std::string("[") + GetTimestamp() + "] [" + level + "] [" + tag + "] " + message + "\n";

    {
        std::lock_guard<std::mutex> lock(bufferMutex);
        buffer.push_back(logEntry);
        if (buffer.size() >= MAX_BUFFER_SIZE) {
            cv.notify_one();
        }
    }
    cv.notify_one();
}

std::string LogUtils::GetTimestamp() {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto t = system_clock::to_time_t(now);

    // 线程安全的时间转换
    struct tm tm{};
    localtime_r(&t, &tm);

    // 组合时间字符串
    std::ostringstream oss;
    oss << std::put_time(&tm, "%F %T") << "."  // %F=%Y-%m-%d %T=%H:%M:%S
        << std::setw(3) << std::setfill('0')
        << duration_cast<milliseconds>(now.time_since_epoch()).count() % 1000;

    return oss.str();
}


void LogUtils::log_impl(int androidLevel, const char* levelStr, const char* tag, const std::string& message) {
    // 输出到Android log和文件
    __android_log_write(androidLevel, tag, message.c_str());
    writeToFile(levelStr, tag, message);
}

void LogUtils::verbose(const char *tag, const std::string& message) {
    log_impl(ANDROID_LOG_VERBOSE, "VERBOSE", tag, message);
}

void LogUtils::debug(const char *tag, const std::string& message) {
    log_impl(ANDROID_LOG_DEBUG, "DEBUG", tag, message);
}

void LogUtils::info(const char *tag, const std::string& message) {
    log_impl(ANDROID_LOG_INFO, "INFO", tag, message);
}

void LogUtils::warn(const char *tag, const std::string& message) {
    log_impl(ANDROID_LOG_WARN, "WARN", tag, message);
}

void LogUtils::error(const char *tag, const std::string& message) {
    log_impl(ANDROID_LOG_ERROR, "ERROR", tag, message);
}

void LogUtils::fatal(const char *tag, const std::string& message) {
    log_impl(ANDROID_LOG_FATAL, "FATAL", tag, message);
}
