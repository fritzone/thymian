#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <string>
#include <map>
#include <vector>

class dictionary
{
public:
	static std::string translate(const std::string& what, const std::string& target_language, bool other_languages_too, std::map<std::string, std::string>& translations);
	static std::string translate(const std::string& what, const std::string& target_language);

	static void add_translation(const std::string& key, const std::string& language, const std::string& translated);

    static const std::vector<std::string> supported_languages;

	// contains the translations generated from external files (such as recipes)
	// the outer key is the identifier of the text that is to be translated (such as: chicken_soup_desc)
	// and the internal keys are the languages, mapped to the given translation
	static std::map<std::string, std::map<std::string, std::string>> m_inMemoryTranslations;
};

#endif // DICTIONARY_H
