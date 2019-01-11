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

#include <stdio.h>

#include "ClanBomber.h"
#include "MapEntry.h"

#include <sys/stat.h>
#include <unistd.h>

static char map_template[MAP_HEIGHT][MAP_WIDTH] = {	{'*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*'},
													{'*','0',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','2','*'},
													{'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
													{'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
													{'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
													{'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
													{'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
													{'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
													{'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
													{'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
													{'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
													{'*','3',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','1','*'},
													{'*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*'}	};

MapEntry::MapEntry( CL_String _path, CL_String _filename )
{
	filename = _filename;
	path = _path;
	enabled = true;
	
	// extract map name
	map_name = filename.mid( 0, filename.get_length()-4 );
	
	// is the file writable?
	struct stat fs;
	stat( path + "/" + filename, &fs );
	
	if (fs.st_mode & 0002  ||  getuid() == 0) { // writable anyway
		writable = true;
	}
	else {
		writable = false;
	}
	if (!writable  &&  fs.st_mode & 0020) { // check if user is in the files group
		if (fs.st_gid == getgid()) {
			writable = true;
		}
	}
	if (!writable  &&  fs.st_mode & 0200) { // check if user is the files user
		if (fs.st_uid == getuid()) {
			writable = true;
		}
	}

	reload();
}

MapEntry::MapEntry( CL_String _name , bool write )
{
	map_name = _name;
	filename = map_name + ".map";
	enabled = true;
	path = ClanBomberApplication::get_local_map_path();
	writable = true;
	
	author = "anonymous coward";
	max_players = 4;
	for (int x=0; x<MAP_WIDTH; x++) {
		for (int y=0; y<MAP_HEIGHT; y++) {
			map_data[x][y] = map_template[y][x]; // do not ask ;-)
		}
	}
	if (write) {
		write_back();
	}
}

MapEntry::~MapEntry()
{
}

void MapEntry::reload()
{
	// read map data from file
	char authorstring[50];
	
	FILE *mapfile;
	mapfile = fopen( path + "/" + filename, "r");
		
	fgets( authorstring, 50, mapfile );
	author = authorstring;

	// fix last char
	int authorlen=author.get_length();
    author.get_string()[authorlen-1]='\0';

	fscanf( mapfile, "%d\n", &max_players);
	
	for (int i=0; i<8; i++) {
		bomber_pos[i].x = 0;
		bomber_pos[i].y = 0;
	}
	
	for (int y=0; y<MAP_HEIGHT; y++) {
		for (int x=0; x<MAP_WIDTH; x++) {
			map_data[x][y] = fgetc( mapfile );
			switch (map_data[x][y]) {
				case 48:
					bomber_pos[0].x = x;
					bomber_pos[0].y = y;
					break;
				case 49:
					bomber_pos[1].x = x;
					bomber_pos[1].y = y;
					break;
				case 50:
					bomber_pos[2].x = x;
					bomber_pos[2].y = y;
					break;
				case 51:
					bomber_pos[3].x = x;
					bomber_pos[3].y = y;
					break;
				case 52:
					bomber_pos[4].x = x;
					bomber_pos[4].y = y;
					break;
				case 53:
					bomber_pos[5].x = x;
					bomber_pos[5].y = y;
					break;
				case 54:
					bomber_pos[6].x = x;
					bomber_pos[6].y = y;
					break;
				case 55:
					bomber_pos[7].x = x;
					bomber_pos[7].y = y;
					break;
			}
		}	

		if (fgetc( mapfile ) == 13) {
			fgetc( mapfile );
		}
	}

	// clean up ;-)	
	fclose( mapfile );
		
	read_bomber_positions();
}

void MapEntry::read_bomber_positions()
{
	for (int i=0; i<8; i++) {
		bomber_pos[i].x = 0;
		bomber_pos[i].y = 0;
	}
	
	for (int y=0; y<MAP_HEIGHT; y++) {
		for (int x=0; x<MAP_WIDTH; x++) {
			switch (map_data[x][y]) {
				case 48:
					bomber_pos[0].x = x;
					bomber_pos[0].y = y;
					break;
				case 49:
					bomber_pos[1].x = x;
					bomber_pos[1].y = y;
					break;
				case 50:
					bomber_pos[2].x = x;
					bomber_pos[2].y = y;
					break;
				case 51:
					bomber_pos[3].x = x;
					bomber_pos[3].y = y;
					break;
				case 52:
					bomber_pos[4].x = x;
					bomber_pos[4].y = y;
					break;
				case 53:
					bomber_pos[5].x = x;
					bomber_pos[5].y = y;
					break;
				case 54:
					bomber_pos[6].x = x;
					bomber_pos[6].y = y;
					break;
				case 55:
					bomber_pos[7].x = x;
					bomber_pos[7].y = y;
					break;
			}
		}
	}
}

CL_Vector MapEntry::get_bomber_pos(int nr)
{
	if (nr < 0 || nr > 7) {
		return CL_Vector( 0,0,0 );
	}
	return bomber_pos[nr];
}

int MapEntry::get_max_players()
{
	return max_players;
}

void MapEntry::set_max_players(int nr)
{
	if (nr > 1  &&  nr < 9) {
		max_players = nr;
	}
}

CL_String MapEntry::get_name()
{
	return map_name;
}

CL_String MapEntry::get_author()
{
	return author;
}

bool MapEntry::is_enabled()
{
	return enabled;
}

bool MapEntry::is_writable()
{
	return writable;
}

bool MapEntry::toggle()
{
	enabled = !enabled;
	return enabled;
}

void MapEntry::enable()
{
	enabled = true;
}

void MapEntry::disable()
{
	enabled = false;
}

int MapEntry::get_data(int x, int y)
{	
	return map_data[x][y];
}

bool MapEntry::is_random(int x, int y)
{
	return map_data[x][y]=='R';
}

void MapEntry::set_data( int x, int y, char data )
{

	map_data[x][y] = data;
}

void MapEntry::set_author( CL_String _author )
{
	author = _author;
}

void MapEntry::set_name(CL_String _name)
{
	map_name = _name;
}

void MapEntry::reset_filename(char* client_name)
{
    if (client_name != NULL) {
		map_name = map_name + "(" + client_name  + ")";
	    filename = map_name + ".map";
	}
	else {
		filename = map_name + ".map";
	}
}

void MapEntry::set_bomber_pos( int posx, int posy, int number )
{

	for (int x=0; x<MAP_WIDTH; x++) {
		for (int y=0; y<MAP_HEIGHT; y++) {
			if (map_data[x][y] == number+48) {
				map_data[x][y] = ' ';
			}
		}
	}
	map_data[posx][posy] = number+48;
}

void MapEntry::write_back()
{
	FILE *f;
	
	f = fopen( path+"/"+filename, "w" );
	if (!f) {
		return;
	}
	
	fprintf( f, "%s\n", author.get_string() );
	fprintf( f, "%d\n", max_players );
	
	for (int y=0; y<MAP_HEIGHT; y++) {
		for (int x=0; x<MAP_WIDTH; x++) {
			fputc( map_data[x][y], f );
		}
		fprintf( f, "\n" );
	}
	
	fclose(f);
}

void MapEntry::clear()
{
	memset( map_data, '-', sizeof(map_data) );
}

void MapEntry::delete_file()
{
	unlink( path+"/"+filename );
}


void MapEntry::randomize_bomber_positions()
{
	bool taken[8]={false,false,false,false,false,false,false,false};
	CL_Vector temp_bomber_pos[8];
	for (int i=0;i<max_players;i++) {
		int new_pos;
		do {
			new_pos = rand()%max_players;
		} while (taken[new_pos] == true);

		temp_bomber_pos[new_pos] = bomber_pos[i];
		taken[new_pos] = true;
	}
	for (int fuckgates=0; fuckgates<8; fuckgates++) {
		bomber_pos[fuckgates] = temp_bomber_pos[fuckgates];
	}
}

unsigned int MapEntry::get_name_checksum(char* name)
{
	int name_cs = 0;
	if (name == NULL) {
		for (int i=0; i<map_name.get_length(); i++) {
	 	    name_cs += (map_name.get_string()[i]*i*77777);
		}
	}
	else {
		for (unsigned int i=0; i<strlen(name); i++) {
			name_cs += (name[i]*i*77777);
		}
	}
	return name_cs;
}

unsigned int MapEntry::get_data_checksum()
{
	int map_cs = 0;
	for (int y=0; y<MAP_HEIGHT; y++) {
		for (int x=0; x<MAP_WIDTH; x++) {
			map_cs += (map_data[x][y]*x*y*777);
		}
	}
	return map_cs;
}
