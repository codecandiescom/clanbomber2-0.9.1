/***************************************************************************
                          FILENAME  -  description                              
                             -------------------                                         
    begin                : in 1999                                           
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

#include <stdlib.h>

#include "Disease.h"

#include "Timer.h"

#include "Disease_Frozen.h"
#include "Disease_PutBomb.h"
#include "Disease_Stoned.h"
#include "Disease_Fast.h"

Disease::Disease(Bomber* _bomber)
{
	bomber = _bomber;
	countdown = -1;
}


void Disease::act()
{
	if (countdown != -1) {
		countdown-= Timer::time_elapsed();
		if (countdown < 0) {
			stop();
			countdown =-1;
		}
	}		
}

float Disease::get_countdown() const
{
	return countdown;
}

Disease::~Disease()
{
}

Disease* Disease::create(DiseaseType type, Bomber* _bomber)
{
	switch (type) { 
		case FROZEN:
			return new Disease_Frozen(_bomber);
		case PUTBOMB:
			return new Disease_PutBomb(_bomber);
		case STONED:
			return new Disease_Stoned(_bomber);
		case FAST:
			return new Disease_Fast(_bomber);
	}
 	return NULL;
}
