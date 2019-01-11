/***************************************************************************
                          Map.cpp  -  description
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
// $Id: Map.cpp,v 1.14 2007/10/15 13:58:16 der_fisch Exp $

#include <fstream>
#include <stdio.h>
#include <string.h>

#include "ClanBomber.h"
#include "Map.h"

#include "MapTile.h"
#include "MapTile_Arrow.h"
#include "MapTile_Trap.h"
#include "MapTile_Wall.h"
#include "MapEntry.h"
#include "Config.h"
#include "Bomb.h"
#include "Bomber.h"
#include "Server.h"

#include <dirent.h>

CL_List<MapEntry> Map::map_list;

Map::Map( ClanBomberApplication* _app )
{
	app = _app;
	dummy_maptile = new MapTile_Wall(1000,1000,app);

	for (int y=0; y<MAP_HEIGHT; y++) {
		for (int x=0; x<MAP_WIDTH; x++) {
			maptiles[x][y] = NULL;
		}
	}
	current_map = NULL;
	enum_maps();
	sort_maps();
	load_selection();
	maps = new CL_Iterator<MapEntry>(map_list);
	is_received_by_client = false;
	current_server_map_name = "receiving map...";
	is_shaken = 0;
}

Map::~Map()
{
	if (app->is_server() || !app->is_client()) {
		delete maps;
	}
}

void Map::sort_maps()
{
	MapEntry **mapss = new MapEntry*[map_list.get_num_items()];
	int num = map_list.get_num_items();

	int i;	// MSVC really should know the new ANSI standard!

	for (i=0; i<num; i++) {
		mapss[i] = map_list[i];
	}

	bool found = true;
	while (found) {
		found = false;
		for (i=0; i<num-1; i++) {
			if (strcasecmp( mapss[i]->get_name(), mapss[i+1]->get_name() ) > 0) {
				MapEntry *temp = mapss[i];
				mapss[i] = mapss[i+1];
				mapss[i+1] = temp;
				found = true;
			}
		}
	}

	map_list.clear();
	for (i=0; i<num; i++) {
		map_list.add( mapss[i] );
	}

	delete[] mapss;
}

void Map::enum_maps()
{
	if (map_list.get_num_items()) {
		return;
	}

	DIR		*dir;
	dirent	*entry;
	
	CL_String path = app->get_map_path();
	dir = opendir( path );
	if (!dir) {
		return;
	}
	while ( (entry = readdir(dir)) != NULL ) {
		CL_String s(entry->d_name);
		
		if ( s.right(4) == ".map" ) {
			map_list.add( new MapEntry( path, s ) );
		}
	}
	closedir( dir );
	
	path = app->get_local_map_path();
	dir = opendir( path );
	if (dir) {
		while ( (entry = readdir(dir)) != NULL ) {
			CL_String s(entry->d_name);
			
			if ( s.right(4) == ".map" ) {
				map_list.add( new MapEntry( path, s ) );
			}
		}
	}
}

bool Map::any_valid_map()
{
	for (int i=0; i<get_map_count(); i++) {
		if (Config::get_number_of_players() <= map_list[i]->get_max_players()  && map_list[i]->is_enabled()) {
			return true;
		}
	}
	return false;
}

int Map::get_map_count()
{
	return map_list.get_num_items();
}

void Map::spin_up()
{
	current_map = maps->next();
	if (!current_map) {
		current_map = maps->next();
	}
}

void Map::spin_down()
{
	current_map = maps->prev();
	if (!current_map) {
		current_map = maps->prev();
	}
}

void Map::spin_to(int nr)
{
	current_map = maps->first();
	
	for (int i=0; i<nr; i++) {
		current_map = maps->next();
	}
}


void Map::load_random_valid()
{
	do {
		spin_to(rand()%get_map_count());
	} while (Config::get_number_of_players() > get_max_players()  || !current_map->is_enabled());
	reload();
}


void Map::load_next_valid(int map_nr)
{
	if (map_nr != -1) {
		spin_to(map_nr);
	}
	else {
		spin_up();
	}
	
	while (Config::get_number_of_players() > get_max_players()  || !current_map->is_enabled()) {
		spin_up();
	}
	
	reload();
}


void Map::load_previous_valid(int map_nr)
{
	if (map_nr != -1) {
		spin_to(map_nr);
	}
	else {
		spin_down();
	}
	
	while (Config::get_number_of_players() > get_max_players()  || !current_map->is_enabled()) {
		spin_down();
	}
	
	reload();
}

void Map::load( int map_nr )
{
	spin_to(map_nr);
	reload();
}

void Map::reload()
{
	current_map->read_bomber_positions(); // needed by MapEditor
	clear();
	
	for (int y=0; y<MAP_HEIGHT; y++) {
		for (int x=0; x<MAP_WIDTH; x++) {
			switch (current_map->get_data(x,y)) {
				case 48:
				case 49:
				case 50:
				case 51:
				case 52:
				case 53:
				case 54:
				case 55:
				case ' ':
					maptiles[x][y] = MapTile::create(MapTile::GROUND, 40*x, 40*y, app);
					break;
				case '*':
					maptiles[x][y] = MapTile::create(MapTile::WALL, 40*x, 40*y, app);
					break;
				case '+':
					maptiles[x][y] = MapTile::create(MapTile::BOX, 40*x, 40*y, app);
					break;
				case 'R':
					if ( rand() %3) {
						maptiles[x][y] = MapTile::create(MapTile::BOX, 40*x, 40*y, app);
					}
					else {
						maptiles[x][y] = MapTile::create(MapTile::GROUND, 40*x, 40*y, app);
					}					
					break;
				case 'S':
					maptiles[x][y] = MapTile::create( MapTile::ICE, 40*x, 40*y, app);
					break;
				case '<':
					maptiles[x][y] = new MapTile_Arrow(40*x, 40*y, DIR_LEFT,app);
					break;
				case '>':
					maptiles[x][y] = new MapTile_Arrow(40*x, 40*y, DIR_RIGHT,app);
					break;
				case '^':
					maptiles[x][y] = new MapTile_Arrow( 40*x, 40*y, DIR_UP,app);
					break;
				case 'v':
					maptiles[x][y] = new MapTile_Arrow( 40*x, 40*y, DIR_DOWN,app);
					break;
				case 'o':
					maptiles[x][y] = new MapTile_Trap( 40*x, 40*y, app);
					break;
				default:
					maptiles[x][y] = MapTile::create(MapTile::NONE, 40*x, 40*y, app);
					break;
			}
		}
	}
}

void Map::show()
{
    int          count = 0;
    DFBRectangle rects[MAP_WIDTH*MAP_HEIGHT];
    DFBPoint     points[MAP_WIDTH*MAP_HEIGHT];

    int          addon_count = 0;
    DFBRectangle addon_rects[MAP_WIDTH*MAP_HEIGHT];
    DFBPoint     addon_points[MAP_WIDTH*MAP_HEIGHT];

    int          hl_count = 0;
    DFBRectangle hl_rects[8];

	for (int y=0; y<MAP_HEIGHT; y++) {
		for (int x=0; x<MAP_WIDTH; x++) {
            MapTile *maptile = maptiles[x][y];

			if (!maptile)
                 continue;

            if (maptile->get_type() == MapTile::NONE)
                 continue;

            if (maptile->vanishing) {
                 maptile->draw();
                 continue;
            }


            int X = 60 + 40*x;
            int Y = 40 + 40*y;

					if (is_shaken > 0) {
                 X += rand()%5 - 2;
                 Y += rand()%5 - 2;
            }

            /* Populate destination points... */
            points[count] = (DFBPoint){ X, Y };

            /* ...and corresponding source rectangles. */
            Resources::Game_maptiles()->get_rect( maptile->sprite_nr, &rects[count++] );

            /* Populate addon rects and points. */
            if (maptile->get_addon_rect( &addon_rects[addon_count] ))
                 addon_points[addon_count++] = (DFBPoint){ X, Y };


            /* Populate highlights. */
            if (Config::get_highlight_maptiles()) {
                CL_Iterator<GameObject> object_counter(maptile->objects);
                while (object_counter.next() != NULL) {
                    if (object_counter() != NULL) {
                        if (object_counter()->get_type() == GameObject::BOMBER) {
                            if (Config::bomber[((Bomber*)object_counter())->get_number()].get_highlight_maptile()) {
                                hl_rects[hl_count++] = (DFBRectangle){ X, Y, 40, 40 };
					}
					}
				}
			}
		}
	}
	}

	if (is_shaken > 0) {
		is_shaken--;
	}

    if (count) {
         primary->SetBlittingFlags( primary, DSBLIT_NOFX );
         primary->BatchBlit( primary, Resources::Game_maptiles()->surface,
                             rects, points, count );
    }

    if (addon_count) {
         primary->SetBlittingFlags( primary, DSBLIT_SRC_COLORKEY );
         primary->BatchBlit( primary, Resources::Game_maptile_addons()->surface,
                             addon_rects, addon_points, addon_count );
    }

    if (hl_count) {
         primary->SetDrawingFlags( primary, DSDRAW_BLEND );
         primary->SetColor( primary, 0x00, 0x00, 0xFF, 50 );

         primary->FillRectangles( primary, hl_rects, hl_count );
    }
}

