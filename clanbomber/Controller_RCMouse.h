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
// $Id: Controller_RCMouse.h,v 1.2 2002/01/14 05:18:08 dok666 Exp $

#ifndef Controller_RCMouse_h
#define Controller_RCMouse_h


#include "Controller.h"
#include <directfb.h>

class Controller_RCMouse : public Controller
{
public:
	Controller_RCMouse (int id);
	virtual ~Controller_RCMouse ();
	
	void update();
	void reset();
	bool is_left();
	bool is_right();
	bool is_up();
	bool is_down();
	bool is_bomb();
	
private:
    bool put_bomb;
	bool left;
	bool right;
	bool up;
	bool down;	
	
	bool dir_pressed;

    int axis_x;
    int axis_y;

    IDirectFBInputDevice *rcmouse;
    IDirectFBEventBuffer *rcbuffer;
};
#endif
