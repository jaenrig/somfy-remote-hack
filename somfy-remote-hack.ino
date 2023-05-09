#include <ArduinoOTA.h>
#include "somfy-remote.hpp"

void led_setup();
void led_blink();
extern void remote_test_cmd_line_help();
extern void remote_test_cmd_line();

void setup() 
{
    Serial.begin(115200);
    delay(1000);

    Serial.println("somfy-remote-hack.ino");
    led_setup();

    remote_test_cmd_line_help();
}

void loop() 
{
    led_blink();
    remote_test_cmd_line();
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

void ntp_client_update_date_time()
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