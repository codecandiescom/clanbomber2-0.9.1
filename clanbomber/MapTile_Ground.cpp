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
#include "MapTile_Ground.h"
#include "Config.h"

MapTile_Ground::MapTile_Ground(int _x, int _y, ClanBomberApplication* _app) : MapTile( _x, _y,_app)
{
	sprite_nr = Config::get_theme()*4;
	passable = true;
	blocking = false;
}

MapTile_Ground::~MapTile_Ground()
{
}

void MapTile_Ground::act()
{
	MapTile::act();
}
