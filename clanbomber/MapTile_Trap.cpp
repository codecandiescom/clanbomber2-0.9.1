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
#include "MapTile_Trap.h"
#include "Map.h"
#include "GameObject.h"
#include "Bomb.h"
#include "Timer.h"

MapTile_Trap::MapTile_Trap(int _x, int _y, ClanBomberApplication* _app) : MapTile_Ground( _x, _y, _app )
{
}

MapTile_Trap::~MapTile_Trap()
{
	CL_Iterator<Bomb_Entry> bomb_counter(bombs);
	while (bomb_counter.next() != NULL) {
		if (bomb_counter() != NULL) {
			delete bomb_counter()->get_bomb();
			delete bomb_counter();
		}
	}
}

void MapTile_Trap::act()
{
	MapTile::act();
	if (ClanBomberApplication::is_server() || !ClanBomberApplication::is_client()) {
		CL_Iterator<GameObject> object_counter(objects);
		while (object_counter.next() != NULL) {
			if (object_counter() != NULL) {
				if (object_counter()->get_type() == GameObject::BOMB) {
					app->objects.del( object_counter() );
					bomb = NULL;
					bombs.add( new Bomb_Entry((Bomb*)object_counter()) );
				}
			}
		}
		CL_Iterator<Bomb_Entry> bomb_counter(bombs);
		while (bomb_counter.next() != NULL) {
			if (bomb_counter() != NULL) {
				if (bomb_counter()->ready_to_shoot()) {
					MapTile *tile = app->map->get_passable();
					if (tile) {
						app->objects.add( bomb_counter()->get_bomb() );
						
						bomb_counter()->get_bomb()->set_fly_over_walls(true);
						bomb_counter()->get_bomb()->set_countdown( 0.01f );
						bomb_counter()->get_bomb()->set_speed( 200 );
						
						bomb_counter()->get_bomb()->fly_to( tile );
					}
					else {
						delete bomb_counter()->get_bomb();
					}
					
					delete bomb_counter();
					bomb_counter.remove();
				}
				else {
					bomb_counter()->count();
				}
			}
		}
	}
}

void MapTile_Trap::draw(int addx, int addy)
{
	MapTile::draw (addx, addy);
	if (!vanishing) {
//		Resources::Game_maptile_addons()->put_screen( x+60+addx, y+40+addy,7 );
	}
}

void MapTile_Trap::draw_tiny( int x, int y, float factor )
{
	__u8 opacity = 255;

	if (vanishing) {
		opacity = (__u8)(vanish_countdown * 255);
	}

	MapTile::draw_tiny(  x,  y,  factor );
	Resources::Game_maptile_addons()->put_screen( x, y, factor, factor, 7, opacity );
}

bool MapTile_Trap::get_addon_rect( DFBRectangle *ret_rect )
{
    Resources::Game_maptile_addons()->get_rect( 7, ret_rect );

    return true;
}

MapTile_Trap::Bomb_Entry::Bomb_Entry( Bomb *_bomb )
{
	bomb = _bomb;
	countdown = 1;
}

void MapTile_Trap::Bomb_Entry::count()
{
	countdown -= Timer::time_elapsed();
}

bool MapTile_Trap::Bomb_Entry::ready_to_shoot()
{
	return (countdown <= 0);
}
