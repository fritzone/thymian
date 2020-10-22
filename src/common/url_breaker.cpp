#include "url_breaker.h"

#include "log.h"

#include <boost/tokenizer.hpp>
#include <vector>

url_breaker::url_breaker(const std::string &components, const std::string &url)
{
    std::vector<std::string> tempcomps;
    boost::char_separator<char> sep("/");

    // gather the mapping
    {
    boost::tokenizer<boost::char_separator<char>> tok(components, sep);

    for(boost::tokenizer<boost::char_separator<char>>::iterator beg=tok.begin(); ; )
    {
        tempcomps.push_back(*beg);
        ++ beg;
        if(beg == tok.end())
        {
            break;
        }
    }

    }

    // gather the varios url components
    {
    boost::tokenizer<boost::char_separator<char>> tok(url, sep);
    auto vecit = tempcomps.begin();
    for(boost::tokenizer<boost::char_separator<char>>::iterator beg=tok.begin();; )
    {
        comps[*vecit] = *beg;
        ++vecit;
        ++beg;

        if(vecit == tempcomps.end() && beg == tok.end())
        {
            break;
        }

        if(vecit == tempcomps.end())
        {
            log_err() << "components map" << components << "contains less keys than" << url;
            break;
        }

        if(beg == tok.end())
        {
            log_err() << "components map" << components << "contains more keys than" << url;
            break;
        }
    }

    }

    // debug part
//    for(auto iterator = comps.begin(); iterator != comps.end(); ++ iterator)
//    {
//        debug() << "COMP[" << iterator->first << "] = " << iterator->second;
//    }
}

std::string url_breaker::operator [](const std::string &k)
{
    if(comps.count(k))
    {
        return comps[k];
    }
    log_err() << "components map contains no value for " << k;
    return "";
}
