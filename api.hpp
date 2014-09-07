#ifndef __API_HPP
#define __API_HPP

#include <iostream>
#include <string>
#include <cmath>
#include <restclient-cpp/restclient.h>
#include <picojson.h>

using namespace std;

namespace mhacks
{
    const string API_ROOT = "http://wirelessturnstile.mybluemix.net";
    RestClient::response send_delta(int delta, string roomId);
    RestClient::response register_room(string roomName);
}

#endif
