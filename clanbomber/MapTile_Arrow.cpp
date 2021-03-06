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
#include "MapTile_Arrow.h"

#include "GameObject.h"
#include "Bomb.h"
#include "Timer.h"

#include <stdio.h>

MapTile_Arrow::MapTile_Arrow(int _x, int _y , Direction _dir, ClanBomberApplication* _app) : MapTile_Ground( _x, _y,_app)
{
	dir = _dir;
	old_bomb = NULL;
	chill_countdown = 0;
}

MapTile_Arrow::~MapTile_Arrow()
{
}

void MapTile_Arrow::act()
{
	MapTile::act();

	if (bomb) {
		if (bomb->is_stopped()) {
			bomb->snap();
		}
		if (bomb->get_cur_dir() != DIR_NONE  &&  bomb->get_cur_dir() != dir  &&  (bomb->get_cur_dir()+dir)%2 == 0  &&  !old_bomb) {
			bomb->snap();
			bomb->stop(true);
			chill_countdown = 0.5f;
		}
		if (chill_countdown > 0) {
			chill_countdown -= Timer::time_elapsed();
		}
		else {
			bomb->kick(dir, true);
		}
	}
	else {
		chill_countdown = 0;
	}
	old_bomb = bomb;
}

void MapTile_Arrow::draw(int addx, int addy)
{
	MapTile::draw (addx, addy);
	if (!vanishing) {
//		Resources::Game_maptile_addons()->put_screen( x+60+addx, y+40+addy,dir+1);
	}
}

void MapTile_Arrow::draw_tiny( int x, int y, float factor )
{
	__u8 opacity = 255;

	if (vanishing) {
		opacity = (__u8)(vanish_countdown * 255);
	}

	MapTile::draw_tiny(  x,  y,  factor );
	Resources::Game_maptile_addons()->put_screen( x, y, factor, factor, dir+1, opacity );
}

bool MapTile_Arrow::get_addon_rect( DFBRectangle *ret_rect )
{
    Resources::Game_maptile_addons()->get_rect( dir+1, ret_rect );

    return true;
}

