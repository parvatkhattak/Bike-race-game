#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include <sstream>

class Logger {
public:
    enum class Level {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    static Logger& GetInstance() {
        static Logger instance;
        return instance;
    }

    void Init(const std::string& filename = "game.log") {
        logFile.open(filename, std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "Failed to open log file: " << filename << std::endl;
        }
    }

    void Log(Level level, const std::string& message) {
        std::string levelStr = LevelToString(level);
        std::string timestamp = GetTimestamp();
        std::string logMessage = "[" + timestamp + "] [" + levelStr + "] " + message;

        // Console output with colors
        std::cout << GetColorCode(level) << logMessage << "\033[0m" << std::endl;

        // File output
        if (logFile.is_open()) {
            logFile << logMessage << std::endl;
            logFile.flush();
        }
    }

    void Debug(const std::string& message) { Log(Level::DEBUG, message); }
    void Info(const std::string& message) { Log(Level::INFO, message); }
    void Warning(const std::string& message) { Log(Level::WARNING, message); }
    void Error(const std::string& message) { Log(Level::ERROR, message); }

    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

private:
    Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::ofstream logFile;

    std::string LevelToString(Level level) {
        switch (level) {
            case Level::DEBUG: return "DEBUG";
            case Level::INFO: return "INFO";
            case Level::WARNING: return "WARNING";
            case Level::ERROR: return "ERROR";
            default: return "UNKNOWN";
        }
    }

    std::string GetColorCode(Level level) {
        switch (level) {
            case Level::DEBUG: return "\033[36m";    // Cyan
            case Level::INFO: return "\033[32m";     // Green
            case Level::WARNING: return "\033[33m";  // Yellow
            case Level::ERROR: return "\033[31m";    // Red
            default: return "\033[0m";               // Reset
        }
    }

    std::string GetTimestamp() {
        std::time_t now = std::time(nullptr);
        char buf[100];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        return std::string(buf);
    }
};

// Convenient macros
#define LOG_DEBUG(msg) Logger::GetInstance().Debug(msg)
#define LOG_INFO(msg) Logger::GetInstance().Info(msg)
#define LOG_WARNING(msg) Logger::GetInstance().Warning(msg)
#define LOG_ERROR(msg) Logger::GetInstance().Error(msg)

#endif // LOGGER_H
