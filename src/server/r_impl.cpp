#include "r_impl.h"
#include "templater.h"
#include "url_breaker.h"
#include "logger.h"
#include "json.h"
#include <dictionary.h>
#include "maddy/parser.h"

#include <boost/algorithm/string.hpp>
#include <tntdb.h>

std::string r_impl::getMd(const std::string& key, const std::string &f, const std::string& cCurrentPath)
{
	std::string filenameDesc(cCurrentPath);
	filenameDesc += RECIPES_ROOT + key + "/" + m_lang + "/" + f;

	std::ifstream finIntro(filenameDesc.c_str(), std::ios::in | std::ios::binary);
	if(finIntro)
	{
		std::ostringstream ossIntro;
		ossIntro << finIntro.rdbuf();
		std::string intro(ossIntro.str());
		std::stringstream markdownInput(intro);

		// config is optional
		std::shared_ptr<maddy::ParserConfig> config = std::make_shared<maddy::ParserConfig>();
		config->isEmphasizedParserEnabled = true; // default
		config->isHTMLWrappedInParagraph = true; // default

		std::shared_ptr<maddy::Parser> parser = std::make_shared<maddy::Parser>(config);
		return parser->Parse(markdownInput);
	}

	return "";
}

r_impl::r_impl(tnt::HttpRequest &request, tnt::HttpReply &reply, const std::string &what) : web_component(request, reply, "")
{
	url_breaker r("r/key", boost::to_lower_copy(what));
	m_key = r["key"];

	// identify the language
	auto pars = request.getQueryParams();
	m_lang = pars.arg<std::string>("l");

	char cCurrentPath[FILENAME_MAX] = {0};

	if (!getcwd(cCurrentPath, sizeof(cCurrentPath)))
	{
		log_err() << "Cannot get current path";
		return;
	}

	std::string filename(cCurrentPath);
	filename += RECIPES_ROOT + "/" + m_key + "/descriptor.json";
	bool found_lang = false;

	{
	std::ifstream fin(filename.c_str(), std::ios::in | std::ios::binary);
	if(fin)
	{
		std::ostringstream oss;
		oss << fin.rdbuf();
		std::string fileData(oss.str());
		auto j = nlohmann::json::parse(fileData);
		auto langs = j["lang"];
		for(const auto& l : langs)
		{
			std::string cpp_string;
			l.get_to(cpp_string);
			if(cpp_string == m_lang)
			{
				found_lang = true;
				break;
			}
		}
	}
	}
	if(!found_lang)
	{
		log_err() << "Cannot find the language " << m_lang << " for " << m_key;
		return;
	}

	m_intro = getMd(m_key, "intro.md", cCurrentPath);
	m_body = getMd(m_key, "recipe.md", cCurrentPath);

	// fix the image, if it contains {#rroot}
	stringholder sh(m_body);
	sh.replace_all("{#rroot}", RECIPES_ROOT + m_key + SLASH);
	sh.replace_all("//", "/");
	m_body = sh.get();

	m_body = "<img src='" + RECIPES_ROOT + m_key + SLASH + "img/main.jpg'>" + m_body;

}

unsigned r_impl::send()
{
	std::string stype = "";
	try {

		tntdb::Connection conn = tntdb::connect("sqlite:lang.db");

		std::string v = std::string("select type from food where food_key='") + m_key + "'";
		tntdb::Result result = conn.select(v);
		for (tntdb::Result::const_iterator it = result.begin(); it != result.end(); ++it)
		{
			std::string type;
			std::string key;
			tntdb::Row row = *it;

			row[0].get(type);

			if(ctg_to_name.count(type))
			{
				stype = ctg_to_name[type];
			}
			else
			{
				return HTTP_NOT_FOUND;
			}
		}
	}
	catch (std::exception& ex)
	{
		log_critical() << ex.what();
		return HTTP_NOT_FOUND;
	}

	prepareLanguages();
	template_vector_par tvp_languages("languages", m_languageStructs);

	std::string title = dictionary::translate(m_key + "_name", m_lang);
	std::string rply = templater<recipe>().templatize("intro" <is> m_intro,
													  "body" <is> m_body,
													  "title" <is> title,
													  "key" <is> m_key,
													  "lng" <is> m_lang,
													  "category" <is> stype).templatize(tvp_languages).get();

	std::map<std::string, std::map<std::string, std::string> > translations;
	mreply.out() << translator<void>::translate(rply, m_lang, translations);


	return HTTP_OK;
}
