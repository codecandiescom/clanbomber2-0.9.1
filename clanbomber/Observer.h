/***************************************************************************
                          Observer.h  -  description
                             -------------------                                         
    begin                : Sat Mar 6 1999                                           
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
// $Id: Observer.h,v 1.3 2003/12/31 17:59:39 xmassx Exp $


#ifndef OBSERVER_H
#define OBSERVER_H

#include "GameObject.h"
#include "GameStatus.h"
#include "Bomber.h"

class ClanBomberApplication;


class Observer : public GameObject
{
public: 
	Observer( int _x, int _y, ClanBomberApplication *_app );
	~Observer();
	
	virtual void show();
	virtual void act();
	virtual void fall() {}; // does not fall ;-)
	
	bool end_of_game_requested();
	int active_players();
	float get_round_time() const;
	void reset_round_time();
	void make_client_game_status();
	GameStatus* get_game_status();
	
	ObjectType get_type() const
	{
		return OBSERVER;
	}
	
	typedef enum
	{
		NORMAL,
		TEAM
	} Mode;

	void set_client_game_runs(bool is_running);
	bool client_game_is_running();

protected:
	bool		end_of_game;
	float		round_time;
	Mode		mode;
	GameStatus	*game_status;
	bool		play_hurryup;
	bool		first_destruction;
	int		repaint_hack;
	void kill_all_bombers();

	bool client_game_runs;
};

#endif


