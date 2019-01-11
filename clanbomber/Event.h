/***************************************************************************
                          Event.h  -  description
                             -------------------
    begin		 : Mon Jul 29 2001
    email		 : clanbomber@fischlustig.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _header_clanbomber_event_
#define _header_clanbomber_event_

#include <pthread.h>

class Event
{
    public:
        Event();
        ~Event();
        void lock();
        void unlock();
        void signal();
        void broadcast();
        void wait();
    private:
        pthread_mutex_t event_mutex;
        pthread_cond_t  wait_condition;
};

#endif
