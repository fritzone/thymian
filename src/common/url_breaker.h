#ifndef URL_BREAKER
#define URL_BREAKER

#include <string>
#include <map>

class url_breaker
{
public:
    url_breaker(const std::string& components, const std::string& url);
    std::string operator [](const std::string&);

private:
    std::map<std::string, std::string> comps;

};

#endif // URL_BREAKER

