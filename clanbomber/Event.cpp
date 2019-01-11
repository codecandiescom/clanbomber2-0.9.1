/***************************************************************************
                          Event.cpp  -  description
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

#include "Event.h"

#include "ClanBomber.h"

Event::Event()
{
    pthread_mutex_init (&event_mutex, 0);
    pthread_cond_init (&wait_condition, 0);
}

Event::~Event()
{
    pthread_mutex_destroy (&event_mutex);
    pthread_cond_destroy (&wait_condition);
}

void Event::lock()
{
    if(ClanBomberApplication::is_client()) {
        pthread_mutex_lock (&event_mutex);
    }
}

void Event::unlock()
{
    if(ClanBomberApplication::is_client()) {
        pthread_mutex_unlock (&event_mutex);
    }
}

void Event::signal()
{
    if(ClanBomberApplication::is_client()) {
        pthread_cond_signal (&wait_condition);
    }
}

void Event::wait()
{
    if(ClanBomberApplication::is_client()) {
         pthread_cond_wait (&wait_condition, &event_mutex);
    }
}

void Event::broadcast()
{
    if(ClanBomberApplication::is_client()) {
        pthread_cond_broadcast (&wait_condition);
    }
}
