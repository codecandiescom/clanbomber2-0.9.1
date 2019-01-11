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


#ifndef CREDITS_H
#define CREDITS_H

/**
  *@author Andreas Hundt, Denis Oliver Kropp
  */

class Credits
{
public: 
	Credits( ClanBomberApplication *app );
	~Credits();
	
	void exec();
	
protected:
	ClanBomberApplication	*app;
	void draw();
	void draw_objects();
	
	CL_List<CL_String> text;
	float	yoffset;
	int		speed;
	bool		stopped;

/*
	CL_SoundBuffer_Session ss_forward;
	CL_SoundBuffer_Session ss_rewind;
*/

};

#endif







