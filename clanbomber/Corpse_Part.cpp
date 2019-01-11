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
#include "Corpse_Part.h"

#include "Timer.h"

Corpse_Part::Corpse_Part(int _x, int _y, ClanBomberApplication* _app) : GameObject(_x, _y, _app)
{
	speed = 800;
	surface = Resources::Corpse_part();
	sprite_nr = rand()%4;
	z = Z_CORPSE_PART;
	countdown = 2.0f;
	can_fly_over_walls = false;
	app->objects.add(this);
}

Corpse_Part::~Corpse_Part()
{
}

void Corpse_Part::act()
{
	if (!flying) {
		countdown -= Timer::time_elapsed();
		if (countdown < 0) {
			countdown = 0;
			delete_me = true;
		}
		opacity = (__u8) min( 255, (int)(255 * countdown) );
	}
	
	GameObject::act();
}
