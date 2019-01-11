/***************************************************************************
                          PlayerSetup.h  -  description                              
                             -------------------                                         
    begin                : Fri May 14 1999                                           
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


#ifndef PLAYERSETUP_H
#define PLAYERSETUP_H


/**
  *@author Andreas Hundt, Denis Oliver Kropp
  */

class ClanBomberApplication;

class PlayerSetup
{
public: 
	PlayerSetup(ClanBomberApplication* _app);
	~PlayerSetup();

	void exec();
	
protected:
	ClanBomberApplication*	app;
	
	int	cur_row, cur_col;
	
	void draw(bool fick=false);
	void enter_name();
	void handle_enter();
};

#endif










