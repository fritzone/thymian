#ifndef _LOG_TYPES_H
#define _LOG_TYPES_H

namespace unafrog {
namespace log {

enum class LogLevel
{
    LOG_EMERGENCY   = 1,
    LOG_CRITICAL    = 2,
    LOG_ERROR       = 3,
    LOG_WARNING     = 4,
    LOG_INFORMATION = 5,
    LOG_DEBUG       = 6,
    LOG_TRACE       = 7
};

}}

#endif
