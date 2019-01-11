/***************************************************************************
                          Mutex.cpp  -  description
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

#include "Mutex.h"
#include "ClanBomber.h"

extern "C"
{
    int pthread_mutexattr_setkind_np(pthread_mutexattr_t *attr, int kind);
}

Mutex::Mutex()
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setkind_np(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
    //pthread_mutex_init(&mutex_handle, &attr);
    pthread_mutex_init(&mutex_handle, 0);
}

Mutex::~Mutex()
{
    pthread_mutex_destroy(&mutex_handle);
}

void Mutex::lock()
{
    if(ClanBomberApplication::is_client()) {
        pthread_mutex_lock(&mutex_handle);
    }
}

void Mutex::unlock()
{
    if(ClanBomberApplication::is_client()) {
        pthread_mutex_unlock(&mutex_handle);
    }
}
