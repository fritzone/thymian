#ifndef LOGSTREAMHELPER_H_
#define LOGSTREAMHELPER_H_

#include "logtypes.h"

#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif

#define emergency()     logstream(__LINE__, __FILE__, __PRETTY_FUNCTION__, unafrog::log::LogLevel::LOG_EMERGENCY)
#define log_critical()  logstream(__LINE__, __FILE__, __PRETTY_FUNCTION__, unafrog::log::LogLevel::LOG_CRITICAL)
#define log_err()       logstream(__LINE__, __FILE__, __PRETTY_FUNCTION__, unafrog::log::LogLevel::LOG_ERROR)
#define log_warning()   logstream(__LINE__, __FILE__, __PRETTY_FUNCTION__, unafrog::log::LogLevel::LOG_WARNING)
#define info()          logstream(__LINE__, __FILE__, __PRETTY_FUNCTION__, unafrog::log::LogLevel::LOG_INFORMATION)
#define debug()         logstream(__LINE__, __FILE__, __PRETTY_FUNCTION__, unafrog::log::LogLevel::LOG_DEBUG)
#define trace()         logstream(__LINE__, __FILE__, __PRETTY_FUNCTION__, unafrog::log::LogLevel::LOG_TRACE)
#define log_ex(ex)      log_err() << ex.what() << ". "

#endif  // LOGSTREAMHELPER_H
