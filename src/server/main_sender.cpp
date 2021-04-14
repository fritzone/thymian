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
    auto t = translator<mainpage>();
    m_translated = t.translate(targetLanguage);
    auto translations = t.get_translations();
    std::cout << translations.size();

    // create the JS to change language on the fly

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
            javascript += "$('#" + spanId + "').html('" + translated + "');\n";
        }
        javascript += "}";
    }

    javascript += "}\n";

    stringholder sh(m_translated);
	sh.replace_all("{#jsfun}", javascript);
	sh.replace_all("{#divChanger}", "changeDivs('" + targetLanguage + "');");
    m_translated = sh.get();

}

unsigned main_sender::send()
{
    mreply.out() << m_translated;
    return HTTP_OK;
}
