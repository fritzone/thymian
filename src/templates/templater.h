#ifndef TEMPLATER_H
#define TEMPLATER_H

#include <common.h>
#include <named_operator.hpp>

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

class TemplateBase
{
public:
    virtual ~TemplateBase();
    virtual std::string content() const = 0;
    virtual bool check() const = 0;

};

class FileTemplate : public TemplateBase
{
public:

    FileTemplate() = default;

    virtual std::string fileName() const  = 0;

    virtual std::string content() const
    {
        std::string fn = TEMPLATES_DIRECTORY + fileName();
        std::ifstream ifs(fn);
        std::string s( (std::istreambuf_iterator<char>(ifs) ),
                     (std::istreambuf_iterator<char>()) );
        return s;
    }

    virtual bool check() const;
};


class TemplateWarehouse;
struct TwhDestroyer
{
    TwhDestroyer() = default;

    void operator() (TemplateWarehouse* twh);
};

class TemplateWarehouse : public boost::noncopyable
{
public:
    static TemplateWarehouse& instance();
    virtual ~TemplateWarehouse() = default;

    std::string getTemplateContent(const std::string& templateName);

    bool registerTemplate(const std::string& name, TemplateBase *templateClass);
    void freeTemplates();
    bool checkTemplates();
private:
    TemplateWarehouse();
    std::map <std::string, TemplateBase*> templates;
    TwhDestroyer destroyer;
};

#define GENERIC_TEMPLATE(Class, Type)                                                       \
class Class : public FileTemplate                                                           \
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
    #define REG_VAR(Class) static bool dummy##Class##Type __attribute__((unused)) = TemplateWarehouse::instance().registerTemplate(#Class, new Class)
#else
    #define REG_VAR(Class) static bool dummy##Class##Type = TemplateWarehouse::instance().registerTemplate(#Class, new Class)
#endif

#define STRING_TEMPLATE(Class, s)                                                          \
class Class : public TemplateBase                                                          \
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
class Class : public TemplateBase                                                          \
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
    virtual ~template_pair_base();
};

class template_special_parameter
{
public:
    template_special_parameter() = default;
    virtual ~template_special_parameter();

    template_special_parameter(const std::string& pname,
                               const std::string& ptype,
                               bool piterable) :
        iterable(piterable), name(pname), type(ptype) {}

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

    virtual ~template_struct();

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

template<class T>
std::string stringify(const T& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

template <class T>
class template_par : public template_pair_base
{
public:


    template_par(const char* name, const T& value) : mname(name), moth(stringify(value))
    {
    }

    template_par(const std::string& name, const T& value) : mname(name), moth(stringify(value))
    {
    }

    std::string value() const
    {
        return moth;
    }

    std::string key() const
    {
        return mname;
    }

    void set_value(const std::string& v)
    {
        moth = v;
    }

private:
    template_pair_base* child;
    std::string mname;
    std::string moth;
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
static auto is = base::make_named_operator(op::is());

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
        parameters[name] = {name, type, iterable};
    }

    templater_base& templatize(const template_struct& s);

    templater_base& templatize(const template_vector_par& v);

    virtual std::string get() = 0;
    virtual void do_not_resolve_in_get() = 0;
    virtual void do_resolve_in_get() = 0;

    void skip_whitespace(const std::string &templatized, size_t &i);
    std::string extract_identifier_word(const std::string &input, size_t &i, std::vector<char> separators = {},
                                        std::set<char> extra_allowed_chars = {}, char &&c = 0);
    bool check_opening_parenthesis(std::string input, size_t &i);
    bool check_closing_comment(const std::string &templatized, size_t &i, std::size_t &include_tag_end_position);

protected:

    std::string get(const std::string& template_name);

    std::string resolve_includes(size_t inc_pos, std::string templatized);
    std::string resolve_structure_declaration(size_t struct_pos, std::string templatized);
    std::string resolve_parameters(size_t parameters_pos, std::string templatized);
    std::string resolve_ifs(size_t if_pos, std::string templatized);
    std::string resolve_ifeq(size_t if_pos, std::string templatized);
    std::string resolve_defines(std::string content);
#ifdef HAVE_SCRIPTED_TEMPLATES
    std::string resolve_script(size_t pos, std::string content);
    std::string resolve_scripts(std::string templatized);
#endif
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
    std::map<std::string, template_special_parameter> parameters;

    std::string precalculated = "";

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
                #ifdef HAVE_SCRIPTED_TEMPLATES
                    templatized = resolve_scripts(templatized);
                #endif
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

HTML_TEMPLATE(HomepageRedirect);
HTML_TEMPLATE(AjaxPaneJs);
HTML_TEMPLATE(GenericHeader);
HTML_TEMPLATE(HostsTableHeader);
HTML_TEMPLATE(HostTableRow);
HTML_TEMPLATE(HostTableRowNoHosts);
HTML_TEMPLATE(HostsTableFooter);
HTML_TEMPLATE(PaneContainer);
HTML_TEMPLATE(AlbumList);
HTML_TEMPLATE(AlbumsPage);
HTML_TEMPLATE(FmTableHeader);
HTML_TEMPLATE(FmTableFooter);
HTML_TEMPLATE(RegisterForm);
HTML_TEMPLATE(AlbumContent);
HTML_TEMPLATE(AlbumNotFound);
HTML_TEMPLATE(Captcha);
HTML_TEMPLATE(Failure);
HTML_TEMPLATE(ThankYouHuman);
HTML_TEMPLATE(RegEmail);
HTML_TEMPLATE(LicenseKey);
HTML_TEMPLATE(AlreadyRegistered);
HTML_TEMPLATE(Login);
HTML_TEMPLATE(ThankYouConfirmation);
HTML_TEMPLATE(Dashboard);
HTML_TEMPLATE(HostListCombo);
HTML_TEMPLATE(DrivesListCombo);
HTML_TEMPLATE(FmTableRow);
HTML_TEMPLATE(WarningBox);
HTML_TEMPLATE(OneAlbumImage);
HTML_TEMPLATE(UpperHeader);
HTML_TEMPLATE(LowerHeader);
HTML_TEMPLATE(ShFolderList);
HTML_TEMPLATE(ShfDirContentTable);
HTML_TEMPLATE(DashboardMenu);
HTML_TEMPLATE(ShfPassword);
HTML_TEMPLATE(Styles);
HTML_TEMPLATE(ShfComputerList);
HTML_TEMPLATE(PasswordResetEmail);
HTML_TEMPLATE(Message);
HTML_TEMPLATE(JsInclude);
HTML_TEMPLATE(CssInclude);
HTML_TEMPLATE(Account);
HTML_TEMPLATE(TwoFacConfirmation);
HTML_TEMPLATE(TwoFactorAuthEmail);
HTML_TEMPLATE(NoBackButton);
HTML_TEMPLATE(RefreshSfPage);
CSS_TEMPLATE(AlbPic);
JS_TEMPLATE(login);
JS_TEMPLATE(logout);
TXT_TEMPLATE(user_props);

#endif // TEMPLATER_H
