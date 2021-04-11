#ifndef MAIN_SENDER_H
#define MAIN_SENDER_H

#include "web_component.h"

#include <string>

static const std::string theme_path = "/theme/current/";

/* just sends a file */
class main_sender final : public web_component
{
public:

    main_sender(tnt::HttpRequest& request, tnt::HttpReply& reply, const std::string& what);
    unsigned send() final;
private:
    std::string m_translated;
};

#endif
