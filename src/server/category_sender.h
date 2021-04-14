#ifndef CATEGORY_SENDER_H
#define CATEGORY_SENDER_H

#include "web_component.h"
#include "templater.h"

#include <string>

static const std::string path = "/theme/current/";

/* just sends a file */
class category_sender final : public web_component
{
public:

	category_sender(tnt::HttpRequest& request, tnt::HttpReply& reply, const std::string& what);

	~category_sender() = default;

    unsigned send() final;
private:

    std::string m_translated;
	std::vector<template_struct> structs;

};

#endif
