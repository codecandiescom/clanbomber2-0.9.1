/***************************************************************************
                          Thread.cpp  -  description
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

#include "Thread.h"

Thread::Thread()
{
    thread_handle=0;
}
	
Thread::~Thread()
{
    stop();
}

void Thread::run(THREADPROC proc, void* param)
{
    pthread_create(&thread_handle, NULL, proc, param);
}

void Thread::stop()
{
    pthread_cancel(thread_handle);
}
