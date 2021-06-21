#include "main_sender.h"
#include <templater.h>
#include <log.h>
#include <unistd.h>
#include <fstream>

main_sender::main_sender(tnt::HttpRequest& request, tnt::HttpReply& reply, const std::string& what)
    : web_component(request, reply, "")
{
    std::string targetLanguage = what.substr(1);

	prepareLanguages();
	template_vector_par tvp_languages("languages", m_languageStructs);


    auto t = translator<mainpage>();
	m_translated = t.templatize(tvp_languages).set().translate(targetLanguage);
    auto translations = t.get_translations();

    stringholder sh(m_translated);
	sh.replace_all("{#jsfun}", prepareLangJs(translations));
	sh.replace_all("{#divChanger}", "changeDivs('" + targetLanguage + "');");
    m_translated = sh.get();

}

unsigned main_sender::send()
{
    mreply.out() << m_translated;
    return HTTP_OK;
}
