/***************************************************************************
                          Thread.h  -  description
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

#ifndef _header_clanbomber_thread_
#define _header_clanbomber_thread_

#include <pthread.h>

typedef void* (*THREADPROC)(void*);

class Thread
{
    public:
        Thread();
        ~Thread();
        void run(THREADPROC proc, void* param);
        void stop();
    private:
        pthread_t thread_handle;
};

#endif