void Map::show_random_boxes()
{
	for (int y=0; y<MAP_HEIGHT; y++) {
		for (int x=0; x<MAP_WIDTH; x++) {
			if (current_map->get_data(x,y) == 'R') {
				Resources::Game_maptile_addons()->put_screen( 60+x*40, 40+y*40, 5 );
			}
		}
	}
}

void Map::show_start_positions()
{
	for (int i=0; i<current_map->get_max_players(); i++) {
		CL_Vector pos = current_map->get_bomber_pos(i);
		Resources::Game_maptile_addons()->put_screen( (int)(60 + pos.x*40), (int)(40+pos.y*40), 6 );
		
		CL_String number = i+1;
		primary->SetFont( primary, Resources::Font_small() );
		primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );
		primary->DrawString( primary, number, -1, (int)(80 + pos.x*40), (int)(52 + pos.y*40), DSTF_TOPCENTER );
	}
}

void Map::show_preview( int x, int y, float factor )
{
	int tile_size = (int)(40*factor);
	for (int yy=0; yy<MAP_HEIGHT; yy++) {
		for (int xx=0; xx<MAP_WIDTH; xx++) {
			if (maptiles[xx][yy]) {
				maptiles[xx][yy]->draw_tiny( x+xx*tile_size, y+yy*tile_size, factor );
			}
		}
	}
}

