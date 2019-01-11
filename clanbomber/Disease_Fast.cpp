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
#include "Disease_Fast.h"
#include "Extra_Koks.h"

#include "Bomber.h"

Disease_Fast::Disease_Fast(Bomber* _bomber) : Disease (_bomber)
{
	start();
}

Disease_Fast::~Disease_Fast()
{
}

void Disease_Fast::start()
{
	countdown = 10.0f;
	bomber->inc_speed(600);
}

void Disease_Fast::stop()
{
	bomber->dec_speed(600);
}

Disease* Disease_Fast::spawn(Bomber* _bomber)
{
	return new Disease_Fast(_bomber);
}

Extra* Disease_Fast::spawn_extra( int _x, int _y)
{
	return new Extra_Koks (_x,_y, bomber->app);
}
