/***************************************************************************
                          Extra_Power.cpp  -  description
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
// $Id: Extra_Power.cpp,v 1.5 2003/06/25 20:26:23 der_fisch Exp $

#include "ClanBomber.h"
#include "Extra_Power.h"

#include "Bomber.h"
#include "Server.h"

Extra_Power::Extra_Power( int _x, int _y, ClanBomberApplication *_app ) : Extra( _x, _y, _app )
{
	sprite_nr = 1;
	if (ClanBomberApplication::is_server()) {
		ClanBomberApplication::get_server()->send_SERVER_ADD_EXTRA(object_id, _x, _y, get_ExtraType());
	}
}

Extra_Power::~Extra_Power()
{
}

void Extra_Power::effect( Bomber* bomber )
{
	bomber->loose_disease();
	bomber->gain_extra_power();
	PLAY_PAN(Resources::Extras_wow());
}