void Map::refresh_holes()
{
    int          count = 0;
    DFBRectangle rects[MAP_WIDTH*MAP_HEIGHT];

	for (int y=0; y<MAP_HEIGHT; y++) {
		for (int x=0; x<MAP_WIDTH; x++) {
			if (maptiles[x][y]) {
				if (maptiles[x][y]->get_type() == MapTile::NONE) {
                    rects[count++] = (DFBRectangle){ 60 + 40*x, 40 + 40*y, 40, 40 };
/*
					if (is_shaken > 0) {
						maptiles[x][y]->draw_shaken();
					}
					else {
						maptiles[x][y]->draw();
					}*/
					}
				}
			}
		}

    if (count) {
         primary->SetDrawingFlags( primary, DSDRAW_NOFX );
         primary->SetColor( primary, 0, 0, 0, 0xFF);

         primary->FillRectangles( primary, rects, count );
	}
}

void Map::maptilize_objects()
{
	for (int y=0; y<MAP_HEIGHT; y++) {
		for (int x=0; x<MAP_WIDTH; x++) {
			maptiles[x][y]->objects.clear();
			maptiles[x][y]->bomb = NULL;
		}
	}

	CL_Iterator<Bomber> bomber_object_counter(app->bomber_objects);
	while (bomber_object_counter.next() != NULL) {
		if (bomber_object_counter() != NULL) {
			if (!bomber_object_counter()->is_flying() && !bomber_object_counter()->is_dead()) {
				bomber_object_counter()->get_maptile()->objects.add(bomber_object_counter());
			}
		}
	}

	CL_Iterator<GameObject> object_counter(app->objects);
	while (object_counter.next() != NULL) {
		if (object_counter() != NULL) {
			if (!object_counter()->is_flying()) {
				if(object_counter()->get_maptile() != NULL) {
					object_counter()->get_maptile()->objects.add(object_counter());
					if (object_counter()->get_type() == GameObject::BOMB && !(object_counter()->is_falling())) {
						object_counter()->get_maptile()->bomb = (Bomb*)(object_counter());
					}
				}
			}
		}
	}
}

