#ifndef FILE_SENDER_H
#define FILE_SENDER_H

#include "web_component.h"

#include <string>

static const std::string theme_path = "/theme/current/";

/* just sends a file */
class file_sender final : public web_component
{
public:

    file_sender(tnt::HttpRequest& request, tnt::HttpReply& reply, const std::string &file);
    file_sender& templatize();
    unsigned send() final;

private:
    std::string mfile;
    bool mneeds_templates = false;
};

#endif
