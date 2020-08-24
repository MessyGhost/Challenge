#include "Logger.hpp"

Logger::Logger(std::ostream& output, const std::string& level, bool enabled)
    :mOutput(output), mLevel(level), mEnabled(enabled)
{

}

void Logger::flush() {
    mMutex.lock();
    mOutput << mBuffer.str() << std::endl;
    mBuffer.str("");
    mMutex.unlock();
}

Logger::LoggerProxy::LoggerProxy(Logger& logger) noexcept 
    :m_pLogger(&logger)
{
    
}

Logger::LoggerProxy::LoggerProxy(Logger::LoggerProxy&& rhs) noexcept 
    :m_pLogger(rhs.m_pLogger)
{
    rhs.m_pLogger = nullptr;
}

Logger::LoggerProxy::~LoggerProxy() {
    if(m_pLogger && m_pLogger->mEnabled) m_pLogger->flush();
}
