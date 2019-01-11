/***************************************************************************
                          Bomber_Corpse.cpp  -  description
                             -------------------
    begin                : ?
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
// $Id: Bomber_Corpse.cpp,v 1.5 2003/06/25 20:26:23 der_fisch Exp $

#include "ClanBomber.h"
#include "Bomber_Corpse.h"
#include "Bomber.h"
#include "Timer.h"
#include "Corpse_Part.h"
#include "Map.h"
#include "MapTile.h"
#include "Config.h"
#include "Server.h"

Bomber_Corpse::Bomber_Corpse ( int _x, int _y, int _color, int _sprite_nr, ClanBomberApplication *_app ) : GameObject(_x, _y, _app )
{
	init();
	sprite_nr = _sprite_nr;
	color = _color;
	switch (color) {
		case 0 :
		case 1 :
		case 3 :
			offset_x = 60;
			offset_y = 11;
			break;
		case 2 :
		case 4 :
		case 5 :
		case 6 :
		case 7 :
			offset_x = 60;
			offset_y = 40;				
	}
	surface = Resources::Bombers(color);
}

void Bomber_Corpse::init()
{
	speed = 30;
	ready_to_splash = true;
	splash_counter = 0;
	z = Z_CORPSE + get_y();
	explode_delay = 0.8f;
	if (ClanBomberApplication::is_server() || !ClanBomberApplication::is_client()) {
		app->objects.add( this );
	}
}

void Bomber_Corpse::act()
{
	if (falling) {
		continue_falling();
		if (fallen_down) {
			if (ClanBomberApplication::is_server() || !ClanBomberApplication::is_client()) {
				delete_me = true;
			}
			return;
		}
	}
	
	if (splash_counter != -1) {
		cur_dir = Direction(rand() % 4);
		move();
		CL_Iterator<Bomber> bomber_object_counter(app->bomber_objects);
		while (bomber_object_counter.next() != NULL) {
			if (bomber_object_counter() != NULL) {
				if (!bomber_object_counter()->is_dead()) {
					if (abs(bomber_object_counter()->get_x() - get_x()) < 30) {
						if (abs(bomber_object_counter()->get_y() - get_y()) < 30) {
							if (ready_to_splash) {
								PLAY_PAN(Resources::Splash(rand()%2));
								splash_counter++;
								if (splash_counter == 5) {
									splash_counter = -1;					
								}
						
							}
							ready_to_splash = false;
							return;
						}				
					}
				}
			}
		}
	}
	ready_to_splash = true;
	explode_delay -= Timer::time_elapsed();
}


Bomber_Corpse::~Bomber_Corpse()
{
}

void Bomber_Corpse::explode()
{
	if (explode_delay < 0) {
		PLAY_PAN(Resources::Corpse_explode());
		if (ClanBomberApplication::is_server() || !ClanBomberApplication::is_client()) {
			delete_me = true;
		}
		
		for (int i=0; i<Config::get_corpse_parts(); i++) {
			Corpse_Part* cp;
			cp = new Corpse_Part( (int)x, (int)y, app );
			
			int xmaptiles, ymaptiles;
			xmaptiles = rand() % 17;
			ymaptiles = rand() % 13;
			
			cp->fly_to(xmaptiles*40 +rand()%20-10 ,ymaptiles*40 +rand()%20-10);
		}
	}
}
