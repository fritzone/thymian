#include "file_sender.h"
#include <templater.h>
#include <log.h>
#include <unistd.h>
#include <fstream>
#include <boost/algorithm/string.hpp>

file_sender::file_sender(tnt::HttpRequest& request, tnt::HttpReply& reply, const std::string& file)
    : web_component(request, reply, ""), mfile(file)
{}

file_sender &file_sender::templatize()
{
    mneeds_templates = true;
    return *this;
}

static std::string fn = "";

unsigned file_sender::send()
{

    char cCurrentPath[FILENAME_MAX] = {0};

    if (!getcwd(cCurrentPath, sizeof(cCurrentPath)))
    {
        return internalServerError();
    }

    std::string filename(cCurrentPath);
    if(mfile[0] != '/' && filename[filename.length() - 1] != '/')
    {
        filename += '/';
    }
    filename += mfile;

    std::ifstream fin(filename.c_str(), std::ios::in | std::ios::binary);

    if(fin)
    {
        if(mneeds_templates)
        {
            //TODO mutex!
            fn = filename;
            GENERIC_FILE_TEMPLATE(FileSender, fn);
            std::string res = templater<FileSender>().templatize().get();
            mreply.out() << res;
        }
        else
        {
            mreply.setContentType(mimeDb.getMimetype(mfile));
            std::ostringstream oss;
            oss << fin.rdbuf();
            std::string fileData(oss.str());
            mreply.out() << fileData;       // send the file
        }

        return HTTP_OK;
    }
    else
    {
        if(boost::starts_with(mfile, theme_path))
        {
            return HTTP_NOT_FOUND;
        }

        // see if the file is actually from the current theme
        unsigned current_theme_file_send = file_sender(mrequest, mreply, theme_path + mfile).send();
        return current_theme_file_send;
    }
}
