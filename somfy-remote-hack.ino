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

      typedef enum
      {
        SHUTTERS_ID1,
        SHUTTERS_ID2,
        SHUTTERS_ID3,
        SHUTTERS_ID4,
        SHUTTERS_ALL
      }shutters_id_t;

      SomfyRemote(uint8_t select_pin, uint8_t up_pin, uint8_t down_pin, uint8_t center_pin);
      ~SomfyRemote();
      void begin();
      void select_shutters(shutters_id_t shutters_id);
      void up();
      void down();
      void center();

    private:
      
      shutters_id_t current_selected_shutters;

};

class ShuttersEvent
{
  public:

    typedef enum
    {
      ONE_TIME,
      DAILY,
      ON_WEEKDAYS,
      MONTHLY
    } Recurrence_t;    

    ShuttersEvent(/*ocrrence, date, time*/);
      //Time
      //Data

    bool is_date_time_matching(/*current date & time*/);

    private:

      Recurrence_t recurrence;
      //date
      //time
      uint8_t open_percentage;
      
};

class SomfyShutters
{
  public:

    typedef uint8_t open_percentage_t;

    SomfyShutters(SomfyRemote & remote, SomfyRemote::shutters_id_t id, String name);
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