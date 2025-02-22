//
// Created by ljh on 2025/2/22.
//

#include "LogUtils.h"


std::mutex LogUtils::logMutex;
std::string LogUtils::currentDate;
std::ofstream LogUtils::logFile;

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

        std::string dirPath = "/sdcard/Download/myapplication/logs/";
        mkdir(dirPath.c_str(), 0777);

        std::string filePath = dirPath + "log_" + today + ".log";
        logFile.open(filePath, std::ios::app);
        currentDate = today;
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

std::string LogUtils::string_format(const char* format, ...) {
    va_list args;
    va_start(args, format);

    int length = vsnprintf(nullptr, 0, format, args);
    va_end(args);

    if (length <= 0) return "";

    std::vector<char> buf(length + 1);

    va_start(args, format);
    vsnprintf(buf.data(), buf.size(), format, args);
    va_end(args);

    return {buf.data()};
}

void LogUtils::writeToFile(const char* level, const char* tag, const char* message) {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    char timeStr[20];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&time));

    std::string logEntry = string_format("[%s][%s][%s] %s\n",
                                         timeStr, level, tag, message);

    {
        std::lock_guard<std::mutex> lock(bufferMutex);
        buffer.push_back(logEntry);
        if (buffer.size() >= MAX_BUFFER_SIZE) {
            cv.notify_one();
        }
    }
    cv.notify_one();
}

void LogUtils::init() {
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


void LogUtils::verbose(const char *tag, const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_impl(ANDROID_LOG_VERBOSE, "VERBOSE", tag, format, args);
    va_end(args);
}

void LogUtils::debug(const char *tag, const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_impl(ANDROID_LOG_DEBUG, "DEBUG", tag, format, args);
    va_end(args);
}

void LogUtils::info(const char *tag, const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_impl(ANDROID_LOG_DEBUG, "INFO", tag, format, args);
    va_end(args);
}

void LogUtils::warn(const char *tag, const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_impl(ANDROID_LOG_DEBUG, "WARN", tag, format, args);
    va_end(args);
}

void LogUtils::error(const char *tag, const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_impl(ANDROID_LOG_DEBUG, "ERROR", tag, format, args);
    va_end(args);
}

void LogUtils::error(const char *tag, const std::exception &ex) {
    __android_log_write(ANDROID_LOG_ERROR, tag, ex.what());
    writeToFile("ERROR", tag, ex.what());
}

void LogUtils::fatal(const char *tag, const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_impl(ANDROID_LOG_DEBUG, "FATAL", tag, format, args);
    va_end(args);
}


void LogUtils::log_impl(int androidLevel, const char* levelStr, const char* tag, const char* format, va_list args) {
    // 第一次调用获取长度
    va_list argsCopy;
    va_copy(argsCopy, args);
    int length = vsnprintf(nullptr, 0, format, argsCopy);
    va_end(argsCopy);

    if (length <= 0) return;

    // 精确分配缓冲区（+1 for null终止符）
    std::vector<char> buffer(length + 1);
    vsnprintf(buffer.data(), buffer.size(), format, args);

    // 输出到Android log和文件
    __android_log_write(androidLevel, tag, buffer.data());
    writeToFile(levelStr, tag, buffer.data());
}