void Map::act()
{
	maptilize_objects();

	for (int y=0; y<MAP_HEIGHT; y++) {
		for (int x=0; x<MAP_WIDTH; x++) {
			maptiles[x][y]->act();
			if (ClanBomberApplication::is_server() || !ClanBomberApplication::is_client()) {
				if (maptiles[x][y]->delete_me) {
					MapTile* new_tile;
					new_tile = MapTile::spawn( maptiles[x][y] );
					delete maptiles[x][y];
					maptiles[x][y] = new_tile;
					if (ClanBomberApplication::is_server()) {
						Direction dir_tmp = DIR_NONE;
						if (new_tile->get_type() == MapTile::ARROW) {
							dir_tmp = ((MapTile_Arrow*)new_tile)->get_direction();
						}
						ClanBomberApplication::get_server()->send_SERVER_MAPTILE_REFRESH(x, y, new_tile->get_type(), dir_tmp);
					}
				}
			}
		}
	}
}	


void Map::shake( int _shake )
{
	is_shaken = Config::get_shaky_explosions()?_shake:0;
}


void Map::clear()
{
	for (int y=0; y<MAP_HEIGHT; y++) {
		for (int x=0; x<MAP_WIDTH; x++) {
			if (maptiles[x][y]) {
				delete maptiles[x][y];
				maptiles[x][y] = NULL;
			}
		}
	}
	is_shaken = 0;
}

bool Map::add_bomb(int bx, int by, int bpower, Bomber *bomber, int bomber_x, int bomber_y)
{
	if ((bx<0) || (by<0) || (bx>=MAP_WIDTH) || (by>=MAP_HEIGHT)) {
		return false;
	}
	if (maptiles[bx][by] == NULL) {
		return false;
	}
	if (maptiles[bx][by]->bomb == NULL) {
		maptiles[bx][by]->bomb = new Bomb( bx*40, by*40, bpower, bomber, app );
		if (ClanBomberApplication::is_server()) {
			app->get_server()->send_SERVER_ADD_BOMB(bomber_x, bomber_y, bx, by, bpower, bomber->get_object_id(), maptiles[bx][by]->bomb->get_object_id());
		}
		return true;
	}
	return false;
}

CL_String Map::get_name()
{	
	return current_map->get_name();
}

CL_String Map::get_author()
{
	return current_map->get_author();
}

CL_Vector Map::get_bomber_pos(int nr)
{
	return current_map->get_bomber_pos(nr);
}

void Map::randomize_bomber_positions()
{
	current_map->randomize_bomber_positions();
}

int Map::get_max_players()
{
	return current_map->get_max_players();
}


MapTile* Map::get_passable()
{
	int i =0;
	int xmapfield;
	int ymapfield;
	do {
		xmapfield = rand() % 17;
		ymapfield = rand() % 13;
		if (maptiles[xmapfield][ymapfield]->is_passable() && !maptiles[xmapfield][ymapfield]->is_vanishing()) {
			return maptiles[xmapfield][ymapfield];				
		}
		i++;
	} while (i<1000);
	return NULL;
}

MapTile* Map::get_random()
{
	int i =0;
	int xmapfield;
	int ymapfield;
	do {
		xmapfield = rand() % 17;
		ymapfield = rand() % 13;
		if (maptiles[xmapfield][ymapfield]->get_type()!=MapTile::NONE && !maptiles[xmapfield][ymapfield]->is_vanishing()) {
			return maptiles[xmapfield][ymapfield];				
		}
		i++;
	} while (i<1000);
	return NULL;
}

void Map::set_maptile(int x, int y, MapTile* mt)
{
    if(maptiles[x][y]) {
        delete maptiles[x][y];
    }
    maptiles[x][y]=mt;
}

MapTile* Map::get_maptile(int x, int y)
{
	if (x < 0 || x > MAP_WIDTH-1 || y < 0 || y > MAP_HEIGHT-1) {
		return dummy_maptile;
	}
	else {
		return maptiles[x][y];
	}
}

