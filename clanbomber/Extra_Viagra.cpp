/***************************************************************************
                          FILENAME  -  description                              
                             -------------------                                         
    begin                : in 1999                                           
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

#include "ClanBomber.h"
#include "Extra_Viagra.h"

#include "Bomber.h"
#include "Disease_PutBomb.h"
#include "Server.h"

Extra_Viagra::Extra_Viagra( int _x, int _y, ClanBomberApplication *_app ) : Extra( _x, _y, _app )
{
	destroyable = false;
	sprite_nr = 6;
	if (ClanBomberApplication::is_server()) {
		ClanBomberApplication::get_server()->send_SERVER_ADD_EXTRA(object_id, _x, _y, get_ExtraType());
	}
}

Extra_Viagra::~Extra_Viagra()
{
}

void Extra_Viagra::effect( Bomber* bomber )
{
	bomber->loose_disease();
	bomber->infect( new Disease_PutBomb(bomber) );
	PLAY_PAN(Resources::Extras_horny());
}
