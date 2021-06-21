#ifndef WEB_COMPONENT_H
#define WEB_COMPONENT_H

#include <tnt/httprequest.h>
#include <tnt/httpreply.h>
#include <tnt/mimedb.h>

#include "template_struct.h"

static const std::string RECIPES_ROOT = "/recipes/";
static const std::string path = "/theme/current/";

class web_component
{
public:

    // generic not found page
    static const std::string WEB_NOT_FOUND;

    web_component(tnt::HttpRequest& request, tnt::HttpReply& reply, const std::string& sessionId);
    virtual ~web_component() = default;

    virtual unsigned send() = 0;

    static tnt::MimeDb mimeDb;

	std::string prepareLangJs(const std::map<std::string, std::map<std::string, std::string> > &translations);

protected:

	void prepareLanguages();

    tnt::HttpRequest& mrequest;
    tnt::HttpReply& mreply;

    std::string m_sessionId;
    std::string m_originatingIp;

	std::vector<template_struct> m_languageStructs;

};

#endif // WEB_COMPONENT_H
