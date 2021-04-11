#ifndef LOGGER_H
#define LOGGER_H

#include "log.h"
#include "log_machine.h"

#include <vector>
#include <memory>
#include <string>

namespace unafrog
{
class logger final
{
public:

    static logger& instance()
    {
        static logger the_instance;
        return the_instance;
    }

    void propagate_log(unafrog::log::LogLevel level, const std::string& message) noexcept;

    void add_log_machine(unafrog::log::log_machine*);

private:
    logger()= default;
    logger(logger const&) = delete;
    logger& operator=(logger const&) = delete;

    std::vector<std::shared_ptr<unafrog::log::log_machine>> log_machines;
};
}
#endif // LOGGER_H
