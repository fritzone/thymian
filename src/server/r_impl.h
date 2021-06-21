#ifndef R_SENDER_H
#define R_SENDER_H

#include "web_component.h"

#include <string>

/* just sends a recipe */
class r_impl final : public web_component
{
public:

	r_impl(tnt::HttpRequest& request, tnt::HttpReply& reply, const std::string& what);
	unsigned send() final;

private:

	std::string m_lang;
	std::string m_intro;
	std::string m_body;
	std::string m_key;

	std::string getMd(const std::string &key, const std::string& f, const std::string &cCurrentPath);
};

#endif
