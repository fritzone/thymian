#include "web_component.h"
#include "flood_check.h"
#include "log.h"
#include "fake_locations.h"
#include "dictionary.h"

#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <thread>
#include <chrono>
#include <map>

tnt::MimeDb web_component::mimeDb = tnt::MimeDb("/etc/mime.types");

const std::string web_component::WEB_NOT_FOUND = "/static/404.html";
struct SleepRecommendation
{
    int attempt = 0;
    int sleep_time = 0;
};

static std::map<std::string, SleepRecommendation> sleep_times;

web_component::web_component(tnt::HttpRequest &request, tnt::HttpReply &reply, const std::string &sessionId)
    : mrequest(request), mreply(reply), m_sessionId(sessionId)
{
    // see that this IP is not trying to flood our system
    m_originatingIp = "";

    std::string originatingIp = request.getHeader("X-Forwarded-For:");
    info() << "got originating IP: " << originatingIp;

    /*auto hit = request.header_begin();
    debug() << "Full header:";
    while(hit != request.header_end())
    {
        if(boost::starts_with(hit->first, "X-Forwarded-For"))
        {
            originatingIp = hit->first;
            m_originatingIp = originatingIp;

        }
        debug() << hit->first << " -- " << hit->second;
        hit ++;
    }*/

    if(originatingIp.empty())
    {
        originatingIp = request.getPeerIp();
    }

    m_originatingIp = originatingIp;

    // firstly see the fake URLs
    std::string requestedUri = request.getPathInfo();
    if(std::find(fake_locations.begin(), fake_locations.end(), requestedUri) != fake_locations.end() || requestedUri.find(".php") != std::string::npos)
    {
        if(sleep_times.count(m_originatingIp) == 0)
        {
            sleep_times[m_originatingIp] = {1, 0};
        }
        else
        {
            sleep_times[m_originatingIp].attempt ++;
        }

        // with every 50 fake request attempts increase the sleep time with 1 second
        if(sleep_times[m_originatingIp].attempt > 50)
        {
            sleep_times[m_originatingIp].sleep_time ++;
            sleep_times[m_originatingIp].attempt = 0;
        }

        if(sleep_times[m_originatingIp].sleep_time > 0)
        {
            std::this_thread::sleep_for(std::chrono::seconds(sleep_times[m_originatingIp].sleep_time));
        }

        throw "fake";
    }

    // this will throw if sees a flooding attempt
    flood_check::attempt(originatingIp);

}

size_t replace(std::string& str, const std::string& from, const std::string& to, size_t pos)
{
	size_t start_pos = str.find(from, pos);
	if(start_pos == std::string::npos)
		return std::string::npos;
	str.replace(start_pos, from.length(), to);
	return start_pos + to.length();
}

std::string web_component::prepareLangJs(const std::map<std::string, std::map<std::string, std::string> > &translations)
{
	std::map<std::string, std::map<std::string, std::string>> languageToSpanIdTranslations;

	for(const auto& [spanId, languageMap] : translations)
	{
		for(const auto& [langId, translated] : languageMap)
		{
			languageToSpanIdTranslations[langId][spanId] = translated;
		}
	}

	std::string javascript = "function changeTexts(l) {";

	for(const auto& [lang, langMap] : languageToSpanIdTranslations)
	{
		javascript += std::string("if(l == '") + lang + "') {";
		for(const auto& [spanId, translated] : langMap)
		{
			std::string translated_c = translated;
			size_t pos = 0;
			while(pos != std::string::npos) pos = replace(translated_c, "'", "\\'", pos);
			javascript += "$('#" + spanId + "').html('" + translated_c + "');\n";
		}
		javascript += "}";
	}


	javascript += "}\n";

	return javascript;
}

void web_component::prepareLanguages()
{
	// populate the supported languages
	for(const auto& supportedLang : dictionary::supported_languages)
	{
		template_struct language("supported_language", "supported_language_item");
		language["code"] = supportedLang;
		m_languageStructs.push_back(language);
	}

}
