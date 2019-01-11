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
#include "MapTile_None.h"
#include "GameObject.h"
#include "Server.h"

MapTile_None::MapTile_None(int _x, int _y, ClanBomberApplication* _app) : MapTile(_x , _y, _app)
{
	passable = false;
	blocking = false;
}

MapTile_None::~MapTile_None()
{
}

void MapTile_None::draw(int addx, int addy)
{
/*	primary->SetDrawingFlags( primary, DSDRAW_NOFX );
	primary->SetColor( primary, 0, 0, 0, 0xFF);
	primary->FillRectangle( primary, x+60+addx, y+40+addy, 40, 40 );*/
}

void MapTile_None::draw_tiny( int x, int y, float factor )
{
}

void MapTile_None::act()
{
	MapTile::act();
	if (ClanBomberApplication::is_server() || !ClanBomberApplication::is_client()) {
		CL_Iterator<GameObject> object_counter(objects);
		while (object_counter.next() != NULL) {
			if (object_counter() != NULL) {
				object_counter()->fall();
			}
		}
	}
}

void MapTile_None::vanish()
{
}
