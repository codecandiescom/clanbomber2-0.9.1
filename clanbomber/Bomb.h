/***************************************************************************
                          Bomb.h  -  description
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
// $Id: Bomb.h,v 1.4 2001/10/28 17:31:46 xmassx Exp $

#ifndef Bomb_h
#define Bomb_h

class Bomber;
class MapTile;

#include "GameObject.h"


class Bomb : public GameObject
{
public:
	void explode_delayed();
	void explode();
	void kick(Direction dir, bool by_arrow=false);
	void throww(Direction dir, int fields=4 );
	void stop(bool by_arrow=false);
	int get_power();
	float get_countdown();
	void set_countdown( float _countdown );
	void set_stopped();
	Bomber* get_bomber() const
	{
		return bomber;
	}
	Bomb( int _x, int _y, int _power, Bomber *_bomber, ClanBomberApplication *_app );
	~Bomb();

	virtual void show();
	virtual void act();
	ObjectType get_type() const
	{
		return BOMB;
	}
	int get_reactivation_frame_counter();
	void set_reactivation_frame_counter(int counter);
protected:
	float cur_scale;
	bool exploding;
	int power;
	float detonation_countdown;
	Bomber *bomber;
	int reactivation_frame_counter;
private:
	void init();
};

#endif
