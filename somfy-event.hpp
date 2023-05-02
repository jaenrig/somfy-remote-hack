#ifndef SOMFY_EVENT_HPP
#define SOMFY_EVENT_HPP

#include "somfy-shutters.hpp"

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

        typedef enum
        {
          NOW,
          LATER
        } exec_time_t;

        ShuttersEvent(/*ocrrence, date, time*/);
          //Time
          //Data

        bool is_date_time_matching(/*current date & time*/);

        exec_time_t when();

        void execute();
    private:

        recurrence_t recurrence;
        //date
        //time
        uint8_t open_percentage;
};

#endif /* SOMFY_EVENT_HPP */