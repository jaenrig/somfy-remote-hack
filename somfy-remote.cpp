#include <Arduino.h>
#include "somfy-remote.hpp"
#include "somfy-conf.hpp"

/**
 * @brief       Somfy remote hack extension
 * @details     Somfy Situo IO II remote
 * 
 *              The remote hack extension consist of the following 
 *              signals:
 *              - Up: GPIO output controlling the Up button signal. 
 *              - My: GPIO output controlling the My button signal.
 *              - Down: GPIO output controlling the Down button signal.
 *              - Select group: GPIO output controlling the Select Group 
 *              signal.  
 *              - Select group monitor: GPIO output monitoring the 
 *              select group signal. 
 *            
 *              This constructor initializes the pins as output or 
 *              inputs.
 * @param[in]   buttons List of buttons structs
 */
SomfyRemote::SomfyRemote(button_set_t & buttons):
buttons(buttons)
{
    pinMode(buttons.up.pin, OUTPUT);
    pinMode(buttons.my.pin, OUTPUT);
    pinMode(buttons.down.pin, OUTPUT);
    pinMode(buttons.select.pin, OUTPUT);
    pinMode(buttons.select_monitor.pin, INPUT);
}

/**
 * @brief Push the Up button
 */
void SomfyRemote::up()
{
    button_push_release(buttons.up);
}

/**
 * @brief Push the My button
 */
void SomfyRemote::my()
{
    button_push_release(buttons.my);
}

/** 
 * @brief Push the Down button
 */
void SomfyRemote::down()
{
    button_push_release(buttons.down);
}

/**
 * @brief       Select the group passed by argument
 * @details     The way that the select algorithm works is by 
 *              (re)setting the select group to the reference position 
 *              LED1 (by using the select monitor), and from there 
 *              perform the required pushes in the select group output
 *              as the index of that group starting from the reference 
 *              LED1.
 * 
 * @param[in]   select_group Desired select group index 
 */
void SomfyRemote::select(select_group_t select_group)
{    
    reset_select_group();
    select_from_reset_position(select_group);
}

/** 
 * @brief       Provides the button lifted logical level given the push button
 *              logic level
 * @param[in]   pushed_level Logic level for push
 * @return      HIGH or LOW pin level
 */
uint8_t SomfyRemote::get_lifted_level(uint8_t pushed_level)
{
    return (uint8_t)((HIGH == pushed_level) ? LOW : HIGH);
}

/**
 * @brief       Performs a push and release on the button pin
 * 
 * @param[in]   button Button to be pushed
 */
void SomfyRemote::button_push_release(button_t & button)
{
    /**
     * The push down time longer than 0.5 s for up/down
     * buttons performs a higher speed on the action.
     * A push down time smaller than that is implemented.
     */
    constexpr uint16_t pushed_down_time_ms = 200; 

    digitalWrite(button.pin, button.pushed_level);
    delay(pushed_down_time_ms);
    digitalWrite(button.pin, get_lifted_level(button.pushed_level));
}

/**
 * @brief   Get the current selected group
 * @details In order to find out the current selected group, the 
 *          select group is pushed until the reference LED1 or LEDS ALL
 *          is detected. The selected group needs to be set again back 
 *          to the current group. 
 * @return  Select group index
 */
SomfyRemote::select_group_t SomfyRemote::get_select()
{
    uint8_t pushes_number = reset_select_group();
    select_group_t select_group = get_select_group_given_pushes_to_reset(pushes_number);
    select_from_reset_position(select_group);

    return select_group;
}

/**
 * @brief   Reset the selector group
 * @details The selector group button is pushed until the monitor
 *          signal is pulled LOW for 5 seconds. 
 * @return  Number of pushes to bring the selector group to reset
 */
SomfyRemote::number_of_pushes SomfyRemote::reset_select_group()
{
    uint8_t pushes_number = 0;
    select_monitor_state_t select_monitor_state = SELECT_MON_UNDEFINED;

    while(SELECT_MON_LOW == select_monitor_state)
    {
        button_push_release(buttons.select);
        pushes_number++;
        select_monitor_state = get_select_monitor_state();
    };

    return pushes_number;
}

