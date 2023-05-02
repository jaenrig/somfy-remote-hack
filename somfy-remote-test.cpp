
#include <Arduino.h>
#include "somfy-remote.hpp"

void get_select_test(SomfyRemote & remote)
{
    Serial.println("test::get_select()\n");
    Serial.println("Press the select group button and enter the select group value (1-5).");
    while(Serial.available() == 0) {}

    int user_select_group = Serial.parseInt();
    Serial.print("User select group : ");
    Serial.println(user_select_group);

    SomfyRemote::select_group_t select_group = remote.get_select();
    Serial.print("Read select group : ");
    Serial.println(select_group);

    if(user_select_group == select_group)
    {
        Serial.println(">> PASS");
    } 
    else
    {
        Serial.println(">> FAIL");
    }
}

void somfy_remote_test()
{
    SomfyRemote::button_t button_up =
    { 
        26,
        LOW
    };
    SomfyRemote::button_t button_my = 
    { 
        17,
        LOW
    };
    SomfyRemote::button_t button_down = 
    { 
        16,
        LOW
    };
    SomfyRemote::button_t button_select = 
    { 
        27, 
        LOW
    };
    SomfyRemote::button_t button_sel_monitor =
    {
        32,
        LOW
    };

    SomfyRemote::button_set_t buttons = 
    {
        button_up,
        button_my,
        button_down,
        button_select,
        button_sel_monitor
    };

    SomfyRemote remote(buttons);

    get_select_test(remote);
    get_select_test(remote);
    get_select_test(remote);

}