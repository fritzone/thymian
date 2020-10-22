#ifndef CPPDB_H
#define CPPDB_H

#include <string>
#include <common.h>
#include <tuple>
#include <functional>
#include <type_traits>
#include <algorithm>

#define _O(x) x

// helper macros
#define CONCAT_IMPL( x, y ) x##y
#define MACRO_CONCAT( x, y ) CONCAT_IMPL( x, y )

// basic value retrieval for various column types
struct value_renderer
{
    value_renderer() = default;
	virtual ~value_renderer() = default;
    virtual std::string render(const std::string& t) const { return t; }

    virtual std::string value() const = 0;
    virtual std::string crypted(bool) = 0;
    virtual value_renderer& decrypted(bool) = 0;

};

/**
* Class representing a column
*/
class Column
{
public:
    virtual ~Column() = default;
    virtual std::string name() const = 0;
    virtual std::string realname() const = 0;
    virtual std::string type() const = 0;
    virtual const value_renderer* get_value_renderer() const  = 0;
    virtual std::string extra_modifiers() const = 0;
    virtual std::string crypt_value(const std::string&) const = 0;
    virtual std::string decrypt_value(const std::string&) const = 0;
    virtual Column* clone() const = 0;
};

class BasicColumn : public Column
{
public:
    virtual std::string name() const { return "" ;}
    virtual std::string type() const  { return "" ;}
    virtual std::string extra_modifiers() const { return "";}
};

class NotAColumn : public Column
{
public:
    virtual std::string name() const = 0;
    virtual std::string realname() const = 0;
    virtual std::string type() const { return ""; }
    virtual const value_renderer* get_value_renderer() const { return nullptr; }
    virtual std::string extra_modifiers() const { return ""; }
    virtual std::string crypt_value(const std::string&) const { return ""; }
    virtual std::string decrypt_value(const std::string&) const { return ""; }
    virtual Column* clone() const { return nullptr; }
};

class DescAsColumn : public NotAColumn
{
public:
    typedef DescAsColumn type;
    virtual std::string name() const { return "DESC"; }
    virtual std::string realname() const { return "DESC"; }
};

class AscAsColumn : public NotAColumn
{
public:
    typedef AscAsColumn type;
    virtual std::string name() const { return "ASC"; }
    virtual std::string realname() const { return "ASC"; }
};

#define DESC ,DescAsColumn()
#define ASC  ,AscAsColumn()

class BasicTable
{
public:
    virtual std::string name() const = 0;
    virtual bool is_crypted() const = 0;
    virtual ~BasicTable() = default;
};

struct Functor
{
    Functor(std::string& rstr) : s(rstr) {}
    std::string& s;

    template<typename T>
    void operator()(T& t)
    {
        std::string g = unafrog::utils::to_string(t);
        this->s = g;
    }
};

struct GetNameFunctor
{
    GetNameFunctor(std::string& rstr) : s(rstr) {}
    std::string& s;

    template<typename T>
    void operator()(T& t)
    {
        std::string g = t.name();
        this->s = g;
    }
};

template<std::size_t I = 0, typename FuncT, typename... Tp>
inline typename std::enable_if< I == sizeof...(Tp), void>::type
for_index(std::size_t, std::tuple<Tp...> &, FuncT)
{}

template<std::size_t I = 0, typename FuncT, typename... Tp>
inline typename std::enable_if< I < sizeof...(Tp), void>::type
for_index(int index, std::tuple<Tp...>& t, FuncT f)
{
    if (index == 0) f(std::get<I>(t));
    for_index<I + 1, FuncT, Tp...>(index-1, t, f);
}


template <class ...Vals>
std::tuple<Vals...> valset(Vals... v)
{
	return std::make_tuple(v...);
}

static const char* insert_into = "INSERT OR IGNORE INTO ";

// columnset stuff for insert operations
#include <iostream>
class Columnset
{	
    size_t colcount;
	std::vector<std::string> colnames;
    std::vector<const Column*> cols;
    std::vector<const Column*> mcols;

