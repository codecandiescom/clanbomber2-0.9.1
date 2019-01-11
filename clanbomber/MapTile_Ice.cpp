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
#include "MapTile_Ice.h"
#include "GameObject.h"

MapTile_Ice::MapTile_Ice(int _x, int _y, ClanBomberApplication* _app) : MapTile_Ground(_x, _y, _app)
{
}

MapTile_Ice::~MapTile_Ice()
{
}

void MapTile_Ice::act()
{
	MapTile::act();

	CL_Iterator<GameObject> object_counter(objects);
	while (object_counter.next() != NULL) {
		if (object_counter() != NULL) {
			object_counter()->move(80,DIR_NONE);
		}
	}
}



void MapTile_Ice::draw(int addx, int addy)
{
	MapTile::draw (addx, addy);

	if (!vanishing) {
//		Resources::Game_maptile_addons()->put_screen( x+60+addx, y+40+addy,0);
	}
}

void MapTile_Ice::draw_tiny( int x, int y, float factor )
{
	__u8 opacity = 255;

	if (vanishing) {
		opacity = (__u8)(vanish_countdown * 255);
	}

	MapTile::draw_tiny(  x,  y,  factor );
	Resources::Game_maptile_addons()->put_screen( x, y, factor, factor, 0, opacity );
}

bool MapTile_Ice::get_addon_rect( DFBRectangle *ret_rect )
{
    Resources::Game_maptile_addons()->get_rect( 0, ret_rect );

    return true;
}