MapTile* Map::get_maptile_xy(int x, int y)
{
	if (x < 0 || x >= (MAP_WIDTH)*40  || y < 0 || y >= (MAP_HEIGHT)*40 ) {
		return dummy_maptile;
	}
	else {
		return maptiles[x/40][y/40];
	}
}


int Map::new_entry( CL_String _name )
{
	map_list.add( new MapEntry(_name , true) );
	return map_list.get_num_items() -1;
}

int Map::delete_entry( int nr )
{	
	map_list[nr]->delete_file();
	delete map_list[nr];
	map_list.del(map_list[nr]);
	
	if (nr > get_map_count()-1) {
		return nr-1;
	}
	return nr;
}

ostream& operator << (ostream& os, Map& m)
{
	for (int y=0; y<MAP_HEIGHT; y++) {
		for (int x=0; x<MAP_WIDTH; x++) {
			if (m.maptiles[x][y]->bomb) {
				os << "*";
			}
			else if (m.maptiles[x][y]->has_bomber()) {
				os << "B";
			}
			else if (m.maptiles[x][y]->has_extra()) {
				os << "e";
			}
			else {
				switch (m.maptiles[x][y]->get_type()) {
					case MapTile::NONE:
						os << "-";
						break;
					case MapTile::GROUND:
						os << " ";
						break;
					case MapTile::WALL:
						os << "#";
						break;
					case MapTile::BOX:
						os << "+";
						break;
					case MapTile::TRAP:
						os << " ";
						break;
					case MapTile::ARROW:
						os << " ";
						break;
					case MapTile::ICE:
						os << " ";
						break;
				}
			}
			os << " ";
		}
		os << std::endl;
	}
	return os;
}


void Map::save_selection()
{
	CL_String filename;
	filename = getenv("HOME");
	filename += "/.clanbomber/maps.disabled";

	ofstream map_selection_file(filename);
	CL_Iterator<MapEntry> map_entry_iterator(map_list);
	while (map_entry_iterator.next() != NULL) {
		if (!(map_entry_iterator()->is_enabled())) {
			map_selection_file << map_entry_iterator()->get_name() << std::endl;
		}
	}	
}

void Map::load_selection()
{
	CL_String filename;

	filename = getenv("HOME");
	filename += "/.clanbomber/maps.disabled";

	FILE* map_selection_file;
	char map_name[100];

	if ((map_selection_file = fopen( filename, "r" )) != NULL) {
		while (!(feof(map_selection_file))) {
			fscanf( map_selection_file, "%s\n", map_name );
			CL_Iterator<MapEntry> map_entry_iterator(map_list);
			while (map_entry_iterator.next() != NULL) {
				if (map_entry_iterator()->get_name() == map_name) {
					map_entry_iterator()->disable();
				}	
			}
		}
		fclose(map_selection_file);
	}
}

bool Map::is_dummy_maptile(MapTile* mt)
{
	return (dummy_maptile == mt);
}

unsigned int Map::get_current_map_name_checksum()
{
    return current_map->get_name_checksum(NULL);
}

unsigned int Map::get_current_map_data_checksum()
{
	return current_map->get_data_checksum();
}

int Map::get_map_name_checksums(unsigned int** chesksum_array)
{
    int num = map_list.get_num_items();
    unsigned int* csa = new unsigned int[num];
    for (int i=0; i<num; i++) {
        csa[i] = map_list[i]->get_name_checksum(NULL);
    }
    (*chesksum_array) = csa;
    return num;
}

int Map::get_map_data_checksums(unsigned int** chesksum_array)
{
	int num = map_list.get_num_items();
	unsigned int* csa = new unsigned int[num];
	for (int i=0; i<num; i++) {
		csa[i] = map_list[i]->get_data_checksum();
	}
	(*chesksum_array) = csa;
	return num;
}

void Map::load_by_data_checksum( unsigned int checksum )
{
    int map_nr = 0;
    int num = map_list.get_num_items();
    for (int i=0; i<num; i++) {
        if (map_list[i]->get_data_checksum() == checksum) {
            map_nr = i;
            break;
        }
    }
    spin_to(map_nr);
    reload();
}

MapEntry* Map::get_current_map()
{
	return current_map;
}
