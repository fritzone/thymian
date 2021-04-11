#ifndef WEB_COMPONENT_H
#define WEB_COMPONENT_H

#include <tnt/httprequest.h>
#include <tnt/httpreply.h>
#include <tnt/mimedb.h>


class web_component
{
public:

    // generic not found page
    static const std::string WEB_NOT_FOUND;

    web_component(tnt::HttpRequest& request, tnt::HttpReply& reply, const std::string& sessionId);
    virtual ~web_component() = default;

    virtual unsigned send() = 0;

    static tnt::MimeDb mimeDb;

protected:

    tnt::HttpRequest& mrequest;
    tnt::HttpReply& mreply;

    std::string m_sessionId;
    std::string m_originatingIp;

};

#endif // WEB_COMPONENT_H
