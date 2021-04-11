#include "dictionary.h"

#include <tntdb.h>

std::string dictionary::translate(const std::string & what, const std::string &target_language)
{

    try {
        tntdb::Connection conn = tntdb::connect("sqlite:lang.db");

        // TODO: prepared statement

        std::string v = "select " + target_language + " from translations where source='" + what + "'";
        tntdb::Result result = conn.select(v);
        for (tntdb::Result::const_iterator it = result.begin(); it != result.end(); ++it)
        {
            tntdb::Row row = *it;

            std::string a;
            row[0].get(a);
            return a;
        }

    }
    catch (std::exception& ex)
    {
        std::cerr << ex.what();
    }
    return "";
}
