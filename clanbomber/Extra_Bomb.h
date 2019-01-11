/***************************************************************************
                          Extra_Bomb.h  -  description
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
// $Id: Extra_Bomb.h,v 1.1 2001/09/20 22:59:49 der_fisch Exp $

#ifndef Extra_Bomb_h
#define Extra_Bomb_h

#include "Extra.h"

class Extra_Bomb : public Extra
{
public:
	Extra_Bomb( int _x, int _y, ClanBomberApplication *_app );	
	~Extra_Bomb();

	ExtraType get_ExtraType()
	{
		return BOMB;
	}

	
protected:
	virtual void effect( Bomber* bomber );
};

#endif













