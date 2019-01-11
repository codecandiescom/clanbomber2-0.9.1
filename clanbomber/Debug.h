/***************************************************************************
                          Debug.h  -  description
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
// $Id: Debug.h,v 1.1 2001/09/20 22:59:49 der_fisch Exp $

#ifndef Debug_h
#define Debug_h

#include "GameObject.h"

class Debug : public GameObject
{
public:
	Debug( int _x, int _y, ClanBomberApplication *_app );
	~Debug();

	virtual void show();
	virtual void act();
	virtual void fall() {}; // does not fall ;-)
	
	int current_map;

	ObjectType get_type() const
	{
		return DEBUG;
	}

protected:
	
	bool key_F2;
	bool key_F3;
	bool key_divide;
	bool key_multiply;
};

#endif


