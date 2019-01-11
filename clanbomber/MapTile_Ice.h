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

#ifndef MAPTILE_ICE_H
#define MAPTILE_ICE_H

#include "MapTile_Ground.h"

/**
  *@author Andreas Hundt, Denis Oliver Kropp
  */

class MapTile_Ice : public MapTile_Ground
{
public:
	MapTile_Ice(int _x, int _y, ClanBomberApplication* _app);
	virtual ~MapTile_Ice();
	
	void act();
	void draw(int addx, int addy);
	void draw_tiny( int x, int y, float factor );
    virtual bool get_addon_rect( DFBRectangle *ret_rect );
	TYPE get_type() { return ICE; }
};

#endif














