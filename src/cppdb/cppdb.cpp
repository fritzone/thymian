#include "cppdb.h"

std::string operator &&(const Condition &c1, const Condition &c2)
{
    return "(" + c1.cond() + " AND " + c2.cond() + ")";
}

std::string operator ||(const Condition &c1, const Condition &c2)
{
    return "(" + c1.cond() + " NOT " + c2.cond() + ")";
}

std::string cppdb::crypt_db_name(const std::string &in)
{
    return in;
}

std::string cppdb::crypt_db_value(const std::string &in, bool do_crypt)
{

    return in;
}

std::string cppdb::decrypt_db_value(const std::string &in, bool do_decrypt)
{

    return in;
}


unsigned long cppdb::crypt_number(unsigned long in, bool do_crypt)
{

        return in;
}

unsigned long cppdb::decrypt_number(unsigned long in, bool do_crypt)
{

    return in;
}

Condition::Condition(const std::string &s) : cstr(s)
{}

std::string Condition::cond() const
{
    return cstr;
}

// Expects a string: "AddressId -> Address.Id"
std::string Table::resolve_foreign_key(const std::string &s) const
{
    std::string fk = _O("FOREIGN KEY (");
    std::istringstream ss(s);
    std::string token;
    std::vector<std::string> tokens;
    while(std::getline(ss, token, ' '))
    {
        tokens.push_back(token);
    }
    if(tokens.size() != 3 || tokens[1] != "->" || tokens[2].find('.') == std::string::npos) throw ("Invalid FOREIGN_KEY macro call: " + s);
    // tokens[0] is the name of the column. Fetch it from the warehouse
    const Column* col = cppdb_warehouse::instance().column(tokens[0]);
    fk += col->name().substr(col->name().find('.') + 1);
    fk += _O(") REFERENCES ");


    // fktab now is the name of the "other" table. Fetch the final name from the warehouse
    std::string fktab = tokens[2].substr(0, tokens[2].find('.'));
    const Table* tab = cppdb_warehouse::instance().table(fktab);
    // fk column is the name of the "other" table's column. Fetch the final name from the warehouse
    std::string fk_column = tokens[2].substr(tokens[2].find('.') + 1);
    const Column* col2 = cppdb_warehouse::instance().column(fk_column);
    fk += tab->name() + "(" + col2->name().substr(col2->name().find('.') + 1) + ")";
    if(std::find(foreign_keys.begin(), foreign_keys.end(), fk) == foreign_keys.end())
    {
        foreign_keys.push_back(fk);
    }

    return fk;
}

bool Table::init_foreign_key(const std::string& s)
{
    inited_foreign_keys.push_back(s);
    return true;
}

std::string Table::create() const
{
    foreign_keys.clear();
    for(const auto& s : inited_foreign_keys)
    {
        resolve_foreign_key(s);
    }

    std::string result = _O("CREATE TABLE IF NOT EXISTS ") + name() + " (";
    for(size_t i=0; i<columns.size(); i++)
    {
        std::string colName = columns[i]->name();
        colName = colName.substr(name().length() + 1);
        result += colName;
        result += " " + columns[i]->type();
        result += (columns[i]->extra_modifiers().length() > 0? " ": "") + columns[i]->extra_modifiers();
        if(i< columns.size() - 1)
            result += ", ";
    }
    if(!foreign_keys.empty())
    {
        result += ", ";
        for(size_t i=0; i<foreign_keys.size(); i++)
        {
            result += foreign_keys[i];
            if(i< foreign_keys.size() - 1)
                result += ", ";
        }
    }
    result += ")";

    return result;
}

std::string Table::verify() const
{
    std::string result = _O("SELECT name FROM sqlite_master WHERE type='table' AND name='") + name() + "'";
    return result;
}

std::string where_helper(const Condition &c)
{
    return c.cond();
}


std::string from_helper(const Table &t)
{
    return t.name();
}


std::string select_helper(const Column &c)
{
    return c.name();
}

std::string orderby_helper(const Column &c)
{
    return c.name();
}

cppdb_warehouse &cppdb_warehouse::instance()
{
    static cppdb_warehouse i;
    return i;
}

bool cppdb_warehouse::add_column(const Column *c)
{
    columns.push_back(c);
    return true;
}

bool cppdb_warehouse::add_table(const Table *t)
{
    tables.push_back(t);
    return true;
}

const Table *cppdb_warehouse::table(const std::string &tabname)
{
    auto it = std::find_if(tables.begin(), tables.end(), [tabname](const Table* t) {return t->realname() == tabname;});
    if(it != tables.end()) return *it;
    return nullptr;
}

const Column *cppdb_warehouse::column(const std::string &colname)
{
    auto it = std::find_if(columns.begin(), columns.end(), [colname](const Column* t) {return t->realname() == colname;});
    if(it != columns.end()) return *it;
    return nullptr;
}


std::string UPDATE(const Table &tab)
{
    std::string s = _O("UPDATE ") + tab.name();
    return s;
}


std::string set_helper(int total_size, const Column &c)
{
    std::string s = c.realname() + "=:v";
    std::stringstream ss;
    ss << total_size;
    s += ss.str() + " ";
    return s;
}

std::basic_ostream<char> &operator <<(std::basic_ostream<char> &os, const text& tx)
{
    os << tx.operator std::string();
    return os;
}
