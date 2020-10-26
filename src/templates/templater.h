#ifndef TEMPLATER_H
#define TEMPLATER_H

#include <common.h>
#include <named_operator.hpp>
#include <json.h>
#include <boost/noncopyable.hpp>

#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <string>
#include <map>
#include <fstream>
#include <memory>

#include <sys/types.h>
#include <sys/stat.h>

#ifndef _WIN32
#include <unistd.h>
#endif

static const std::string TEMPLATES_DIRECTORY    = "templates/";

/**
 * @brief The template_base class is the basic class for all the templates
 */
class template_base
{
public:
    virtual ~template_base() = default;
    virtual std::string content() const = 0;
    virtual bool check() const = 0;

};

/**
 * @brief The file_template class is a class for a template that resides in a file
 */
class file_template : public template_base
{
public:

    file_template() = default;

    virtual std::string fileName() const  = 0;

    virtual std::string content() const;
    virtual bool check() const;
};

/**
 * @brief The template_warehouse class is the class that keeps track of the templates that were regisdtered
 */
class template_warehouse : public boost::noncopyable
{
public:
    static template_warehouse& instance();

    std::string getTemplateContent(const std::string& templateName);
    bool registerTemplate(const std::string& name, template_base *templateClass);
    bool checkTemplates();

private:
    template_warehouse() = default;
    std::map <std::string, std::shared_ptr<template_base>> templates;
};

