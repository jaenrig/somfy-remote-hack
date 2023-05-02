
#ifndef SOMFY_REMOTE_HPP
#define SOMFY_REMOTE_HPP

#include <stdint.h>

/* Somfy situo 5 IO II */
class SomfyRemote
{
    public:

        typedef struct
        {
            uint8_t pin;
            uint8_t pushed_level;
        } button_t;

        typedef struct
        {
            button_t up;
            button_t my;
            button_t down;
            button_t select;
            button_t select_monitor;
        } button_set_t;

        typedef enum
        {
            SELECT_GROUP_UNDEFINED = 0,
            SELECT_GROUP_LED1,
            SELECT_GROUP_LED2,
            SELECT_GROUP_LED3,
            SELECT_GROUP_LED4,
            SELECT_GROUP_LEDSALL
        } select_group_t;

        SomfyRemote(button_set_t & buttons);
        ~SomfyRemote() { };

        void up();
        void my();
        void down();
        void select(select_group_t select_group);
        select_group_t get_select();

    private:
      
        button_set_t & buttons;

        static uint8_t get_lifted_level(uint8_t pushed_level);
        static void button_push_release(button_t & button);

        typedef uint8_t number_of_pushes;
        number_of_pushes reset_select_group();
        static select_group_t get_select_group_given_pushes_to_reset(uint8_t pushes_to_reset);
        void select_from_reset_position(select_group_t sel_group);

        typedef enum
        {
            SELECT_MON_UNDEFINED,
            SELECT_MON_LOW,
            SELECT_MON_HIGH,
            SELECT_MON_PWM
        } select_monitor_state_t;

        select_monitor_state_t get_select_monitor_state();
};

#endif /* SOMFY_REMOTE_HPP */