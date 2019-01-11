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


#ifndef DISEASE_FAST_H
#define DISEASE_FAST_H

#include "Disease.h"

/**
  *@author Andreas Hundt, Denis Oliver Kropp
  */

class Disease_Fast : public Disease
{
public: 
	Disease_Fast(Bomber* bomber);
	void start();
	void stop();
	~Disease_Fast();
	
	Disease* spawn(Bomber* _bomber);
 	Extra* spawn_extra(int _x, int _y);

	DiseaseType get_DiseaseType()
	{
         	return FAST;
	}
};

#endif




















