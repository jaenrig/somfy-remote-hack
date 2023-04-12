#include <ArduinoOTA.h>

void led_setup();
void led_blink();

void setup() 
{
  led_setup();
}

void loop() 
{
  led_blink();
}

void led_setup()
{
  pinMode(LED_BUILTIN, OUTPUT);  
}

void led_blink()
{
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
}

void ota_server_setup()
{

}

void ota_server_loop()
{

}

void ntp_client_setup()
{

}

void ntp_client_udpdate_date_time()
{

}

void somfy_hack_setup()
{

}

void somfy_hack_loop()
{
    //for each set of triggers
    //if time match
      //operate to configuration
}

class SomfyRemote
{
    public:

        typedef struct
        {
            uint8_t pin;
            uint8_t pushed_level;
        } button_config_t;

        typedef struct
        {
            button_config_t up;
            button_config_t center;
            button_config_t down;
            button_config_t select;
        } button_config_set_t;

        typedef enum
        {
            SELECT_POS_UNKNOWN = 0,
            SELECT_POS_1,
            SELECT_POS_2,
            SELECT_POS_3,
            SELECT_POS_4,
            SELECT_POS_ALL
        } select_position_t;

        SomfyRemote(button_config_set_t & buttons);
        ~SomfyRemote() { };
        void begin();
        void up();
        void center();
        void down();
        void select(select_position_t select_position);

    private:
      
        button_config_set_t & buttons;
        // select_position_t current_selected_position;
};

SomfyRemote::SomfyRemote(button_config_set_t & buttons):
buttons(buttons)
{
    // select_position_t current_selected_position = SELECT_POS_UNKNOWN;
    pinMode(buttons.up.pin, OUTPUT);
    pinMode(buttons.center.pin, OUTPUT);
    pinMode(buttons.down.pin, OUTPUT);
    pinMode(buttons.select.pin, INPUT);
}

void SomfyRemote::begin()
{
    /* We need to find a way to know 
    which is the current selected 
    remote position  */
}

void SomfyRemote::up()
{
   digitalWrite(buttons.up.pin, buttons.up.pushed_level);
}

void SomfyRemote::down()
{
   digitalWrite(buttons.down.pin, buttons.down.pushed_level);
}

void SomfyRemote::center()
{
    digitalWrite(buttons.center.pin, buttons.center.pushed_level);
}

void SomfyRemote::select(select_position_t select_position)
{ 
    // current_sel_pos = get_current_select_position():
    // calculate distance to desired position
    // move to desired selection position
}

class ShuttersEvent
{
    public:

        typedef enum
        {
          ONE_TIME,
          DAILY,
          ON_WEEKDAYS,
          MONTHLY
        } recurrence_t;    

        ShuttersEvent(/*ocrrence, date, time*/);
          //Time
          //Data

        bool is_date_time_matching(/*current date & time*/);

    private:

        recurrence_t recurrence;
        //date
        //time
        uint8_t open_percentage;
      
};

class SomfyShutters
{
    public:

        typedef uint8_t open_percentage_t;

        SomfyShutters(SomfyRemote & remote, SomfyRemote::select_position_t id, String name);
        ~SomfyShutters();

        void open(uint8_t open_percentage);
        
        void register_event();
        void deregister_event();
        ShuttersEvent * is_event_registered(/* Date Time */);

    private:
        //time per 5% openess
        //current status.
        //id_in_remote
        String name;
        SomfyRemote &remote;
        ShuttersEvent events[5];
};