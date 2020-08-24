#pragma once
#include <ostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <chrono>
#include <mutex>
#include "Noncopyable.h"
#include "Nonmovable.h"

class Logger 
    :private Nonmovable, private Noncopyable
{
public:
    Logger(std::ostream& output, const std::string& level, bool enabled = true);
    class LoggerProxy 
        :private Noncopyable
    {
    public:
        LoggerProxy(Logger& logger) noexcept;
        LoggerProxy(LoggerProxy&& rhs) noexcept;
        template <typename T>
        inline LoggerProxy& operator<<(const T& what) noexcept {
            if(m_pLogger->mEnabled) {
                m_pLogger->mBuffer << what;
            }
            return *this;
        }
        ~LoggerProxy();
    private:
        Logger* m_pLogger;
    };
    template <typename T>
    inline LoggerProxy operator<<(const T& what) noexcept {
        if(mEnabled) {
            auto _now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            auto* now = localtime(&_now);
            return std::move(LoggerProxy(*this) << "[" << std::put_time(now, "%H:%M:%S") << "]"
                    << '[' << mLevel << "] " << what);
        }
        return LoggerProxy(*this);
    }
    void flush();
private:
    std::mutex mMutex;
    std::ostream& mOutput;
    std::string mLevel;
    std::ostringstream mBuffer;
    const bool mEnabled;
};
