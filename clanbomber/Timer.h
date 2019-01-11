/***************************************************************************
                          timer.h  -  description                              
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


#ifndef TIMER_H
#define TIMER_H

class Timer {
public: 
	Timer();
	~Timer();
	
	static void reset();
	static float time_elapsed(bool update=false);
	static long get_time();
protected:

	static int begin_time;
	static float elapsed;
};

#endif

