/***************************************************************************
                          Bomber_Corpse.h  -  description
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
// $Id: Bomber_Corpse.h,v 1.1 2001/09/20 22:59:49 der_fisch Exp $

#ifndef Bomber_Corpse_h
#define Bomber_Corpse_h

#include "GameObject.h"


class Bomber_Corpse : public GameObject
{
public:
	Bomber_Corpse (int _x, int _y, int _color, int _sprite_nr, ClanBomberApplication *_app );
	void act();
	~Bomber_Corpse();
	
	void explode();

	ObjectType get_type() const
	{
		return BOMBER_CORPSE;
	}
protected:
	int color;
	int splash_counter;
	bool ready_to_splash;
	
	float explode_delay;

private:
	void init();
};

#endif

