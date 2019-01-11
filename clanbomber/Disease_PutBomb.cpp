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
#include "Disease_PutBomb.h"
#include "Extra_Viagra.h"

#include "Bomber.h"
#include "Controller.h"

Disease_PutBomb::Disease_PutBomb(Bomber* _bomber) : Disease(_bomber)
{
	start();
}

Disease_PutBomb::~Disease_PutBomb()
{
}

void Disease_PutBomb::start()
{
	countdown = 10.0f;
	bomber->controller->bomb_always();
}

void Disease_PutBomb::stop()
{
	bomber->controller->bomb_normal();
}

Disease* Disease_PutBomb::spawn(Bomber* _bomber)
{
	return new Disease_PutBomb(_bomber);
}

Extra* Disease_PutBomb::spawn_extra( int _x, int _y)
{
     return new Extra_Viagra (_x,_y, bomber->app);
}
