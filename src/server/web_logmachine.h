#ifndef WEB_LOGMACHINE_H
#define WEB_LOGMACHINE_H

#include "log_machine.h"

class web_logmachine : public unafrog::log::log_machine
{
public:
    web_logmachine();
    ~web_logmachine() final = default;
    void do_log(unafrog::log::LogLevel level, const std::string& message) noexcept override;
};

#endif // WEB_LOGMACHINE_H
