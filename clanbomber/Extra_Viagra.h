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


#ifndef EXTRA_VIAGRA_H
#define EXTRA_VIAGRA_H

#include "Extra.h"

class Extra_Viagra : public Extra
{
public:
	Extra_Viagra( int _x, int _y, ClanBomberApplication *_app );
	~Extra_Viagra();

       	ExtraType get_ExtraType()
	{
		return VIAGRA;
	}

protected:
	virtual void effect( Bomber* bomber );
};

#endif



