#include "settings.hpp"

bool mhacks::load_settings(string filename, settings& sett)
{
    ifstream in(filename);
    picojson::value sets;
    in >> sets;
    string err = picojson::get_last_error();
    if (!err.empty())
    {
        cerr << "Failed to parse settings file" << endl;
        cerr << err << endl;
        return false;
    }

    auto obj = sets.get<picojson::value::object>();
    sett.roomId = obj["roomId"].get<string>();
    string left = obj["left"].get<string>();
    string right = obj["right"].get<string>();
    if (left == "entry" && right == "exit")
    {
        sett.left = Entry;
        sett.right = Exit;
    }
    else if (left == "exit" && right == "entry")
    {
        sett.left = Exit;
        sett.right = Entry;
    }
    else
    {
        cerr << "Left and right cannot perform the same action!" << endl;
        return false;
    }
    return true;
}

string mhacks::get_default_filename()
{
    passwd* pw = getpwuid(getuid());
    const char* homedir = pw->pw_dir;
    return string(homedir) + "/optical_iq.json";
}
