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


#ifndef MAPENTRY_H
#define MAPENTRY_H

#define MAP_WIDTH	17
#define MAP_HEIGHT	13

#include "cl_vector.h"

class MapEntry
{
public:
	MapEntry( CL_String _path, CL_String _filename );
	MapEntry( CL_String _name, bool write );
	~MapEntry();
	
	CL_Vector	get_bomber_pos(int nr);
	int			get_max_players();
	CL_String	get_name();
	CL_String	get_author();
	bool		is_enabled();
	bool		is_writable();
	bool		toggle();
	void		enable();
	void		disable();

	int	get_data(int x, int y);
	bool	is_random(int x, int y);
	void	set_data(int x, int y, char data);
	void	set_bomber_pos(int x, int y, int number);
	void	set_max_players(int nr);
	void	set_author(CL_String _author);
	void	set_name(CL_String _name);
	void	reset_filename(char* client_name);
	void	write_back();
	void 	clear();
	void	reload();
	void	read_bomber_positions();
	void	delete_file();
	void	randomize_bomber_positions();

	unsigned int get_name_checksum(char* name);
	unsigned int get_data_checksum();
protected:
	CL_String	path;
	CL_String	filename;
	CL_String	map_name;
	CL_String	author;
	bool		enabled;
	bool		writable;
	int			max_players;
	CL_Vector	bomber_pos[8];
	char		map_data[MAP_WIDTH][MAP_HEIGHT];
};

#endif


