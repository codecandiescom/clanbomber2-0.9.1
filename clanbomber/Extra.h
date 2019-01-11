/***************************************************************************
                          Extra.h  -  description
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
// $Id: Extra.h,v 1.1 2001/09/20 22:59:49 der_fisch Exp $

#ifndef Extra_h
#define Extra_h

#include "GameObject.h"

class ClanBomberApplication;
class Bomber;

class Extra : public GameObject
{
public:
	void destroy();
	Extra( int _x, int _y, ClanBomberApplication *_app );
	~Extra();

	void act();
	
	char* get_name()
	{
		return "Extra";
	}

	ObjectType get_type() const
	{
		return EXTRA;
	}
	
	typedef enum
	{
		VIAGRA = -3,
		JOINT = -2,
		KOKS = -1,
		GLOVE = 1,
		KICK = 2,
		SKATEBOARD = 3,
		BOMB = 4,
		POWER = 5
	} ExtraType;		

	static Extra* create( ExtraType type, int _x, int _y, ClanBomberApplication* _app );

	virtual ExtraType get_ExtraType() =0;

	bool is_destroyable() const;
		
protected:
	bool destroyable;
	bool destroyed;
	float destroy_countdown;
	virtual void effect( Bomber* bomber ) = 0;
};

#endif


