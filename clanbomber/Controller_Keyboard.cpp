/***************************************************************************
                   Controller_Keyboard.cpp  -  description
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
// $Id: Controller_Keyboard.cpp,v 1.3 2002/05/28 00:17:24 dok666 Exp $

#include "ClanBomber.h"
#include "Controller_Keyboard.h"

Controller_Keyboard::Controller_Keyboard(int keymap_nr) : Controller()
{
	switch (keymap_nr) {
	case 0:
		left_key = DIKI_LEFT;
		right_key = DIKI_RIGHT;
		up_key = DIKI_UP;
		down_key = DIKI_DOWN;
		bomb_key = DIKI_ENTER;
		break;
	case 1:
		left_key = DIKI_A;
		right_key = DIKI_D;
		up_key = DIKI_W;
		down_key = DIKI_S;
		bomb_key = DIKI_TAB;
		break;
	case 2:
		left_key = DIKI_J;
		right_key = DIKI_L;
		up_key = DIKI_I;
		down_key = DIKI_K;
		bomb_key = DIKI_SPACE;
		break;
	case 3:
		left_key = DIKI_4;
		right_key = DIKI_6;
		up_key = DIKI_8;
		down_key = DIKI_5;
		bomb_key = DIKI_0;
		break;
	}
	c_type = KEYMAP_1;
	reset();
}

void Controller_Keyboard::update()
{
	DFBInputDeviceKeyState bk;
	
	keyboard->GetKeyState( keyboard, bomb_key, &bk);
	
	if ( (bk==DIKS_DOWN) && (bomb_key_state==DIKS_UP)) {
		put_bomb = true;
	}
	else {
		put_bomb = false;
	}
	keyboard->GetKeyState( keyboard, bomb_key, &bomb_key_state );
}

void Controller_Keyboard::reset()
{
	put_bomb = false;
	bomb_key_state = DIKS_DOWN;
	reverse = false;
}

bool Controller_Keyboard::is_left()
{
	DFBInputDeviceKeyState ks;

	if (reverse) {
		keyboard->GetKeyState( keyboard, right_key, &ks);
	}
	else {
		keyboard->GetKeyState( keyboard, left_key, &ks);
	}
	return ( ks==DIKS_DOWN ) && active;
}

bool Controller_Keyboard::is_right()
{
	DFBInputDeviceKeyState ks;

	if (reverse) {
		keyboard->GetKeyState( keyboard, left_key, &ks);
	}
	else {
		keyboard->GetKeyState( keyboard, right_key, &ks);
	}
	return ( ks==DIKS_DOWN ) && active;
}

bool Controller_Keyboard::is_up()
{
	DFBInputDeviceKeyState ks;

	if (reverse) {
		keyboard->GetKeyState( keyboard, down_key, &ks);
	}
	else {
		keyboard->GetKeyState( keyboard, up_key, &ks);
	}
	return ( ks==DIKS_DOWN ) && active;
}

bool Controller_Keyboard::is_down()
{
	DFBInputDeviceKeyState ks;

	if (reverse) {
		keyboard->GetKeyState( keyboard, up_key, &ks);
	}
	else {
		keyboard->GetKeyState( keyboard, down_key, &ks);
	}
	return ( ks==DIKS_DOWN ) && active;
}

bool Controller_Keyboard::is_bomb()
{
	switch (bomb_mode) {
		case NEVER:
			return false;
		case ALWAYS:
			return true;
		default:
		break;
	}
	return put_bomb && active;
}
