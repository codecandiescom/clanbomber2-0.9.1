/***************************************************************************
                          Explosion.cpp  -  description
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
// $Id: Explosion.cpp,v 1.9 2003/06/25 20:26:23 der_fisch Exp $

#include "ClanBomber.h"
#include "Explosion.h"

#include "Extra.h"
#include "Timer.h"
#include "Map.h"
#include "MapTile.h"
#include "Bomb.h"
#include "Bomber.h"
#include "Bomber_Corpse.h"
#include "Server.h"

Explosion::Explosion( int _x, int _y, int _power, Bomber *_bomber, ClanBomberApplication *_app ) : GameObject( _x, _y, _app )
{
	X = (_x+20) / 40;
	Y = (_y+20) / 40;
	power = _power;
	bomber = _bomber;

	init();
	find_lengths();
	do_destruction();
	splatter_corpses();
}

Explosion::~Explosion()
{
}

void Explosion::init()
{
	z = Z_EXPLOSION;
	detonation_period = 0.5f;
	
	PLAY_PAN(Resources::Game_explode());
	if (ClanBomberApplication::is_server() || !ClanBomberApplication::is_client()) {
		app->objects.add( this );
	}
}

void Explosion::find_lengths()
{
	if (ClanBomberApplication::is_server() || !ClanBomberApplication::is_client()) {
		int i;
		for (i=1; i<=power; i++) {
			if ((app->map->get_maptile(X-i,Y)->is_blocking()) || app->map->get_maptile(X-i,Y)->has_extra()) {
				break;
			}
		}
		l_left = i-1;
		for (i=1; i<=power; i++) {
			if ((app->map->get_maptile(X+i,Y)->is_blocking()) || app->map->get_maptile(X+i,Y)->has_extra()) {
				break;
			}
		}
		l_right = i-1;
		for (i=1; i<=power; i++) {
			if ((app->map->get_maptile(X,Y-i)->is_blocking()) || app->map->get_maptile(X,Y-i)->has_extra()) {
				break;
			}
		}
		l_up = i-1;
		for (i=1; i<=power; i++) {
			if ((app->map->get_maptile(X,Y+i)->is_blocking()) || app->map->get_maptile(X,Y+i)->has_extra()) {
				break;
			}
		}
		l_down = i-1;
	}
}

void Explosion::show()
{
	int anim = 14;
	if (detonation_period < 0.4f) {
		anim = 7;
	}
	if (detonation_period < 0.3f) {
		anim = 0;
	}
	if (detonation_period < 0.2f) {
		anim = 7;
	}
	if (detonation_period < 0.1f) {
		anim = 14;
	}

	Resources::Game_explosion()->put_screen( (int)(x+60), (int)(y+40), EXPLODE_X+anim );
	int i;
	for (i=1; i<=l_left; i++) {
		if (i == power) {
			Resources::Game_explosion()->put_screen( (int)(x-i*40+60), (int)(y+40), EXPLODE_LEFT+anim );
		}
		else {
			Resources::Game_explosion()->put_screen( (int)(x-i*40+60), (int)(y+40), EXPLODE_H+anim );
		}
	}
	for (i=1; i<=l_right; i++) {
		if (i == power) {
			Resources::Game_explosion()->put_screen( (int)(x+i*40+60), (int)(y+40), EXPLODE_RIGHT+anim );
		}
		else {
			Resources::Game_explosion()->put_screen( (int)(x+i*40+60), (int)(y+40), EXPLODE_H+anim );
		}
	}
	for (i=1; i<=l_up; i++) {
		if (i == power) {
			Resources::Game_explosion()->put_screen( (int)(x+60), (int)(y-i*40+40), EXPLODE_UP+anim );
		}
		else {
			Resources::Game_explosion()->put_screen( (int)(x+60), (int)(y-i*40+40), EXPLODE_V+anim );
		}
	}
	for (i=1; i<=l_down; i++) {
		if (i == power) {
			Resources::Game_explosion()->put_screen( (int)(x+60), (int)(y+i*40+40), EXPLODE_DOWN+anim );
		}
		else {
			Resources::Game_explosion()->put_screen( (int)(x+60), (int)(y+i*40+40), EXPLODE_V+anim );
		}
	}
}

void Explosion::kill_bombers()
{
	if (ClanBomberApplication::is_server() || !ClanBomberApplication::is_client()) {
		CL_Iterator<Bomber> bomber_object_counter(app->bomber_objects);
		while (bomber_object_counter.next() != NULL) {
			if(bomber_object_counter() != NULL) {
				int bx = (bomber_object_counter()->get_x()+20)/40;
				int by = (bomber_object_counter()->get_y()+20)/40;
				if ((by==Y && bx>=X-l_left && bx<=X+l_right) || (bx==X && by>=Y-l_up && by<=Y+l_down)) {
					if (bomber_object_counter()->die() && bomber_object_counter() != bomber) {
						bomber->inc_kills();
					}
				}
			}
		}
	}
}

void Explosion::destroy_extras()
{
	if (ClanBomberApplication::is_server() || !ClanBomberApplication::is_client()) {
		CL_Iterator<GameObject> object_counter(app->objects);
		while (object_counter.next() != NULL) {
			if(object_counter() != NULL) {
				if (object_counter()->get_type() == GameObject::EXTRA) {
					int bx = (object_counter()->get_x()+20)/40;
					int by = (object_counter()->get_y()+20)/40;
					
					if ((by==Y && bx>=X-l_left && bx<=X+l_right) || (bx==X && by>=Y-l_up && by<=Y+l_down)) {
						if (!(object_counter()->is_flying())) {
							((Extra*)object_counter())->destroy();
						}
					}
				}
			}
		}
	}
}

void Explosion::splatter_corpses()
{
	if (ClanBomberApplication::is_server() || !ClanBomberApplication::is_client()) {
		CL_Iterator<GameObject> object_counter(app->objects);
		while (object_counter.next() != NULL) {
			if(object_counter() != NULL) {
				if (object_counter()->get_type() == GameObject::BOMBER_CORPSE) {
					int bx = (object_counter()->get_x()+20)/40;
					int by = (object_counter()->get_y()+20)/40;
					
					if ((by==Y && bx>=X-l_left && bx<=X+l_right) || (bx==X && by>=Y-l_up && by<=Y+l_down)) {
                        ((Bomber_Corpse*)object_counter())->explode();
						if (ClanBomberApplication::is_server()) {
							ClanBomberApplication::get_server()->send_SERVER_EXPLOSION_SPLATTERED_CORPSE(object_counter()->get_object_id());
						}
					}
				}
			}
		}
	}
}

int Explosion::get_power() const
{
	return power;
}

void Explosion::act()
{
	detonate_other_bombs();
	kill_bombers();
	destroy_extras();

	detonation_period -= Timer::time_elapsed();
	if (detonation_period < 0) {
		if (ClanBomberApplication::is_server() || !ClanBomberApplication::is_client()) {
			delete_me = true;
		}
	}
}

void Explosion::do_destruction()
{
	if (ClanBomberApplication::is_server() || !ClanBomberApplication::is_client()) {
		app->map->get_maptile(X,Y)->destroy();
		
		if (l_left < power) {
			app->map->get_maptile(X-l_left-1,Y)->destroy();
		} 
		if (l_right < power) {
			app->map->get_maptile(X+l_right+1,Y)->destroy();
		}
		if (l_up < power) {
			app->map->get_maptile(X,Y-l_up-1)->destroy();
		}
		if (l_down < power) {
			app->map->get_maptile(X,Y+l_down+1)->destroy();
		}
	}
}

void Explosion::detonate_other_bombs()
{
	if (ClanBomberApplication::is_server() || !ClanBomberApplication::is_client()) {
		int i;
		for (i=0; i<=l_left; i++) {
			if (app->map->get_maptile(X-i,Y)->bomb) {
				app->map->get_maptile(X-i,Y)->bomb->explode_delayed();
				break;
			}
		}
		for (i=1; i<=l_right; i++) {
			if (app->map->get_maptile(X+i,Y)->bomb) {
				app->map->get_maptile(X+i,Y)->bomb->explode_delayed();
				break;
			}
		}
		for (i=1; i<=l_up; i++) {
			if (app->map->get_maptile(X,Y-i)->bomb) {
				app->map->get_maptile(X,Y-i)->bomb->explode_delayed();
				break;
			}
		}
		for (i=1; i<=l_down; i++) {
			if (app->map->get_maptile(X,Y+i)->bomb) {
				app->map->get_maptile(X,Y+i)->bomb->explode_delayed();
				break;
			}
		}
	}
}	

Bomber* Explosion::get_bomber()
{
	return bomber;
}

int Explosion::get_length_left()
{
	return l_left;
}

int Explosion::get_length_right()
{
	return l_right;
}

int Explosion::get_length_up()
{
	return l_up;
}

int Explosion::get_length_down()
{
	return l_down;
}

void Explosion::set_lengths(int leftlen, int rightlen, int uplen, int downlen)
{
	l_left = leftlen;
	l_right = rightlen;
	l_up = uplen;
	l_down = downlen;
}
