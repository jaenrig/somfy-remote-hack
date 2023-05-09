
#include <Arduino.h>
#include "somfy-conf.hpp"
#include "somfy-remote.hpp"

void remote_test_cmd_line_help()
{
    Serial.println("Available commands: \n");
    Serial.println(" up           Push release UP button");
    Serial.println(" my           Push release MY button");
    Serial.println(" dw           Push release DOWN button");
    Serial.println(" sl[value]    Get/set Select Group");
}

void remote_test_cmd_line()
{
    static const String cmds[] = 
    {
        "up", "my", "dw", "sl"
    };

    typedef enum
    {
        CMD_UP = 0U,
        CMD_MY,
        CMD_DOWN,
        CMD_SEL_G,
        CMD_INVALID = 255U
    }cmd_t;

    while(Serial.available() == 0) {};
    String cmd_input = Serial.readString();

    uint8_t cmd_index;
    for (cmd_index = CMD_UP ; cmd_index < 4; cmd_index++)
    {
        if(cmd_input.substring(0,2) == cmds[cmd_index])
        {
            break;
        }
    }

    SomfyRemote::select_group_t sel_group = SomfyRemote::SELECT_GROUP_UNDEFINED;
    bool sel_group_set = false;
    if(cmd_index == CMD_SEL_G && cmd_input.length() == 3)
    {
        sel_group_set = true;
        sel_group = (SomfyRemote::select_group_t)cmd_input.substring(2).toInt();
        if(sel_group < SomfyRemote::SELECT_GROUP_LED1 || sel_group > SomfyRemote::SELECT_GROUP_LEDSALL)
        {
            cmd_index = CMD_INVALID; 
        }
    }

    switch(cmd_index)
    {
        case CMD_UP:
        Serial.println("up cmd");
        remote.up();
        break;

        case CMD_MY:
        Serial.println("my cmd");
        remote.my();
        break;

        case CMD_DOWN:
        Serial.println("down cmd");
        remote.down();
        break;

        case CMD_SEL_G:
        {
            if(sel_group_set)
            {
                Serial.print("group select cmd : ");
                Serial.println(sel_group);
                remote.select(sel_group);
            }
            else
            {
                Serial.print("group select read cmd : ");
                Serial.println(remote.get_select());
            }
        }
        break;

        default:
            remote_test_cmd_line_help();
        break;
    }
}