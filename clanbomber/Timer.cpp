/***************************************************************************
                          timer.cpp  -  description                              
                             -------------------                                         
    begin                : Sun Apr 4 1999                                           
    copyright            : (C) 1999 by Andreas Hundt, Denis Oliver Kropp                         
    email                : clanbomber@fischlustig.de                                     
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/

#include <stddef.h>
#include <sys/time.h>
#include "Timer.h"

int Timer::begin_time = 0;
float Timer::elapsed = 0;

Timer::Timer()
{
	reset();
}

Timer::~Timer()
{
}

void Timer::reset()
{
	begin_time = Timer::get_time();
	elapsed = 0;
}

long Timer::get_time()
{
 	struct timeval tv;

 	gettimeofday( &tv, NULL );

 	return (tv.tv_sec*1000  +  tv.tv_usec/1000);
}

float Timer::time_elapsed(bool update)
{	
	if (update) {
		elapsed = (Timer::get_time() - begin_time)/1000.0f;
		begin_time = Timer::get_time();
	}
	
	return elapsed;	
}
