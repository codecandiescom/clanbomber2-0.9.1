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

#include "ClanBomber.h"
#include "Disease_Stoned.h"
#include "Extra_Joint.h"

#include "Bomber.h"
#include "Controller.h"

Disease_Stoned::Disease_Stoned(Bomber* _bomber) : Disease (_bomber)
{
	start();
}

Disease_Stoned::~Disease_Stoned()
{
}

void Disease_Stoned::start()
{
	countdown = 10.0f;
	bomber->controller->revert();
}

void Disease_Stoned::stop()
{
	bomber->controller->revert();
}

Disease* Disease_Stoned::spawn(Bomber* _bomber)
{
	return new Disease_Stoned(_bomber);
}

Extra* Disease_Stoned::spawn_extra( int _x, int _y)
{
     return new Extra_Joint (_x,_y, bomber->app);
}
