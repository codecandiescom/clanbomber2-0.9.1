/***************************************************************************
                          Bomb.cpp  -  description
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
// $Id: Bomb.cpp,v 1.11 2003/06/25 20:26:23 der_fisch Exp $

#include "ClanBomber.h"
#include "Resources.h"

#include "Bomb.h"

#include "Explosion.h"
#include "Timer.h"
#include "Bomber.h"
#include "Map.h"
#include "MapTile.h"
#include "Config.h"
#include "Server.h"

Bomb::Bomb( int _x, int _y, int _power, Bomber *_bomber, ClanBomberApplication *_app ) : GameObject( _x, _y, _app )
{
	surface = Resources::Game_bombs();
	detonation_countdown = Config::get_bomb_countdown()/10.0f;
	cur_scale = 0;
	cur_dir = DIR_NONE;
	exploding = false;
	speed = Config::get_bomb_speed();
	can_kick = false;
	can_fly_over_walls = false;
	bomber = _bomber;
	power = _power;
	sprite_nr = bomber->get_color()*4;
	reactivation_frame_counter = -1;
    bomber->dec_cur_bombs();
	if (ClanBomberApplication::is_server() || !ClanBomberApplication::is_client()) {
		app->objects.add( this );
	}
}

Bomb::~Bomb()
{
	if (bomber != NULL) {
		bomber->inc_cur_bombs();
	}
	if (get_maptile() != NULL) {
		get_maptile()->bomb = NULL;
	}
}

void Bomb::stop(bool by_arrow)
{
	if (ClanBomberApplication::is_server() || !ClanBomberApplication::is_client() || by_arrow) {
		GameObject::stop();
		if (!flying) {
			cur_dir = DIR_NONE;
			snap();
		}
		if (ClanBomberApplication::is_server() && !by_arrow) {
			ClanBomberApplication::get_server()->send_SERVER_BOMB_KICK_STOP(object_id, (int)x, (int)y);
		}
	}
}

void Bomb::show()
{
	GameObject::show();
}

void Bomb::kick (Direction dir, bool by_arrow)
{
	if (!flying) {
		if (ClanBomberApplication::is_server() || !ClanBomberApplication::is_client() || by_arrow) {
			cur_dir = dir;
			if (ClanBomberApplication::is_server() && !by_arrow) {
				ClanBomberApplication::get_server()->send_SERVER_BOMB_KICK_START(object_id, (int)dir);
			}
		}
	}
}

void Bomb::throww(Direction dir, int fields)
{
	can_fly_over_walls = false;
	if (fields > 0  &&  !flying) {
		if (ClanBomberApplication::is_server() || !ClanBomberApplication::is_client()) {
			cur_dir = dir;
			switch( dir ) {
				case DIR_UP:
					fly_to( get_x(), get_y()-fields*40 );
					break;
				case DIR_DOWN:
					fly_to( get_x(), get_y()+fields*40 );
					break;
				case DIR_LEFT:
					fly_to( get_x()-fields*40, get_y() );
					break;
				case DIR_RIGHT:
					fly_to( get_x()+fields*40, get_y() );
					break;
				default:
					break;
			}
		}
	}
}

void Bomb::act()
{
	GameObject::act();

	if (fallen_down) {
		if(ClanBomberApplication::is_server() || !ClanBomberApplication::is_client()) {
			delete_me = true;
		}
		return;
	}
	
	if (flying) {
		z = Z_BOMB + Z_FLYING + get_y();
		return;
	}
	
	if (fly_progress == 1) {
		fly_progress = 0;
		can_fly_over_walls = false;
		if(ClanBomberApplication::is_server() || !ClanBomberApplication::is_client()) {
			if (get_maptile()->bomb) {
				throww( cur_dir, 1 );
			}
			else {
				cur_dir = DIR_NONE;
			}
		}
		else if(!ClanBomberApplication::is_server() && ClanBomberApplication::is_client()) {
			cur_dir = DIR_NONE;
		}
	}
	else {
		move();
		cur_scale += Timer::time_elapsed()*10;
		sprite_nr = bomber->get_color()*4+(int)cur_scale % 4;
		detonation_countdown -= Timer::time_elapsed();
		if (detonation_countdown <= 0) {
			if (ClanBomberApplication::is_server() || !ClanBomberApplication::is_client()) {
				explode();
			}
		}
		z = Z_BOMB + get_y();
	}
}

void Bomb::explode()
{
	if (!exploding && !falling) {
		snap();
		exploding = true;
		Explosion* tmp=new Explosion( (int)x, (int)y, power, bomber, app );
		if (ClanBomberApplication::is_server()) {
            int leftlen = tmp->get_length_left();
			int rightlen = tmp->get_length_right();
			int uplen = tmp->get_length_up();
			int downlen = tmp->get_length_down();
			ClanBomberApplication::get_server()->send_SERVER_ADD_EXPLOSION((int)x, (int)y, leftlen, rightlen, uplen, downlen, power, bomber->get_object_id(), tmp->get_object_id());
		}
		app->map->shake( (int)(0.25f/Timer::time_elapsed()) );
		if (ClanBomberApplication::is_server() || !ClanBomberApplication::is_client()) {
			delete_me = true;
		}
	}
}

void Bomb::explode_delayed()
{
	if (detonation_countdown > Config::get_bomb_delay()/100.0f) {
		detonation_countdown = Config::get_bomb_delay()/100.0f;

		if (detonation_countdown == 0) {
			explode();
		}
	}
}

int Bomb::get_power()
{
	return power;
}

float Bomb::get_countdown()
{
	return detonation_countdown;
}

void Bomb::set_countdown( float _countdown )
{
	detonation_countdown = _countdown;
}

void Bomb::set_stopped()
{
	stopped = true;
}

int Bomb::get_reactivation_frame_counter()
{
	return reactivation_frame_counter;
}

void Bomb::set_reactivation_frame_counter(int counter)
{
	reactivation_frame_counter = counter;
}
