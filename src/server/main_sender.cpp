#include "main_sender.h"
#include <templater.h>
#include <log.h>
#include <unistd.h>
#include <fstream>
#include <boost/algorithm/string.hpp>

main_sender::main_sender(tnt::HttpRequest& request, tnt::HttpReply& reply, const std::string& what)
    : web_component(request, reply, "")
{
    std::string targetLanguage = what.substr(1);
    m_translated = translator<mainpage>().translate(targetLanguage);
}

unsigned main_sender::send()
{
    mreply.out() << m_translated;
    return HTTP_OK;
}