/**
 * @brief   Gets the selected group given the select pushes to reset
 * @return  Selected group
 */
SomfyRemote::select_group_t SomfyRemote::get_select_group_given_pushes_to_reset(uint8_t pushes_to_reset)
{
    static const select_group_t sel_group_for_given_select_pushes_to_reset [] = {
        SELECT_GROUP_UNDEFINED, 
        SELECT_GROUP_LED1, 
        SELECT_GROUP_LEDSALL,  
        SELECT_GROUP_LED4, 
        SELECT_GROUP_LED3,   
        SELECT_GROUP_LED2 
    };

    return sel_group_for_given_select_pushes_to_reset[pushes_to_reset];
}

/**
 * @brief       Set select group starting from reference reset position
 * @param[in]   sel_group Desired select group
 */
void SomfyRemote::select_from_reset_position(select_group_t sel_group)
{
    for(uint8_t i = SELECT_GROUP_LED1; i < sel_group; i++)
    {
        button_push_release(buttons.select);
        delay(300);
    }
}

/**
 * @brief       Get select monitor state
 * @details     The select group behaves in the following way:
 *              - Select group LED1 from inactivity or from LEDS ALL:
 *                falling edge, 5 s low, rising edge
 *              - Select group in LED ALLS from inactivity or from LED4:
 *                falling edge, 5 s of PWM (5 ms period), rising edge.
 *              - Select group in LED2, LED3 or LED4 from inactivity, 
 *                or from previous LED1, LED2 and LED3 group respectively:
 *                signal high
 * @note          This function is meant to be called immediately 
 *                after a select group push and release event
 * @return      Select monitor state
 */

SomfyRemote::select_monitor_state_t SomfyRemote::get_select_monitor_state()
{
    /**
     * By monitoring the signal for 0.5 s after a push and release
     * of the rising edge select group button, it be determined the
     * of the select value set.
     */
    constexpr uint16_t sampling_window_in_ms = 500;

    /* Experimental measured delay is < 50 x ms */
    constexpr uint8_t delay_after_select_group_push_in_ms = 50;     
    /** 
     * Experimental PWM period is ~5 ms. 
     * To ensure both levels are detected, 
     * the sampling frequency must be at 
     * least double. 
     */
    constexpr uint8_t sampling_period_in_ms = 2;                       


    delay(delay_after_select_group_push_in_ms);

    /**
     * During a second, measure the amount of
     * high and low levels digital reads
     */
    unsigned long start_time = millis();
    
    uint16_t level_count_low = 0;
    uint16_t level_count_high = 0; 

    while((millis() - start_time) < sampling_window_in_ms)
    {
        uint8_t sel_mon_level = digitalRead(buttons.select_monitor.pin);
        if(LOW == sel_mon_level)
        {
            level_count_low++;
        }
        else if(HIGH == sel_mon_level)
        {
            level_count_high++;
        }
        delay(sampling_period_in_ms);
    }

    if((level_count_high > 0) && (level_count_low == 0))
    {
        return SELECT_MON_HIGH;
    }
    
    if((level_count_high == 0) && (level_count_low > 0))
    {
        return SELECT_MON_LOW;
    }

    if((level_count_high > 0) && (level_count_low > 0))
    {
        return SELECT_MON_PWM;
    }

    return SELECT_MON_UNDEFINED;
}

/**
 * @brief Pre-instantiated object   
 */

SomfyRemote::button_t button_up =
{ 
    UP_PIN,
    LOW
};
SomfyRemote::button_t button_my = 
{ 
    MY_PIN,
    LOW
};
SomfyRemote::button_t button_down = 
{ 
    DOWN_PIN,
    LOW
};
SomfyRemote::button_t button_select = 
{ 
    SELECT_PIN, 
    LOW
};
SomfyRemote::button_t button_sel_monitor =
{
    SELECT_MON_PIN,
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