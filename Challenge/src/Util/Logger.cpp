#include "Logger.hpp"

Logger::Logger(std::ostream& output, const std::string& level, bool enabled)
    :mOutput(output), mLevel(level), mProxy(*this), mEnabled(enabled)
{

}

Logger::LoggerProxy::LoggerProxy(Logger& logger) noexcept 
    :mLogger(logger)
{
    
}