    std::vector<const value_renderer*> valrends;
    const BasicTable& t;
    mutable std::string values_s;
    mutable std::string s;

public:
    template<class ...Cs>
    Columnset(const BasicTable& bt, Cs&... inputs) : colcount(sizeof...(Cs)), cols{&inputs...}, t(bt), values_s(""),
		s(insert_into + t.name() + "(")
    {
		for (auto c : cols)
		{
			colnames.push_back(c->name());
            valrends.push_back(c->get_value_renderer());
            mcols.push_back(c->clone());
		}
	}

    virtual ~Columnset()
    {
        for (auto c : mcols)
        {
            delete c;
        }
        for (auto c : valrends)
        {
            delete c;
        }
    }

	template <class ...Vals>
    std::string insert(const char* c, Vals... v)
    {
        return std::string(c),  + valset(v...);
    }

    template <class ...Vals>
    std::string insert(Vals... v) const
    {
		auto r = valset(v...);
        for(size_t i=0; i<colcount; i++)
        {
            std::string cn = colnames.at(i);
            s += cn.substr(cn.find('.') + 1);

            // the value from the template arguments, stored as a tuple.
            std::string returned;
            Functor a(returned);
            for_index(i, r, a);
			std::string crypted = mcols.at(i)->crypt_value(returned);
            values_s += valrends.at(i)->render(crypted);
            if(i < cols.size() - 1)
            {
                s+= ", ";
                values_s += ", ";
            }
        }
        s += ") VALUES (" + values_s + ")";
        return s;
    }

    std::string prepare_insert() const
    {
        std::string s = std::string(insert_into) + t.name() + "(";
        std::string values_s = "";

        for(size_t i=0; i<colcount; i++)
        {
            std::string cn = mcols[i]->name();
            s += cn.substr(cn.find('.') + 1);
            values_s += ":v" + std::to_string(i + 1);

            if(i < cols.size() - 1)
            {
                s+= ", ";
                values_s += ", ";
            }
        }
        s += _O(") VALUES (") + values_s + ")";
        return s;
    }

};

/**
 * Class representing a Table
 */
class Table : public BasicTable
{
    mutable std::vector<std::string> foreign_keys;
    std::vector<std::string> inited_foreign_keys;
    std::vector<const Column*> columns;
    std::string resolve_foreign_key(const std::string& s) const;

protected:
    bool init_foreign_key(const std::string& s);

public:
    virtual std::string verify() const;
    virtual std::string create() const;

    virtual std::string name() const = 0;
    virtual std::string realname() const = 0;
    virtual bool is_crypted() const = 0;

    void addColumn(const Column* c) {columns.push_back(c);}
};

class Condition
{
public:
    Condition(const std::string& s);
    std::string cond() const;
private:
    std::string cstr;
};

std::string operator && (const Condition& c1, const Condition& c2);

std::string operator || (const Condition& c1, const Condition& c2);

typedef std::string (*PT)(const Condition& c1, const Condition& c2);

#define AND &&
#define OR ||


template<typename TF>
std::string write_debug_output( TF const& f) {
    std::stringstream ss;
    ss << f.get_modifier();
    return ss.str();
}

struct modifier_retriever {
    template<typename TF, typename ... TR>
    std::string write( TF const& f, TR const& ... rest ) {
        std::string full = write_debug_output( f );
        full += " ";
        full += write( rest... );
        return full;
    }
    template<typename TF>
    std::string write( TF const& f ) {
        return write_debug_output( f );
    }
    std::string write() {
        return "";
    }
};

namespace cppdb
{
std::string crypt_db_name(const std::string& in);
std::string crypt_db_value(const std::string& in, bool do_crypt);
std::string decrypt_db_value(const std::string& in, bool do_decrypt);
unsigned long crypt_number(unsigned long in, bool do_crypt);
unsigned long decrypt_number(unsigned long in, bool do_crypt);
}

