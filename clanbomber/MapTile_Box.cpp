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
#include "MapTile_Box.h"
#include "Config.h"
#include "Server.h"

MapTile_Box::MapTile_Box(int _x, int _y, ClanBomberApplication* _app) : MapTile(_x, _y, _app)
{
	sprite_nr = Config::get_theme()*4 +2;
	next = GROUND;
	passable = false;
	blocking = true;
}

MapTile_Box::~MapTile_Box()
{
}

void MapTile_Box::act()
{
	MapTile::act();
	if (ClanBomberApplication::is_server() || !ClanBomberApplication::is_client()) {
		if (destroyed && destroy_countdown <= 0) {
			spawn_extra();
		}
	}
}

void MapTile_Box::destroy()
{
	if (!destroyed) {
		if (ClanBomberApplication::is_server()) {
		 	ClanBomberApplication::get_server()->send_SERVER_MAPTILE_VANISH(x, y);
		}
		destroyed = true;
		destroy_countdown = 0.5f;
		sprite_nr++;
	}
}

void MapTile_Box::vanish()
{
	destroy();
}
