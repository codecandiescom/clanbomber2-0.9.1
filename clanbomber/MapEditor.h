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


#ifndef MAPEDITOR_H
#define MAPEDITOR_H

class Map;

/**
  *@author Andreas Hundt, Denis Oliver Kropp
  */

class MapEditor
{
public: 
	MapEditor( ClanBomberApplication *app );
	~MapEditor();
	
	void exec();
	
protected:
	ClanBomberApplication	*app;
	
	int			current_map;
	int			map_at_top;
	Map			*map;
	
	int list_width;
	int cur_x, cur_y;
	bool text_editor_mode;
		
	void draw_editor();
	void show_help();
	void draw_select_screen(bool flip=true);
	void edit_map( int number );
	bool new_map();
	CL_String get_new_author();
	void clip_cursor();
};

#endif


