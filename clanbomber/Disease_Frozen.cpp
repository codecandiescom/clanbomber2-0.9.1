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
#include "Disease_Frozen.h"
#include "Extra_Viagra.h"

#include "Bomber.h"
#include "Controller.h"

Disease_Frozen::Disease_Frozen(Bomber* _bomber) : Disease (_bomber)
{
	start();
}

Disease_Frozen::~Disease_Frozen()
{
}

void Disease_Frozen::start()
{
	countdown = 3.0f;
	bomber->controller->deactivate();
}

void Disease_Frozen::stop()
{
	bomber->controller->activate();
}

Disease* Disease_Frozen::spawn(Bomber* _bomber)
{
	return new Disease_Frozen(_bomber);
}

Extra* Disease_Frozen::spawn_extra( int _x, int _y)
{
	return new Extra_Viagra (_x,_y, bomber->app);
}
