#ifndef CATEGORY_SENDER_H
#define CATEGORY_SENDER_H

#include "web_component.h"
#include "templater.h"

#include <string>

/* just sends a file */
class category_sender final : public web_component
{
public:

	category_sender(tnt::HttpRequest& request, tnt::HttpReply& reply, const std::string& what);
    unsigned send() final;

private:

	bool populate_short_description(const std::string& key) const;
	std::string prepareLangTrs(const std::vector<std::string>&);
private:

    std::string m_translated;
	std::vector<template_struct> m_foodStructs;
	unsigned m_status = HTTP_OK;
	std::string m_lang = "gb";
	std::string m_category = "";
};

#endif

