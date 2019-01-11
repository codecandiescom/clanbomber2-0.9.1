/***************************************************************************
                          Controller.h  -  description
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
// $Id: Controller.h,v 1.2 2003/06/12 00:39:46 xmassx Exp $

#ifndef Controller_h
#define Controller_h

class Bomber;

class Controller
{
public:
	Controller();
	virtual ~Controller() {};

	typedef enum
	{
		AI,
		AI_mass,
		KEYMAP_1,
		KEYMAP_2,
		KEYMAP_3,
		RCMOUSE,
		JOYSTICK_1,
		JOYSTICK_2,
		JOYSTICK_3,
		JOYSTICK_4,
		JOYSTICK_5,
		JOYSTICK_6,
		JOYSTICK_7,
		JOYSTICK_8
	} CONTROLLER_TYPE;
	
	enum
	{
		NORMAL,
		ALWAYS,
		NEVER
	} bomb_mode;
	
	static Controller*	create( CONTROLLER_TYPE _type );
	
	void attach(Bomber* _bomber);

	virtual void update() = 0;
	virtual void reset() = 0;
	virtual bool is_left() = 0;
	virtual bool is_right() = 0;
	virtual bool is_up() = 0;
	virtual bool is_down() = 0;
	virtual bool is_bomb() = 0;
	void deactivate();
	void activate();
	void revert();
	
	void bomb_always();
	void bomb_normal();
		
	Bomber*	bomber;
	bool		active;
	bool		reverse;
	bool put_bomb;

	CONTROLLER_TYPE get_type();
	CONTROLLER_TYPE c_type;
};

#endif




