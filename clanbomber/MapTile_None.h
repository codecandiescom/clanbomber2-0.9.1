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


#ifndef MAPTILE_NONE_H
#define MAPTILE_NONE_H

#include "MapTile.h"

/**
  *@author Andreas Hundt, Denis Oliver Kropp
  */

class MapTile_None : public MapTile
{
public: 
	MapTile_None(int _x, int _y, ClanBomberApplication* _app);
	virtual ~MapTile_None();
	
	void act();
	void draw(int addx =0, int addy =0);
	void draw_tiny( int x, int y, float factor );
	
	void vanish();	
	TYPE get_type() { return NONE; }

};

#endif



















