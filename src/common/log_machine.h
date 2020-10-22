#ifndef LOG_MACHINE_H
#define LOG_MACHINE_H

#include "logtypes.h"

#include <string>

namespace unafrog {
namespace log {

/* This is a platform independent log machine abstract class, those deriving from this will need to provide their own log mahcines*/
class log_machine
{
public:

    virtual ~log_machine() = default;

    /**
     * @brief do_log Every log machine deriving from this needs to overrid this method
     *
     * @param level The level of the log
     * @param message The message to go out
     */
    virtual void do_log(unafrog::log::LogLevel level, const std::string& message) noexcept = 0;
};

}}

#endif // LOG_MACHINE_H
