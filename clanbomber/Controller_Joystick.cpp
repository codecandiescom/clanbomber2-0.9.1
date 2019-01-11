/***************************************************************************
                          Controller.cpp  -  description
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
// $Id: Controller_Joystick.cpp,v 1.3 2001/10/25 11:52:56 xmassx Exp $

#include "ClanBomber.h"
#include "Controller_Joystick.h"

Controller_Joystick::Controller_Joystick(int joystick_nr) : Controller()
{
	dfb->GetInputDevice( dfb, DIDID_JOYSTICK, &dfb_joydev );
	
	c_type = JOYSTICK_1;
}

void Controller_Joystick::update()
{
	if (active) {
		DFBInputDeviceButtonMask buttonmask;
		bool new_left;  
		bool new_right; 
		bool new_up;    
		bool new_down; 
		int dirvalue;

		dfb_joydev->GetButtons( dfb_joydev, &buttonmask );
				
		dfb_joydev->GetAxis( dfb_joydev,(DFBInputDeviceAxisIdentifier)0, &dirvalue);
		new_left  = (dirvalue < -10000);
		new_right = (dirvalue >  10000);

		dfb_joydev->GetAxis( dfb_joydev,(DFBInputDeviceAxisIdentifier)1, &dirvalue);
		new_up    = (dirvalue < -10000);
		new_down  = (dirvalue >  10000);	
		
		if (buttonmask && !bomb_button_down) {
			put_bomb = true;
		}
		else {
			put_bomb = false;
		}
		bomb_button_down = buttonmask;

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
					new_down = false;
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
}

void Controller_Joystick::reset()
{
	put_bomb = false;
	bomb_button_down = true;
	reverse = false;
}

bool Controller_Joystick::is_left()
{
	return left && dir_pressed;
}

bool Controller_Joystick::is_right()
{
	return right && dir_pressed;
}

bool Controller_Joystick::is_up()
{
	return up && dir_pressed;
}

bool Controller_Joystick::is_down()
{
	return down && dir_pressed;
}

bool Controller_Joystick::is_bomb()
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
