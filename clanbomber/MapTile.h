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


#ifndef MAPTILE_H
#define MAPTILE_H

class GameObject;
class Bomb;

/**
  *@author Andreas Hundt, Denis Oliver Kropp
  */

class MapTile
{
friend class Map;

public: 
	MapTile(int _x, int _y, ClanBomberApplication* _app);
	virtual ~MapTile();
	
	typedef enum
	{
		NONE = -3,
		ARROW = -2,
		ICE = -1,
		GROUND = 0,
		WALL = 1,
		BOX = 2,
		TRAP,
	} TYPE;
	
	virtual TYPE get_type() =0;
	
	static MapTile* create( TYPE type, int _x, int _y, Direction _dir, ClanBomberApplication* _app );
	static MapTile* create( TYPE type, int _x, int _y, ClanBomberApplication* _app );
	static MapTile* spawn( MapTile* old_tile );
	
	void set_pos( int _x, int _y );
	int get_x();
	int get_y();
	void set_sprite_nr( int sprite );
	int get_sprite_nr();
	void set_vanish_countdown(float cd);
	void set_destroy_countdown(float cd);
	void set_vanishing(bool v);
	void spawn_extra();

	virtual void act();
	virtual void draw(int addx =0, int addy =0);
	virtual void draw_tiny(int x, int y, float factor);
	virtual void draw_shaken();
	
    virtual bool get_addon_rect( DFBRectangle *ret_rect );
	
	CL_List<GameObject> objects;
	
	//MapTile *right, *left, *up, *down;
	
	bool is_vanishing() { return vanishing; }
	bool is_destroyed() { return destroyed; }
	bool has_extra();
	bool has_bomber();
	bool is_passable();
	bool is_blocking();
	
	virtual void vanish();
	virtual void destroy();
	
	Bomb *bomb;
	
	bool delete_me;
	
protected:
	int x;
	int y;
	int sprite_nr;
	TYPE next;
	
	bool vanishing;
	bool destroyed;
	bool passable;
	bool blocking;
	
	float vanish_countdown;
	float destroy_countdown;
	
	ClanBomberApplication* app;
};

#endif


