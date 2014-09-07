#include <iostream>
#include "api.hpp"

using namespace std;

int main(int argc, char **argv)
{
    string roomId("my room");
    cout << "registering a room" << endl;
    RestClient::response r = mhacks::register_room(roomId);
    if (r.code != 200)
    {
        cerr << "Failed" << endl;
        cerr << r.body << endl;
        return 1;
    }
    cout << r.body << endl;

    cout << "Sending a delta of +2" << endl;

    r = mhacks::send_delta(2, roomId);
    if (r.code != 200)
    {
        cerr << "Failed" << endl;
        cerr << r.body << endl;
        return 1;
    }
    cout << r.body << endl;
    
    cout << "Sending a delta of -1" << endl;
    r = mhacks::send_delta(-1, roomId);
    if (r.code != 200)
    {
        cerr << "Failed" << endl;
        cerr << r.body << endl;
        return 1;
    }

    cout << r.body << endl;

    return 0;
}
