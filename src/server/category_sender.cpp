#include "category_sender.h"
#include "templater.h"
#include "url_breaker.h"
#include "logger.h"
#include "json.h"
#include <dictionary.h>

#include <boost/algorithm/string.hpp>
#include <tntdb.h>

category_sender::category_sender(tnt::HttpRequest &request, tnt::HttpReply &reply, const std::string &what) : web_component(request, reply, "")
{

	// identify the category
	static std::map<std::string, std::string> name_to_dbtype = { {"soups", "0"}, {"sides", "2"}, {"mains", "3"}, {"sweets", "4"}, {"starters", "1"} };
	static std::map<std::string, std::string> ctg_to_name = { {"0", "Soups"}, {"2", "Side Dishes"}, {"3", "Main Courses"}, {"4", "Sweets"}, {"1", "Appetizers"} };

	url_breaker r("l/category", boost::to_lower_copy(what));
	std::string food_type = r["category"];
	if(name_to_dbtype.find(food_type) == name_to_dbtype.end())
	{
		log_critical() << "Not found:" << what;
		m_status = HTTP_NOT_FOUND;
		return;
	}
	m_category = food_type; m_category[0] = std::toupper(m_category[0]);
	food_type = name_to_dbtype[food_type];


	// identify the language
	auto pars = request.getQueryParams();
	m_lang = pars.arg<std::string>("l");

	// generate the page elements as translateable
	try {
		tntdb::Connection conn = tntdb::connect("sqlite:lang.db");

		std::string v = std::string("select * from food where type='") + food_type + "'";
		tntdb::Result result = conn.select(v);
		std::vector<std::string> keys;
		for (tntdb::Result::const_iterator it = result.begin(); it != result.end(); ++it)
		{
			std::string name_src = "";
			std::string img = "";
			int food_idx = -1;
			std::string type;
			std::string key;

			tntdb::Row row = *it;

			row[0].get(food_idx);
			row[1].get(name_src);
			row[2].get(type);
			row[3].get(img);
			row[4].get(key);

			template_struct food("foods", "fooditem");

			food["name"] = "<!--#translate " + name_src + "#-->";

			// fix the image, if it contains {#rroot}
			stringholder sh(img);
			sh.replace_all("{#rroot}", RECIPES_ROOT + key + SLASH);
			sh.replace_all("//", "/");

			food["img"] =  sh.get();
			food["desc"] =  "<!--#translate " + key + "_desc#-->";
			food["key"] = key;

			keys.push_back(key);

			populate_short_description(key);

			m_foodStructs.push_back(food);
		}

		prepareLanguages();

		template_vector_par tvp_foods("foods", m_foodStructs);
		template_vector_par tvp_languages("languages", m_languageStructs);

		std::string page = templater<category_list>().templatize("category" <is> ctg_to_name[food_type], "lang" <is> m_lang).templatize(tvp_languages).templatize(tvp_foods).get();

		std::map<std::string, std::map<std::string, std::string> > translations;
		m_translated = translator<void>::translate(page, m_lang, translations);

		stringholder sh(m_translated);
		sh.replace_all("{#jsfun}", prepareLangJs(translations));
		sh.replace_all("{#trjsfun}", prepareLangTrs(keys));
		sh.replace_all("{#divChanger}", "changeDivs('" + m_lang + "');");
		m_translated = sh.get();


	}
	catch (std::exception& ex)
	{
		log_critical() << ex.what();
		m_status = HTTP_NOT_FOUND;
	}
}

unsigned category_sender::send()
{
	mreply.out() << m_translated;
	return HTTP_OK;
}

bool category_sender::populate_short_description(const std::string &key) const
{
	char cCurrentPath[FILENAME_MAX] = {0};

	if (!getcwd(cCurrentPath, sizeof(cCurrentPath)))
	{
		log_err() << "Cannot get current path";
		return false;
	}

	std::string filename(cCurrentPath);
	filename += RECIPES_ROOT + "/" + key + "/descriptor.json";
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
			std::string filenameDesc(cCurrentPath);
			std::string cpp_string;
			l.get_to(cpp_string);

			filenameDesc += RECIPES_ROOT + key + "/" + cpp_string+ "/descr.md";
			std::ifstream finDesc(filenameDesc.c_str(), std::ios::in | std::ios::binary);
			if(finDesc)
			{
				std::ostringstream ossDesc;
				ossDesc << finDesc.rdbuf();
				std::string fileDataDesc(ossDesc.str());

				dictionary::add_translation(key + "_desc", cpp_string, fileDataDesc);
			}
		}
	}
	else
	{
		log_err() << "Cannot get descriptor file:" << filename;
		return false;
	}

	return true;
}

std::string category_sender::prepareLangTrs(const std::vector<std::string>& keys)
{
	std::string javascript = "function changeTrs(l) {";
	for(const auto &k : keys)
	{
		javascript += "$('#tr_" + k + "').data('href', '/r/" + k + "?l=' + l);\n";
	}

	javascript += "}\n";
	return javascript;
}
