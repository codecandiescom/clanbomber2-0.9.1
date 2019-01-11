/***************************************************************************
                          Debug.cpp  -  description
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
// $Id: Debug.cpp,v 1.5 2003/06/12 00:39:46 xmassx Exp $

#include "ClanBomber.h"
#include "Debug.h"

#include "Timer.h"
#include "Config.h"
#include "Controller.h"
#include "Map.h"
#include "Bomber.h"

Debug::Debug( int _x, int _y, ClanBomberApplication *_app ) : GameObject( _x, _y, _app )
{
	app->objects.add( this );
	current_map = 1;
	key_F2 = DIKS_DOWN;
	key_F3 = DIKS_DOWN;
	key_divide = DIKS_DOWN;
	key_multiply = DIKS_DOWN;		
}

Debug::~Debug()
{
}

void Debug::show()
{
}

void Debug::act()
{
	DFBInputDeviceKeyState temp_ks;
	
	keyboard->GetKeyState( keyboard, DIKI_KP_MULT, &temp_ks);	
	if ((temp_ks == DIKS_DOWN) && (key_multiply == DIKS_UP)) {
		app->map->load_next_valid();
	}
	key_multiply = temp_ks;
	
	keyboard->GetKeyState( keyboard, DIKI_KP_DIV, &temp_ks);	
	if ((temp_ks == DIKS_DOWN) && (key_divide == DIKS_UP)) {
		app->map->load_previous_valid();
	}
	key_divide = temp_ks;

	keyboard->GetKeyState( keyboard, DIKI_F2, &temp_ks);	
	if ((temp_ks == DIKS_DOWN) && (key_F2 == DIKS_UP)) {
		Bomber* bomber = new Bomber(((rand()%(MAP_WIDTH-2))+1)*40,((rand()%(MAP_HEIGHT-2))+1)*40,Bomber::GREEN, Controller::create(Controller::AI), "Debug Robi", 0,0, app);
		bomber->fly_to(app->map->get_passable());
		bomber->controller->activate();
	}
	key_F2 = temp_ks;

	keyboard->GetKeyState( keyboard, DIKI_F3, &temp_ks);	
	if ((temp_ks == DIKS_DOWN) && (key_F3 == DIKS_UP)) {
		Bomber* bomber = new Bomber(((rand()%(MAP_WIDTH-2))+1)*40,((rand()%(MAP_HEIGHT-2))+1)*40,Bomber::RED, Controller::create(Controller::AI_mass), "Debug Robi mass", 0,0, app);
		bomber->fly_to(app->map->get_passable());
		bomber->controller->activate();
	}
	key_F3 = temp_ks;
}
