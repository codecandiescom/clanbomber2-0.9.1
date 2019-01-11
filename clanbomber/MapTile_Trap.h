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


#ifndef MAPTILE_TRAP_H
#define MAPTILE_TRAP_H

class Bomb;

#include "MapTile_Ground.h"

/**
  *@author Andreas Hundt, Denis Oliver Kropp
  */

class MapTile_Trap : public MapTile_Ground  {
public: 
	MapTile_Trap(int _x, int _y, ClanBomberApplication* _app);
	~MapTile_Trap();

	void act();
	void draw(int addx, int addy);
	void draw_tiny( int x, int y, float factor );
    virtual bool get_addon_rect( DFBRectangle *ret_rect );
	TYPE get_type() { return TRAP; }

protected:
	class Bomb_Entry
	{
	public:
		Bomb_Entry( Bomb *_bomb );
		void count();
		bool ready_to_shoot();
		Bomb *get_bomb() { return bomb; }
	protected:
		float countdown;
		Bomb *bomb;
	};

	CL_List<Bomb_Entry> bombs;
};

#endif






