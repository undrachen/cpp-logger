#include "cpp_logger/logger.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

constexpr uint8_t NUMBER_OF_MS_DIGITS = 4U;
constexpr uint32_t MS_TIME_DIVISOR = 10'000U;

namespace
{

std::tm getLocalTime(const std::time_t time)
{
    std::tm localTime;

#ifdef _WIN32
    localtime_s(&localTime, &time);
#else
    localtime_r(&time, &localTime);
#endif

    return localTime;
}

std::string getTimestampString()
{
    std::ostringstream timestamp;

    const auto now = std::chrono::system_clock::now();
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % MS_TIME_DIVISOR;

    const std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    const std::tm localTime = getLocalTime(currentTime);

    timestamp << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S")
              << '.'
              << std::setfill('0')
              << std::setw(NUMBER_OF_MS_DIGITS)
              << ms.count();

    return timestamp.str();
}

}  // namespace

namespace cpp_logger
{

static std::string_view logLevelToString(LogLevel level)
{
    switch(level)
    {
        case LogLevel::Debug:
            return "DEBUG";
        case LogLevel::Info:
            return "INFO";
        case LogLevel::Warning:
            return "WARNING";
        case LogLevel::Error:
            return "ERROR";
    }
    return "UNKNOWN";
}

void Logger::debug(std::string_view message) const
{
    log(LogLevel::Debug, message);
}

void Logger::info(std::string_view message) const
{
    log(LogLevel::Info, message);
}

void Logger::warning(std::string_view message) const
{
    log(LogLevel::Warning, message);
}

void Logger::error(std::string_view message) const
{
    log(LogLevel::Error, message);
}

void Logger::log(LogLevel level, std::string_view message) const
{
    if (level >= mMinLevel)
    {
        std::cout << "[" << getTimestampString() << "] "
                  << "[" << logLevelToString(level) << "] "
                  << message << '\n';
    }
}

void Logger::setLevel(LogLevel level)
{
    mMinLevel = level;
}

}  // namespace cpp_logger
