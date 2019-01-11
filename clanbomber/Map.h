/***************************************************************************
                          Map.h  -  description
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
// $Id: Map.h,v 1.9 2003/12/30 12:50:03 xmassx Exp $

#ifndef Map_h
#define Map_h

#include "MapEntry.h"

class ClanBomberApplication;
class MapTile;

class Map
{
public:
	Map( ClanBomberApplication* _app );
	~Map();

	bool add_bomb( int bx, int by, int bpower, Bomber *bomber, int bomber_x=0, int bomber_y=0 );
	void clear();
	void load(int map_nr );
	void load_by_data_checksum( unsigned int checksum );
	void load_random_valid();
	void load_next_valid(int map_nr = -1);
	void load_previous_valid(int map_nr = -1);
	void show();
	void show_preview( int x, int y, float factor );
	void show_random_boxes();
	void show_start_positions();
	void refresh_holes();
	void maptilize_objects();
	void act();
	void shake( int _shake );
	void save_selection();
	void load_selection();
	MapTile* get_passable();
	MapTile* get_random();
	MapTile* get_maptile(int x, int y);
	MapTile* get_maptile_xy (int x, int y);
        void set_maptile(int x, int y, MapTile* mt);
	bool is_dummy_maptile(MapTile* mt);

	CL_String get_name();
	CL_String get_author();
	int	get_max_players();
	
	CL_Vector get_bomber_pos(int nr);
	void randomize_bomber_positions();
	
	void reload();
	int get_map_count();
	bool any_valid_map();
	
	int new_entry( CL_String _name );
	int delete_entry( int nr );

	friend ostream& operator << (ostream&, Map& m);

	// static stuff
	static CL_List<MapEntry> map_list;
	bool is_received_by_client;
	char* current_server_map_name;
	
	unsigned int get_current_map_name_checksum();
	int get_map_name_checksums(unsigned int** chesksum_array);
	unsigned int get_current_map_data_checksum();
	int get_map_data_checksums(unsigned int** chesksum_array);
	MapEntry* get_current_map();
protected:
	ClanBomberApplication* app;
	int is_shaken;
	MapTile* maptiles[MAP_WIDTH][MAP_HEIGHT];
	MapTile* dummy_maptile; // this is a dummy maptile::wall for "clipping" in get_maptile(int x, int y)

	MapEntry *current_map;
	CL_Iterator<MapEntry> *maps;
	
	void enum_maps();
	void sort_maps();
	void spin_up();
	void spin_down();
	void spin_to(int nr);
};

#endif








