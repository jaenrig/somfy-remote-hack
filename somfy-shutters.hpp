
#ifndef SOMFY_SHUTTERS_HPP
#define SOMFY_SHUTTERS_HPP

#include "somfy-remote.hpp"

class SomfyShutters
{
    public:

        typedef uint8_t open_percentage_t;

        SomfyShutters(SomfyRemote & remote, SomfyRemote::select_group_t id, String name);
        ~SomfyShutters();

        void open(uint8_t open_percentage = 100);
        void close(uint8_t close_percentage = 100) { open(100 - close_percentage); };
        void my();
        
        void register_event();
        void deregister_event();
        ShuttersEvent * is_event_registered(/* Date Time */);

    private:
        //time per 5% openess
        //time from 0 to 100% open (or close)
        //id_in_remote
        SomfyRemote &remote;
        String name;
        SomfyRemote::select_group_t remote_select_position;
        ShuttersEvent events[5];
};

#endif /* SOMFY_SHUTTERS_HPP */