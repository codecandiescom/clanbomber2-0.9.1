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


#ifndef CORPSE_PART_H
#define CORPSE_PART_H

class ClanBomberApplication;

#include "GameObject.h"

/**
  *@author Andreas Hundt, Denis Oliver Kropp
  */

class Corpse_Part : public GameObject
{
public: 
	Corpse_Part(int _x, int _y, ClanBomberApplication* _app);
	virtual ~Corpse_Part();

	virtual void act();
	
	ObjectType get_type() const
	{
		return CORPSE_PART;
	}
	
protected:
	float countdown;
};

#endif
