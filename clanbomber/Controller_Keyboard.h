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
// $Id: Controller_Keyboard.h,v 1.1 2001/09/20 22:59:49 der_fisch Exp $

#ifndef Controller_Keyboard_h
#define Controller_Keyboard_h


#include "Controller.h"
#include <directfb.h>

class Controller_Keyboard : public Controller
{
public:
	Controller_Keyboard (int keymap_nr);
	virtual ~Controller_Keyboard () {};
	
	void update();
	void reset();
	bool is_left();
	bool is_right();
	bool is_up();
	bool is_down();
	bool is_bomb();
	
private:
	DFBInputDeviceKeyIdentifier left_key;
	DFBInputDeviceKeyIdentifier right_key;
	DFBInputDeviceKeyIdentifier up_key;
	DFBInputDeviceKeyIdentifier down_key;
	DFBInputDeviceKeyIdentifier bomb_key;
	DFBInputDeviceKeyState bomb_key_state;
};
#endif