#define TABLE_WRAPPER(Name,Crypt) namespace {                       \
    class Table##Name : public Table                                \
    {                                                               \
        mutable std::string crypted_name = "";                      \
    public:                                                         \
        static std::string tablename;                               \
        Table##Name() {}                                            \
        bool is_crypted() const {return Crypt;}                     \
        template<class ...Cs>                                       \
        const Columnset operator() (Cs... cols) const               \
        { return Columnset(*this, cols...);}                        \
        virtual std::string name() const                            \
        {                                                           \
            if(Crypt)                                               \
            {                                                       \
                if(crypted_name.empty())                            \
                    crypted_name = cppdb::crypt_db_name(tablename); \
                return crypted_name;                                \
            }                                                       \
            else return tablename;                                  \
        }                                                           \
        virtual std::string realname() const {return tablename; }

#define TABLE(Name) TABLE_WRAPPER(Name,0)
#define CRYP_TABLE(Name) TABLE_WRAPPER(Name,1)

// Will create the actual "Name" object of type Table##Name
#define ENDTABLE(Name) };                                                                       \
     std::string Table##Name::tablename = #Name;                                            \
     const Table##Name& Name = Table##Name();                                                   \
     bool MACRO_CONCAT(reg_tab_, Name) = cppdb_warehouse::instance().add_table(&Name);          \
     }

