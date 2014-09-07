#ifndef __SETTINGS_HPP
#define __SETTINGS_HPP

#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <picojson.h>

using namespace std;

namespace mhacks
{
    enum motion { Entry, Exit };
    struct settings
    {
        string roomId;
        motion left;
        motion right;
    };

    bool load_settings(string filename, settings& sett);
    string get_default_filename();
}

#endif
