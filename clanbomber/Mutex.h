/***************************************************************************
                          Mutex.h  -  description
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

#ifndef _header_clanbomber_mutex_
#define _header_clanbomber_mutex_

#include <pthread.h>

class Mutex
{
    public:
        Mutex();
        ~Mutex();
        void lock();
        void unlock();
    private:
        pthread_mutex_t mutex_handle;
};

#endif
