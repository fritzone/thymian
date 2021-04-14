#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <string>
#include <map>
#include <vector>

class dictionary
{
public:
    static std::string translate(const std::string& what, const std::string& target_language, bool other_languages_too, std::map<std::string, std::string>& translations);

    static const std::vector<std::string> supported_languages;

};

#endif // DICTIONARY_H
