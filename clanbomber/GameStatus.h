/***************************************************************************
                          GameStatus.h  -  description                              
                             -------------------                                         
    begin                : Fri Mar 19 1999                                           
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


#ifndef GAMESTATUS_H
#define GAMESTATUS_H

class ClanBomberApplication;
class Bomber;

class GameStatus
{
public: 
	GameStatus(ClanBomberApplication* _app);
	virtual void	show();
	virtual bool	get_end_of_game();
	virtual void	analyze_game();
	virtual ~GameStatus();

	static void draw();
	
	void show_enter_chat_message_background();
protected:
 	ClanBomberApplication*	app;
	
	bool		end_of_game;
	Bomber*	winner;
};

#endif
