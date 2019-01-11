/***************************************************************************
                          Menu.h  -  description
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
// $Id: Menu.h,v 1.7 2003/12/27 01:43:09 xmassx Exp $

#ifndef Menu_h
#define Menu_h

#include "Client.h"

#include "array.h"

class MenuItem
{
friend class Menu;
public:
	MenuItem( const CL_String _text, int _id, int _parent );
	virtual ~MenuItem() {};

	void add_child( MenuItem* child );
	bool has_children();
	int get_id() const;
	int get_parent() const;
	CL_String get_text();
	char* get_string();
	void set_text( const CL_String _text );

	typedef enum
	{
		MT_NORMAL,
		MT_VALUE,
		MT_STRING,
		MT_STRINGLIST
	 } MENUTYPE;

	virtual MENUTYPE get_type() { return MT_NORMAL; };

protected:
	CL_String		text;
	int			id;
	int			parent;
	CL_List<MenuItem>	children;
};

class MenuItem_Value : public MenuItem
{
friend class Menu;
public:
	MenuItem_Value( const CL_String _text, int _id, int _parent, int _min, int _max, int _value );
	virtual ~MenuItem_Value() {};

	int get_value();
	void set_value(int _value);
	int get_min();
	int get_max();
	void set_min( int _min );
	void set_max( int _max );
	void inc_value();
	void dec_value();

	virtual MENUTYPE get_type() { return MT_VALUE; };

protected:
	void test_value();
	int value;
	int min;
	int max;
};


class MenuItem_StringList : public MenuItem
{
friend class Menu;
public:
	MenuItem_StringList( const CL_String _text, int _id, int _parent, CL_Array<CL_String> _string_list, int _value );
	virtual ~MenuItem_StringList() {};

	char *get_sel_string();
	CL_Array<CL_String> get_strings();
	void set_strings(CL_Array<CL_String> _string_list);
	void inc_value();
	void dec_value();
	int get_value();
	void set_value(int _value);

	virtual MENUTYPE get_type() { return MT_STRINGLIST; };

protected:
	CL_Array<CL_String> string_list;
	void test_value();
	int value;
	int min;
	int max;
};

class Menu
{
public:
	Menu( const CL_String& name, ClanBomberApplication* _app );
	~Menu();

	void add_item( const CL_String& text, int id, int parent=-1 );
	void add_value( const CL_String& text, int id, int parent, int min, int max, int value );
	void add_string( const CL_String& text, int id, int parent, CL_String string );
	void add_stringlist( const CL_String& text, int id, int parent, CL_Array<CL_String> string_list, int cur_string );

	void redraw(bool options_menu_hack, int yoffset=0);
	int execute(bool options_menu_hack);

	void scroll_in();
	void scroll_out();
	MenuItem* get_item_by_id( int id );

	void go_to_game_menu(bool server);
	void execute_options_menu_hack();
	void backup_options_values();
	void restore_options_values();
	void save_common_options(int id, bool options_menu_hack, bool force_save);
	void set_left_netgame_setup();
protected:
	ClanBomberApplication* app;
	CL_List<MenuItem> items;
	int current_run_id;
	int current_selection;

	MenuItem_Value* options_values[MENU_OPTIONS_NUMBER];
	bool left_netgame_setup;
};

#endif
