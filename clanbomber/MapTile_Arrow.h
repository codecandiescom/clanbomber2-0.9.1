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

#ifndef MAPTILE_ARROW_H
#define MAPTILE_ARROW_H

#include "MapTile_Ground.h"

/**
  *@author Andreas Hundt, Denis Oliver Kropp
  */

class MapTile_Arrow : public MapTile_Ground
{
public:
	

	MapTile_Arrow(int _x, int _y, Direction _dir, ClanBomberApplication* _app);
	virtual ~MapTile_Arrow();
	
	void act();
	void draw(int addx, int addy);
	void draw_tiny( int x, int y, float factor );
    virtual bool get_addon_rect( DFBRectangle *ret_rect );
  Direction get_direction() const
	{
    return dir;
	}
	TYPE get_type() { return ARROW; }

protected:
	Direction dir;
	Bomb* old_bomb;
	float chill_countdown;
};

#endif


