#define GENERIC_TEMPLATE(Class, Type)                                                       \
class Class : public file_template                                                          \
{                                                                                           \
public:                                                                                     \
    Class() = default;                                                                      \
    static std::string name() {return std::string(#Class); }                                \
    virtual std::string fileName() const {return std::string(#Class) + "." + #Type; }       \
};                                                                                          \
static bool dummy##Class##Type = TemplateWarehouse::instance().registerTemplate(#Class, new Class)

#define CSS_TEMPLATE(Class)     GENERIC_TEMPLATE(Class, css)
#define JS_TEMPLATE(Class)      GENERIC_TEMPLATE(Class, js)
#define TXT_TEMPLATE(Class)     GENERIC_TEMPLATE(Class, txt)
#define HTML_TEMPLATE(Class)    GENERIC_TEMPLATE(Class, html)

#if (defined(__GNUC__) || defined(__GNUG__)) && !(defined(__clang__))
    #define REG_VAR(Class) [[maybe_unused]] static bool dummy##Class##Type __attribute__((unused)) = template_warehouse::instance().registerTemplate(#Class, new Class)
#else
    #define REG_VAR(Class) [[gnu::unused]] static bool dummy##Class##Type = template_warehouse::instance().registerTemplate(#Class, new Class)
#endif

#define STRING_TEMPLATE(Class, s)                                                          \
class Class : public template_base                                                         \
{                                                                                          \
public:                                                                                    \
    Class() : m_content(s) {}                                                              \
    virtual ~Class() = default;                                                            \
    static std::string name() {return std::string(#Class); }                               \
    virtual std::string content() const                                                    \
    {                                                                                      \
        return m_content;                                                                  \
    }                                                                                      \
    virtual bool check() const { return true; }                                            \
private:                                                                                   \
    std::string m_content;                                                                 \
};                                                                                         \
REG_VAR(Class)

#define STRING_VAR_TEMPLATE(Class, s)                                                      \
static std::string _TS_ ## _Class = s; \
class Class : public template_base                                                          \
{                                                                                          \
public:                                                                                    \
    Class() : m_content(_TS_ ## _Class) {}                                                 \
    virtual ~Class() = default;                                                            \
    static std::string name() {return std::string(#Class); }                               \
    virtual std::string content() const                                                    \
    {                                                                                      \
        return m_content;                                                                  \
    }                                                                                      \
    virtual bool check() const { return true; }                                            \
private:                                                                                   \
    std::string m_content;                                                                 \
};                                                                                         \
REG_VAR(Class)

class template_pair_base
{
public:
    virtual ~template_pair_base() = default;
};

class template_special_parameter
{
public:
    template_special_parameter() = default;
    virtual ~template_special_parameter() = default;

    template_special_parameter(const std::string& pname,
                               const std::string& ptype,
                               bool piterable) :
        iterable(piterable), name(pname), type(ptype) {}

private:
    bool iterable;
    std::string name;
    std::string type;
};

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
        for(auto it : struct_members)
        {
            v.push_back(it.first);
        }
        return v;
    }

    bool has_key(const std::string& k)
    {
        for(auto it : struct_members)
        {
            if(it.first == k) return true;
        }
        return false;
    }

    std::string name;
    std::string type;
    std::map<std::string, std::string> struct_members;
};

class stringholder
{
public:

    stringholder(const std::string& d) : str(d)
    {}

    void replaceAll(const std::string& from, const std::string& to);

    std::string get() const
    {
        return str;
    }

    stringholder& templatize(const std::map<std::string, std::string>& m);

private:

    std::string str;
};

template <class T>
class template_par : public template_pair_base
{
public:

    template_par(const std::string& name, const T& value) : m_key(name), m_value(unafrog::utils::to_string(value))
    {
    }

    template_par(const char* name, const T& value) : template_par(std::string(name), value)
    {
    }

    std::string value() const
    {
        return m_value;
    }

    std::string key() const
    {
        return m_key;
    }

    void set_value(const std::string& v)
    {
        m_value = v;
    }

private:
    template_pair_base* child;
    std::string m_key;
    std::string m_value;
};

class template_vector_par : public template_pair_base
{
public:
    template_vector_par() = default;

    template_vector_par(const char* name, const std::vector<template_struct>& value) : mname(name), mvalue(value)
    {
    }

    template_vector_par(const std::string& name, const std::vector<template_struct>& value) : mname(name), mvalue(value)
    {
    }

    const std::vector<template_struct>& value() const
    {
        return mvalue;
    }

    std::string name() const
    {
        return mname;
    }

private:
    std::string mname = "";
    std::vector<template_struct> mvalue = {};
};

namespace op
{
    struct is
    {
        template <typename T>
        template_par<T> operator ()(std::string const& vs, T const& v) const
        {
            return template_par<T>(vs, v);
        }

    };
}
[[maybe_unused]] static auto is = base::make_named_operator(op::is());

/**
 * @brief The templater_base class is the basic class for a templater engine
 */
class templater_base
{

public:
    virtual ~templater_base() = default;

    std::map<std::string, std::string> pairs() const
    {
        return kps;
    }

    void add_structure_decl(const std::string& name, const std::string& type)
    {
        structures[name] = template_struct(name, type);
    }

    template_struct& get_structure(const std::string& name)
    {
        return structures[name];
    }

    void add_parameter(const std::string& name, const std::string& type, bool iterable = false)
    {
        m_parameters[name] = {name, type, iterable};
    }

    templater_base& templatize(const template_struct& s);

    templater_base& templatize(const template_vector_par& v);

    virtual std::string get() = 0;
    virtual void do_not_resolve_in_get() = 0;
    virtual void do_resolve_in_get() = 0;

    void skip_whitespace(const std::string &templatized, size_t &i);
    std::string extract_identifier_word(const std::string &input, size_t &i, std::vector<char> separators = {},
                                        std::set<char> extra_allowed_chars = {}, char &&c = 0);
    bool check_opening_parenthesis(const std::string &input, size_t &i);
    bool check_closing_comment(const std::string &templatized, size_t &i, std::size_t &include_tag_end_position);

    template <typename... Args>
    void set_error(Args&&... args)
    {
        std::string terr = unafrog::utils::join_string(' ', std::forward<Args>(args)...);
        error += unafrog::utils::trim(terr) + "\n";

    }

    std::string get_error() const;

    /**
     * @brief variables will return all the variables that can be replaced in the template
     * @return
     */
    std::vector<std::string> variables();

protected:

    std::string get(const std::string& template_name);

    std::string resolve_includes(size_t inc_pos, std::string templatized);
    std::string resolve_structure_declaration(size_t struct_pos, std::string templatized);
    std::string resolve_parameters(size_t parameters_pos, std::string templatized);
    std::string resolve_ifs(size_t if_pos, std::string templatized);
    std::string resolve_ifeq(size_t if_pos, std::string templatized);
    std::string resolve_defines(std::string content);
    std::string resolve_script(size_t pos, const std::string &content);
    std::string resolve_scripts(std::string templatized);
    std::string resolve_loops(std::string templatized, const template_vector_par &v);
    std::string resolve_ifeqs(std::string templatized);
    std::string resolve_dynamic_section(std::string templatized, const template_vector_par &v);

    std::map<std::string, std::string> kps;

    std::string include_tag = "<!--#include";
    std::string if_tag = "<!--#if";
    std::string ifeq_tag = "<!--#eq";
    std::string else_tag = "<!--#else";
    std::string endif_tag = "<!--#endif";
    std::string endifeq_tag = "<!--#endeq";
    std::string struct_tag = "<!--#struct";
    std::string in_tag = "<!--#parameters";
    std::string loop_tag = "<!--#loop";
    std::string endloop_tag = "<!--#endloop";
    std::string define_tag = "<!--#define";
    std::string script_tag = "<!--#script";
    std::string endscript_tag = "<!--#endscript";

    std::map<std::string, template_struct> structures;
    std::map<std::string, template_special_parameter> m_parameters;

    std::string precalculated = "";

    std::string error = "";
};

template<class T>
class templater : public templater_base
{
protected:
    bool resolve_in_get = true;
public:

    templater& templatize()
    {
        return *this;
    }

    templater& templatize(const template_vector_par& v)
    {
        return dynamic_cast<templater&>(templater_base::templatize(v));
    }

    template <typename T1>
    templater& templatize(const template_par<T1>& b)
    {
        precalculated = "";
        kps.insert(make_pair(b.key(), unafrog::utils::to_string(b.value())));

        return *this;
    }

    templater& templatize(const nlohmann::json& j)
    {
        precalculated = "";
        for (const auto& [k, v] : j.items())
        {
            if(v.is_string())
            {
                std::string s = unafrog::utils::to_string(v);
                remove_quotes(s);
                kps.insert(make_pair(unafrog::utils::to_string(k), s));
            }
/*            else if(v.is_object())
            {
                for()
            }*/
        }
        return *this;
    }

    templater& templatize(const template_struct& s)
    {
        return dynamic_cast<templater&>(templater_base::templatize(s));
    }

    template<typename... Args1>
    templater& templatize(const template_struct& s, Args1... args)
    {
        precalculated = "";
        templatize(s);
        return templatize(args...);
    }

    template<typename T1, typename... Args1>
    templater& templatize(const template_par<T1>& first, Args1... args)
    {
        precalculated = "";
        kps.insert(make_pair(first.key(), unafrog::utils::to_string(first.value())));
        return templatize(args...);
    }

    void do_not_resolve_in_get() override
    {
        resolve_in_get = false;
    }
    void do_resolve_in_get() override
    {
        resolve_in_get = true;
    }

    std::string get()
    {
        if(precalculated.empty())
        {
            std::string tname = T::name();
            std::string templatized = templater_base::get(tname);
            if(resolve_in_get)
            {
                templatized = resolve_ifeqs(templatized);
                templatized = resolve_scripts(templatized);
                precalculated = templatized;
            }
            else
            {
                precalculated = templatized;
            }
            return precalculated;
        }
        else
        {
            return precalculated;
        }
    }
};

template <class T>
std::ostream& operator << (std::ostream& os, const templater<T>& t)
{
    os << t.get();
    return os;
}

#endif // TEMPLATER_H