#define DEF_OPERATOR(OP,sqlop,Type)                                                                     \
    Condition operator OP (const Type::type& t) const                                                   \
    { return Condition( "(" + name() + #sqlop + unafrog::utils::to_string(t) + ")"); }                                  \
    Condition operator OP (const Column& t) const                                                       \
    { return Condition( "(" + name() + #sqlop + t.name() + ")"); }                                      \


#define COLUMN_WRAPPER(Name, Type, Crypt, ...)                                                          \
    class Column##Name : public BasicColumn                                                             \
    {                                                                                                   \
        Table& parent;                                                                                  \
        mutable std::string crypted_name = "";                                                          \
        std::string extra = modifier_retriever().write(__VA_ARGS__);                                    \
    public:                                                                                             \
        Column##Name (Table& p) : parent(p) {p.addColumn(this);}                                        \
            virtual std::string name() const                                                            \
            {                                                                                           \
                if (Crypt || parent.is_crypted())                                                       \
                {                                                                                       \
                    if(crypted_name.empty())                                                            \
                    {                                                                                   \
                        crypted_name = parent.name() + "." + cppdb::crypt_db_name(#Name);           \
                    }                                                                                   \
                    return crypted_name;                                                                \
                }                                                                                       \
                else return parent.name() + "." + #Name;                                            \
            }                                                                                           \
            virtual std::string realname() const { return #Name; }                                  \
            DEF_OPERATOR(==, =, Type)                                                                   \
            Condition operator == (const std::string& t) const                                          \
            { return Condition( "(" + name() + " = '" + t + "')"); }                                    \
            DEF_OPERATOR(!=, <>, Type)                                                                  \
            DEF_OPERATOR(<=, <=, Type)                                                                  \
            DEF_OPERATOR(>=, >=, Type)                                                                  \
            DEF_OPERATOR(<, <, Type)                                                                    \
            DEF_OPERATOR(>, >, Type)                                                                    \
            std::string type() const { return Type().get_type(); }                                      \
            std::string extra_modifiers() const { return extra; }                                       \
            const value_renderer* get_value_renderer() const { return new Type(); }                     \
            std::string crypt_value(const std::string& value) const                                     \
            {                                                                                           \
                return Type(value).crypted(Crypt || parent.is_crypted());                               \
            }                                                                                           \
            std::string crypt_value(const Type::type& value) const                                      \
            {                                                                                           \
                return Type(value).crypted(Crypt || parent.is_crypted());                               \
            }                                                                                           \
            std::string decrypt_value(const std::string& value) const                                   \
            {                                                                                           \
                return Type(value).decrypted(Crypt || parent.is_crypted()).value();                     \
            }                                                                                           \
            Column* clone() const { Column* c = new Column##Name(parent); dynamic_cast<Column##Name*>(c)->crypted_name = crypted_name; dynamic_cast<Column##Name*>(c)->extra = extra; return c;}\
    };                                                                                                  \
    Column##Name Name = Column##Name(*this);                                                            \
    bool MACRO_CONCAT(reg_col_, Name) = cppdb_warehouse::instance().add_column(&Name)


#define COLUMN(Name, Type, ...) COLUMN_WRAPPER(Name, Type, 0, __VA_ARGS__)
#define CRYP_COLUMN(Name, Type, ...) COLUMN_WRAPPER(Name, Type, 1,  __VA_ARGS__)

// Foreign Key stuff
#define FOREIGN_KEY(X) bool MACRO_CONCAT(g, __COUNTER__ ) = init_foreign_key(std::string(#X))

// Order by

template<typename T, typename U>
struct is_same : std::false_type { };

template<typename T>
struct is_same<T, T> : std::true_type { };

template<typename T, typename U>
constexpr bool eqTypes() { return is_same<T, U>::value; }

std::string orderby_helper(const Column& c);

template <class... args>
std::string orderby_helper(args... a)
{

    std::size_t cnt = sizeof...(args);
    auto r = std::tuple<args...>(a...);
    std::vector<std::string> columns;

    for(std::size_t i=0; i<cnt; i++)
    {
        std::string returned;
        GetNameFunctor a(returned);
        for_index(i, r, a);
        columns.push_back(returned);
    }
    std::string result = "";
    for(size_t i = 0; i<columns.size(); i++)
    {
        result += columns.at(i);
        if(i < columns.size() - 1)
        {
            if(columns.at(i + 1) != "ASC" && columns.at(i + 1) != "DESC")
            {
                result += ", ";
            }
            else
            {
                result += " ";
            }
        }
    }
    return result;
}

template <class... all>
std::string ORDER_BY(all... a)
{
    std::string s = " ORDER BY ";
    std::string g = orderby_helper(a...);
    return s + g;
}

// Where

std::string where_helper(const Condition& c);

template <class C = Condition, class... args>
std::string where_helper(const Condition& c, args... a)
{
    std::string s = c.cond();
    s += from(a...);
    return s;
}

template <class... all>
std::string WHERE(all... a)
{
    std::string s = "WHERE ";
    std::string g = where_helper(a...);
    return s + g;
}

// From

std::string from_helper(const Table& t);

template <class T = Table, class... args>
std::string from_helper(const Table& t, args... a)
{
    std::string s = t.name() + ", ";
    s += from_helper(a...);
    return s;
}

template <class... all>
std::string FROM(all... a)
{
    std::string s = "FROM ";
    std::string g = from_helper(a...);
    return s + g + " ";
}

// Select

std::string select_helper(const Column& c);

template <class C = Column, class... args>
std::string select_helper(const Column& c, args... a)
{
    std::string s = c.name() + ", ";
    s += select_helper(a...);
    return s;
}


template <class... all>
std::string SELECT(all... a)
{
    std::string s = "SELECT ";
    std::string g = select_helper(a...);
    return s + g + " ";
}

// Delete

#define DELETE "DELETE "

// Update

std::string UPDATE(const Table& tab);


std::string set_helper(int total_size, const Column& c);

template <class C = Column, class... args>
std::string set_helper(std::size_t total_size, const Column& c, args... a)
{
    std::string s = c.realname() + "=:v";
    std::stringstream ss;
    ss << total_size - sizeof...(a);
    s += ss.str() + ", ";
    s += set_helper(total_size, a...);
    return s;
}

template <class... all>
std::string SET(all... a)
{
    std::string s = " SET ";
    std::string g = set_helper(sizeof...(a), a...);
    return s + g ;
}

// varchar support
template<size_t SIZE> class varchar : public value_renderer
{
public:
    // this has to be "const char*" otherwise overloading upon const std::string& is not possible in Column classes
    typedef const char* type;

    varchar() : s(SIZE) {}
    varchar(const std::string& v) : s(SIZE), mvalue(v) {}
    operator std::string () const
    {
        return _O("VARCHAR(") + std::to_string(s) + ")";
    }

    std::string get_type() const
    {
        return operator std::string();
    }

    virtual std::string render(const std::string& t) const {return "\"" + t + "\"" ; }

    virtual std::string crypted(bool crypt) {return cppdb::crypt_db_value(mvalue, crypt); }
    virtual value_renderer& decrypted(bool crypt) { mvalue = cppdb::decrypt_db_value(mvalue, crypt); return *this;}
    std::string value() const {return mvalue; }
private:
    size_t s;
    std::string mvalue = "";
};

template <size_t SIZE>
std::basic_ostream<char>& operator << (std::basic_ostream<char>& os, varchar<SIZE> vc)
{
    os << vc.operator std::string();
    return os;
}

#define VARCHAR(S) varchar<S>

// text support
class text: public value_renderer
{
public:
    // this has to be "const char*" otherwise overloading upon const std::string& is not possible in Column classes
    typedef const char* type;

    text() = default;
    text(const std::string& v) : mvalue(v) {}
    operator std::string () const
    {
        return _O("TEXT");
    }

    std::string get_type() const
    {
        return operator std::string();
    }

    virtual std::string render(const std::string& t) const {return "\"" + t + "\"" ; }

    virtual std::string crypted(bool crypt) {return cppdb::crypt_db_value(mvalue, crypt); }
    virtual value_renderer& decrypted(bool crypt) { mvalue = cppdb::decrypt_db_value(mvalue, crypt); return *this;}
    std::string value() const {return mvalue; }
private:
    std::string mvalue = "";
};
std::basic_ostream<char>& operator << (std::basic_ostream<char>& os, const text& tx);

#define TEXT text

// other types support
template <class T> class simple_type : public value_renderer
{
public:
    typedef T type;
    simple_type() = default;
    simple_type(T t) : mt(t) {}
private:
    T mt;
};

template <> class simple_type<time_t> : public value_renderer
{
public:
    typedef int type;
    simple_type() = default;
    simple_type(time_t t) : mt(t) {}
    simple_type(const std::string& sv) : mt(stol(sv)) {}
    std::string get_type() const {return _O("TIMESTAMP"); }
    virtual std::string crypted(bool crypt) {return std::to_string(cppdb::crypt_number(mt, crypt));}
    virtual value_renderer& decrypted(bool crypt) { mt = cppdb::decrypt_number(mt, crypt); return *this; }
    std::string value() const {return std::to_string(mt); }
private:
    time_t mt = 0;
};

template <> class simple_type<int> : public value_renderer
{
public:
    typedef int type;
    simple_type() = default;
    simple_type(int t) : mt(t) {}
    simple_type(const std::string& sv) : mt(stoi(sv)) {}
    std::string get_type() const {return _O("INTEGER"); }
    virtual std::string crypted(bool crypt) {return std::to_string(cppdb::crypt_number(mt, crypt));}
    virtual value_renderer& decrypted(bool crypt) { mt = static_cast<int>(cppdb::decrypt_number(mt, crypt)); return *this; }
    std::string value() const {return std::to_string(mt); }
private:
    int mt = 0;
};

#define INTEGER simple_type<int>
#define TIMESTAMP simple_type<time_t>

struct primary_key { const std::string modifier() {return _O("PRIMARY KEY"); } };
struct not_null { const std::string modifier() {return _O("NOT NULL"); } };

template<class D>
struct defaults
{
    defaults(const D& d) : md(d) {}
    std::string get_modifier() const
    {
        std::string res = _O("DEFAULT ");
        std::stringstream ss; ss << md ; return res + ss.str();
    }
private:
    D md;
};

template <class T> class modifier_templ
{
public:
    modifier_templ(){}
    std::string get_modifier() const {return T().modifier();}
};

static const modifier_templ<primary_key> pk;
static const modifier_templ<not_null> nn;

#define PRIMARY_KEY pk
#define NOT_NULL nn
#define CURRENT_TIMESTAMP _O("CURRENT_TIMESTAMP")
#define DEFAULT(X) defaults<decltype(X)>(X)


/* The actual warehouse keeping all the tables, columns and other elements */
class cppdb_warehouse
{
public:
    static cppdb_warehouse& instance();
    bool add_column(const Column* c);
    bool add_table(const Table* t);
    const Table* table(const std::string& tabname);
    const Column* column(const std::string& colname);
private:
    std::vector<const Column*> columns;
    std::vector<const Table*> tables;
};

#endif // CPPDB
