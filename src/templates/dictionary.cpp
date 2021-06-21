#include "dictionary.h"

#include <tntdb.h>
#include <boost/algorithm/string.hpp>

const std::vector<std::string> dictionary::supported_languages = {"gb", "no"};
std::map<std::string, std::map<std::string, std::string>> dictionary::m_inMemoryTranslations;

template<typename T>
std::string join(const std::vector<T> in,
                 const std::string & separator =", ",  // see 1.
                 const std::string & concluder =" ")    // see 1.
{
    std::ostringstream ss;
    std::size_t i = 0;
    for(; i<in.size() - 1; i++)
    {
        ss << in[i] << separator;
    }

    ss << in[i] << concluder;
    return ss.str();
}

std::string dictionary::translate(const std::string & what, const std::string &target_language, bool other_languages_too, std::map<std::string, std::string> &translations)
{
    std::string result_translation;
	bool found = false;

    try {
        tntdb::Connection conn = tntdb::connect("sqlite:lang.db");

        // TODO: prepared statement

        std::string v = "select " + join(supported_languages) + " from translations where source='" + what + "'";
        tntdb::Result result = conn.select(v);
        for (tntdb::Result::const_iterator it = result.begin(); it != result.end(); ++it)
        {
            tntdb::Row row = *it;

            for(size_t i=0; i<row.size(); i++)
            {
                std::string row_name = boost::to_lower_copy(row.getName(i));

                if(other_languages_too)
                {
                    std::string temp;
                    row[i].get(temp);
                    translations[row_name] = temp;
                }

                if(row_name == boost::to_lower_copy(target_language))
                {
                    row[i].get(result_translation);
					found = true;
                }
            }
        }

    }
    catch (std::exception& ex)
    {
        std::cerr << ex.what();
		return "";
    }

	if(!found)
	{
		if(m_inMemoryTranslations.count(what))
		{
			for(const auto& l : supported_languages)
			{
				if(m_inMemoryTranslations[what].count(l))
				{
					translations[l] = m_inMemoryTranslations[what][l];
				}
			}
			if(m_inMemoryTranslations[what].count(target_language))
			{
				result_translation = m_inMemoryTranslations[what][target_language];
			}
		}
	}

	return result_translation;
}

std::string dictionary::translate(const std::string &what, const std::string &target_language)
{
	std::map<std::string, std::string> translations;
	return translate(what, target_language, false, translations);
}

void dictionary::add_translation(const std::string &key, const std::string &language, const std::string &translated)
{
	m_inMemoryTranslations[key][language] = translated;
}
