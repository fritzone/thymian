#include "config.h"
#include <log.h>

#include <iostream>

config::config() : flood(), server(), m_valid(true)
{
}

config &config::instance()
{
    static config i;
    return i;
}
