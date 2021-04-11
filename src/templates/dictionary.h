#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <string>

class dictionary
{
public:
    static std::string translate(const std::string& what, const std::string& target_language);
};

#endif // DICTIONARY_H
