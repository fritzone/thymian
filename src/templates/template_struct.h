#ifndef TEMPLATE_STRUCT_H
#define TEMPLATE_STRUCT_H

#include <string>
#include <vector>
#include <map>

/**
 * @brief The template_struct class represents a logical structure that can be inserted into the
 * template's content in order to have a logical grouping of various data.
 */
class template_struct
{
public:

    template_struct() = default;
    explicit template_struct(const std::string& t) : name("name"), type(t) {}
    template_struct(const std::string& n, const std::string& t) : name(n), type(t) {}

    virtual ~template_struct() = default;

    std::string& operator[] (const char* p)
    {
        return struct_members[std::string(p)];
    }

    const std::string& operator[] (const char* p) const
    {
        return struct_members.at(std::string(p));
    }

    std::string& operator[] (const std::string& p)
    {
        return struct_members[std::string(p)];
    }

    const std::string& operator[] (const std::string& p) const
    {
        return struct_members.at(std::string(p));
    }

    std::vector<std::string> keys() const
    {
        std::vector<std::string> v;
                for(const auto& it : struct_members)
        {
            v.push_back(it.first);
        }
        return v;
    }

    bool has_key(const std::string& k)
    {
                for(const auto& it : struct_members)
        {
            if(it.first == k) return true;
        }
        return false;
    }

    std::string name;
    std::string type;
    std::map<std::string, std::string> struct_members;
};

#endif // TEMPLATE_STRUCT_H
