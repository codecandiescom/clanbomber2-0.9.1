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
#include "MapTile_Wall.h"
#include "Config.h"

MapTile_Wall::MapTile_Wall(int _x, int _y, ClanBomberApplication* _app) : MapTile(_x, _y, _app)
{
	sprite_nr = Config::get_theme()*4 +1;
	passable = false;
	blocking = true;
	next = GROUND;
}

MapTile_Wall::~MapTile_Wall()
{
}

void MapTile_Wall::act()
{
	MapTile::act();
}

void MapTile_Wall::vanish()
{
	if (ClanBomberApplication::is_server() || !ClanBomberApplication::is_client()) {
		delete_me = true;
	}
}
