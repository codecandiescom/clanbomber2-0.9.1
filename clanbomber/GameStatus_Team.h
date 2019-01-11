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


#ifndef GAMESTATUS_TEAM_H
#define GAMESTATUS_TEAM_H

#include "GameStatus.h"

class ClanBomberApplication;

class GameStatus_Team :public GameStatus
{
public: 
	GameStatus_Team(ClanBomberApplication* _app);
	void show();
	void	analyze_game();
	~GameStatus_Team();
	static void draw();
	
	void show_enter_chat_message_background();
protected:
	int team_count[4];
	int team_points[4];

	typedef struct
	{
		unsigned char r,g,b;
	} team_color;
	
	static team_color team_colors[4];
				
	static char* team_names[4];
};

#endif
