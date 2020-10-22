#include "templater.h"

#include <algorithm>
#include <iostream>

#define BOOST_FILESYSTEM_NO_DEPRECATED
#define BOOST_FILESYSTEM_VERSION 3

#include <iostream>
#include "boost/filesystem.hpp"

using namespace std;
using namespace boost::filesystem;

int main(int argc, char *argv[])
{
    // list all files in current directory.
    //You could put any file path in here, e.g. "/home/me/mwah" to list that directory
    path p (TEMPLATES_DIRECTORY);

    directory_iterator end_itr;

    // cycle through the directory
    for (directory_iterator itr(p); itr != end_itr; ++itr)
    {
        // If it's not a directory, list it. If you want to list directories too, just remove this check.
        if (is_regular_file(itr->path())) {
            // assign current file name to current_file and echo it out to the console.
            string current_file = itr->path().string();
            cout << current_file << endl;
        }
    }
}
