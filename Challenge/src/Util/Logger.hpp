#pragma once
#include <ostream>
#include <iomanip>
#include <string>
#include <chrono>
#include "Noncopyable.h"
#include "Nonmovable.h"

class Logger 
    :private Nonmovable, private Noncopyable
{
public:
    Logger(std::ostream& output, const std::string& level, bool enabled = true);
    class LoggerProxy 
        :private Noncopyable, private Nonmovable
    {
    public:
        LoggerProxy(Logger& logger) noexcept;
        template <typename T>
        inline LoggerProxy& operator<<(const T& what) noexcept {
            if(mLogger.mEnabled) {
                mLogger.mOutput << what;
            }
            return *this;
        }
    private:
        Logger& mLogger;
    };
    template <typename T>
    inline LoggerProxy& operator<<(const T& what) noexcept {
        if(mEnabled) {
            auto _now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            auto* now = localtime(&_now);
            mOutput << "\n[" << std::put_time(now, "%H:%M:%S") << "]"
                    << '[' << mLevel << "] " << what << std::flush;
        }
        return mProxy;
    }
private:
    std::ostream& mOutput;
    std::string mLevel;
    LoggerProxy mProxy;
    const bool mEnabled;
};
