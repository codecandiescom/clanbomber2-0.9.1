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


#ifndef MAPSELECTOR_H
#define MAPSELECTOR_H

class Map;

class CL_InputBuffer;

/**
  *@author Andreas Hundt, Denis Oliver Kropp
  */

class MapSelector
{
public: 
	MapSelector( ClanBomberApplication *app );
	~MapSelector();
	
	void exec();
	
protected:
	ClanBomberApplication	*app;
	
	int			current_map;
	int			map_at_top;
	int			current_theme;
	Map			*map;
	
	int list_width;
		
	void draw();
};

#endif


