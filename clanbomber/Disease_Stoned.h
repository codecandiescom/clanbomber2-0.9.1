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


#ifndef DISEASE_STONED_H
#define DISEASE_STONED_H

#include "Disease.h"

/**
  *@author Andreas Hundt, Denis Oliver Kropp
  */

class Disease_Stoned : public Disease
{
public:
	Disease_Stoned(Bomber* bomber);
	void start();
	void stop();
	~Disease_Stoned();
	
	Disease* spawn(Bomber* _bomber);
 	Extra* spawn_extra(int _x, int _y);

	DiseaseType get_DiseaseType()
  	{
         	return STONED; 
        }
};

#endif


















