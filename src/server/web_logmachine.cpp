#include "web_logmachine.h"

#include <cxxtools/log/cxxtools.h>

#include <map>

namespace
{

cxxtools::Logger* getLogger()
{
  static cxxtools::Logger* logger = nullptr;
  if (!::cxxtools::LoggerManager::isEnabled())
  {
      return nullptr;
  }
  if (logger == nullptr)
  {
      logger = ::cxxtools::LoggerManager::getInstance().getLogger("application");
  }
  return logger;
}

void do_log(cxxtools::Logger::log_level_type level, const std::string& msg)
{
    cxxtools::Logger* _cxxtools_logger = getLogger();
    if (_cxxtools_logger != nullptr && _cxxtools_logger->isEnabled(level))
    {
        ::cxxtools::LogMessage _cxxtools_logMessage(_cxxtools_logger, level);
        _cxxtools_logMessage.out() << msg;
        _cxxtools_logMessage.finish();
    }
}

}

web_logmachine::web_logmachine()
{
    static bool log_inited = false;
    if(!log_inited)
    {
        log_init("log_conf.xml");
        log_inited = true;
    }
}

void web_logmachine::do_log(unafrog::log::LogLevel level, const std::string &message) noexcept
{
    static std::map<unafrog::log::LogLevel, ::cxxtools::Logger::log_level_type> levelMap{
        {unafrog::log::LogLevel::LOG_EMERGENCY, ::cxxtools::Logger::LOG_LEVEL_FATAL},
        {unafrog::log::LogLevel::LOG_CRITICAL, ::cxxtools::Logger::LOG_LEVEL_FATAL},
        {unafrog::log::LogLevel::LOG_ERROR, ::cxxtools::Logger::LOG_LEVEL_ERROR},
        {unafrog::log::LogLevel::LOG_WARNING, ::cxxtools::Logger::LOG_LEVEL_WARN},
        {unafrog::log::LogLevel::LOG_INFORMATION, ::cxxtools::Logger::LOG_LEVEL_INFO},
        {unafrog::log::LogLevel::LOG_DEBUG, ::cxxtools::Logger::LOG_LEVEL_DEBUG},
        {unafrog::log::LogLevel::LOG_TRACE, ::cxxtools::Logger::LOG_LEVEL_TRACE}
    };

    ::do_log(levelMap[level], message);
}
