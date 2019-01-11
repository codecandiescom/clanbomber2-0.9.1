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
// $Id: Controller_Joystick.h,v 1.2 2001/10/11 20:49:56 der_fisch Exp $

#ifndef Controller_Joystick_h
#define Controller_Joystick_h

#include "Controller.h"

class Bomber;

class Controller_Joystick : public Controller
{
public:
	Controller_Joystick (int joystick_nr);
	virtual ~Controller_Joystick () {};

	void update();
	void reset();
	bool is_left();
	bool is_right();
	bool is_up();
	bool is_down();
	bool is_bomb();

private:
//	CL_InputDevice*	joystick;
	IDirectFBInputDevice    *dfb_joydev;
	
//	CL_List<CL_InputAxis>	axes;
//	CL_List<CL_InputButton>	buttons;
	bool bomb_button_down;
	
	bool left;
	bool right;
	bool up;
	bool down;	
	
	bool dir_pressed;
			
};

#endif




