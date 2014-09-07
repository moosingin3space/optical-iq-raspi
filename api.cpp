#include "api.hpp"

RestClient::response mhacks::send_delta(int delta, string roomId)
{
    // send a post request to API_ROOT/send
    map<string, picojson::value> object {
        {"delta", picojson::value(to_string(delta))}, 
        {"roomId", picojson::value(roomId)}
    };
    
    picojson::value val(object);
    string json = val.serialize();
    string url = API_ROOT + "/add-event";
    RestClient::response r = RestClient::post(url, "text/json", json);
    return r;
}

RestClient::response mhacks::register_room(string roomName)
{
    map<string, picojson::value> object {
        {"roomId", picojson::value(roomName) }
    };
    picojson::value val(object);
    string json = val.serialize();
    string url = API_ROOT + "/roomregistration";
    RestClient::response r = RestClient::post(url, "text/json", json);
    return r;
}

