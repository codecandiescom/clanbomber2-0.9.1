/***************************************************************************
                   Controller_Mouse.cpp  -  description
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
// $Id: Controller_RCMouse.cpp,v 1.4 2002/05/28 00:17:24 dok666 Exp $

#include "ClanBomber.h"
#include "Controller_RCMouse.h"

DFBEnumerationResult input_enum_callback( unsigned int id, DFBInputDeviceDescription desc, void *data )
{
	if (desc.type & DIDTF_REMOTE) {
		*((int*)data) = id;
	}

	return DFENUM_OK; 
}

Controller_RCMouse::Controller_RCMouse(int id) : Controller()
{
	DFBResult ret;

	if (id < 0) {
		dfb->EnumInputDevices( dfb, input_enum_callback, &id );
	}

	if (id < 0) {
		cerr << "FATAL: Controller_RCMouse could not find a remote device!\n";
	}
	ret = dfb->GetInputDevice( dfb, id, &rcmouse );
	if (ret) {
		DirectFBErrorFatal( "Controller_RCMouse: GetInputDevice", ret );
	}
	rcmouse->CreateEventBuffer( rcmouse, &rcbuffer );

	reset();
}

Controller_RCMouse::~Controller_RCMouse()
{
	rcmouse->Release( rcmouse );
	rcbuffer->Release( rcbuffer );
}

void Controller_RCMouse::update()
{
	DFBInputEvent evt;

	put_bomb = false;

	while (rcbuffer->GetEvent( rcbuffer, DFB_EVENT(&evt) ) == DFB_OK) {
		switch (evt.type) {
			case DIET_KEYPRESS:
				if (evt.key_symbol != DIKS_OK) {
					break;
				}
			case DIET_BUTTONPRESS:
				put_bomb = true;
				break;
			case DIET_AXISMOTION:
				if (evt.flags & DIEF_AXISREL) {
					switch (evt.axis) {
						case DIAI_X:
							axis_x += evt.axisrel;
							break;
						case DIAI_Y:
							axis_y += evt.axisrel;
							break;
						default:
							;
					}
				}
				break;
			default:
				;
		}
	}

	if (active) {
		bool new_left  = (axis_x < -2);
		bool new_right = (axis_x >  2);
		bool new_up    = (axis_y < -2);
		bool new_down  = (axis_y >  2);

		if (new_left || new_right || new_up || new_down) {
			dir_pressed = true;

			if (new_left && new_up) {
				if (down || up) {
					new_left = false;
				}
				if (right || left) {
					new_up = false;
				}
			}
			else if (new_left && new_down) {
				if (down || up) {
					new_left = false;
				}
				if (right || left) {
					new_down=false;
				}
			}
			else if (new_right && new_up) {
				if (down || up) {
					new_right = false;
				}
				if (right || left) {
					new_up = false;
				}
			}
			else if (new_right && new_down) {
				if (down || up) {
					new_right = false;
				}
				if (right || left) {
					new_down = false;
				}
			}
			if (reverse) {
				left = new_right;
				right = new_left;
				up = new_down;
				down = new_up;         
			}
			else {
				left = new_left;
				right = new_right;
				up = new_up;
				down = new_down;
			}
		}
		else {
			dir_pressed = false;
		}
	}
	else {
		left = right = up = down = false;
	}

	axis_x /= 2;
	axis_y /= 2;
}

void Controller_RCMouse::reset()
{
	put_bomb = false;
	reverse = false;
	dir_pressed = false;
	axis_x = axis_y = 0;

	rcbuffer->Reset( rcbuffer );
}

bool Controller_RCMouse::is_left()
{
	return left && dir_pressed && active;
}

bool Controller_RCMouse::is_right()
{
	return right && dir_pressed && active;
}

bool Controller_RCMouse::is_up()
{
	return up && dir_pressed && active;
}

bool Controller_RCMouse::is_down()
{
	return down && dir_pressed && active;
}

bool Controller_RCMouse::is_bomb()
{
	if (!active) {
		return false;
	}

	switch (bomb_mode) {
		case NEVER:
			return false;
		case ALWAYS:
			return true;
		default:
			break;
	}

	return put_bomb;
}
