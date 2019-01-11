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


#ifndef MAPTILE_BOX_H
#define MAPTILE_BOX_H

#include "MapTile.h"

/**
  *@author Andreas Hundt, Denis Oliver Kropp
  */

class MapTile_Box : public MapTile
{
public: 
	MapTile_Box(int _x, int _y, ClanBomberApplication* _app);
	virtual ~MapTile_Box();
	
	void act();
	
	virtual void destroy();
	virtual void vanish();
	
	TYPE get_type() { return BOX; }
};

#endif
























