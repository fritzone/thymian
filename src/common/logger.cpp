#include "logger.h"

void unafrog::logger::propagate_log(unafrog::log::LogLevel level, const std::string &message) noexcept
{
    for(const auto& machine : log_machines)
    {
        machine->do_log(level, message);
    }
}

void unafrog::logger::add_log_machine(unafrog::log::log_machine* m)
{
    std::shared_ptr<unafrog::log::log_machine> p(m);
    log_machines.push_back(p);
}
