/***************************************************************************
                          GameObject.h  -  description
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
// $Id: GameObject.h,v 1.4 2001/10/31 18:23:40 xmassx Exp $

#ifndef GameObject_h
#define GameObject_h

#include "ClanBomber.h"
class ClanBomberApplication;
class MapTile;
class CL_Surface;


#define Z_FLYING			9000
#define Z_CORPSE_PART		7000
#define Z_OBSERVER			5000
#define Z_BOMBER			4000
#define Z_BOMB				3000
#define Z_EXPLOSION			2000
#define Z_EXTRA				1000
#define Z_CORPSE			 100
#define Z_GROUND			   0
#define Z_FALLING			  -1
#define Z_FALLING_MAPTILE	  -2

class GameObject
{
public:
	GameObject( int _x, int _y, ClanBomberApplication *_app );
	virtual ~GameObject();

	int get_object_id();
	void set_object_id(int obj_id);
	int get_server_x();
	int get_server_y();
	int get_orig_x();
	int get_orig_y();

	void output_object_info();
	
	Direction get_server_dir();
	Direction get_client_dir();
	void set_server_x(int sx);
	void set_server_y(int sy);
	void set_server_dir(int sd);
	void set_client_dir(int cd);
	void set_local_dir(int ld);
	void set_cur_dir(int cd);
	void set_offset(int _x, int _y);

	void set_pos( int _x, int _y );
	void set_dir (Direction _dir);
	void set_orig( int _x, int _y );
	void move_pos( int _x, int _y );
	void snap();
	bool move(int _speed, Direction _dir=DIR_NONE);
	void inc_speed( int _c=1 );
	void dec_speed( int _c=1 );
	void set_speed( int _speed );
	virtual void fall();

	int get_x() const;
	int get_y() const;
	int get_z() const;
	int get_map_x() const;
	int get_map_y() const;

	int get_speed() const;
	
	MapTile* get_maptile() const;
	
	bool is_flying() const;
	bool is_stopped() const;

	void set_fly_over_walls(bool val)
	{
		can_fly_over_walls = val;
	}

	Direction get_cur_dir() const;

	typedef enum
	{
		BOMB,
		BOMBER,
		BOMBER_CORPSE,
		EXPLOSION,
		EXTRA,
		DEBUG,
		OBSERVER,
		CORPSE_PART
	} ObjectType;

	static char* objecttype2string(ObjectType t);

	virtual ObjectType	get_type() const =0;
	virtual void		stop(bool by_arrow=false);
	virtual void		show();
	virtual void		act();
	void show	(int _x, int _y);
	void show	(int _x, int _y, float _scale);
	void fly_to (int _x, int _y, int _speed=0);
	void fly_to (MapTile*, int _speed=0);
	void gain_kick();
	void loose_kick();

	bool is_falling();
	bool is_able_to_kick() const
	{
		return can_kick;
	}
	
	/* set to true if object should be deleted next act loop */
	bool delete_me;
	ClanBomberApplication *app;
	void set_next_fly_job(int flyjobx, int flyjoby, int flyjobspeed);
protected:
	/* which surface to put for this object */
	Resources::Surface* surface;
	/* which sprite_nr to show in the next draw */
	int sprite_nr;
	/* screen offset added to x and y */
	int offset_x;
	int offset_y;
	/* opacity to blit with */
	__u8 opacity;
	__u8 opacity_scaled;
	
	float x;
	float y;
	int z;
	int orig_x;
	int orig_y;
	float remainder;
	Direction cur_dir;
	int speed;
	
	bool move();
	bool move_left();
	bool move_right();
	bool move_up();
	bool move_down();

	int whats_left();
	int whats_right();
	int whats_up();
	int whats_down();

	void continue_flying();
	void continue_falling();

	bool falling;
	bool fallen_down;
	float fall_countdown;

	bool flying;
	float fly_dist_x;
	float fly_dist_y;
	float fly_dest_x;
	float fly_dest_y;
	float fly_speed;
	float fly_progress;
	
	bool can_kick;
	bool can_pass_bomber;
	bool can_fly_over_walls;
	bool stopped;
	
	int object_id;
	Direction server_dir;
	Direction client_dir;
	Direction local_dir;
	int server_x;
	int server_y;
	void reset_next_fly_job();
private:
	bool is_next_fly_job();
	int next_fly_job[3];
	void init(ClanBomberApplication *_app);
};

#endif


