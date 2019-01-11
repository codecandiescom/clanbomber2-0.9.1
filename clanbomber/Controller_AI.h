/***************************************************************************
                          Controller_AI.h  -  description                              
                             -------------------                                         
    begin                : Sat Apr 10 1999                                           
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
// $Id: Controller_AI.h,v 1.2 2001/09/22 15:49:44 der_fisch Exp $

#ifndef Controller_AI_h
#define Controller_AI_h

// Ratings

#define RATING_EXTRA	   30	// simply... an extra ;)

#define RATING_HOT		 -100	// bomb will explode and reach this field
#define RATING_X		 -666	// absolute death, e.g. explosion, hole

#define RATING_BLOCKING -1000	// never run into walls


#define DRATING_ENEMY	 150
#define DRATING_BOX	  	  20
#define DRATING_DISEASE	  10
#define DRATING_DBOX	 -50
#define DRATING_EXTRA	 -90
#define DRATING_BOMB	-100
#define DRATING_FRIEND	-200


#include "Map.h"

#include "Controller.h"

class ClanBomberApplication;
class Bomber;

class Controller_AI;

class Job
{
public:
	Job( Controller_AI* _controller );
	virtual ~Job();
	bool is_finished();
	bool is_obsolete();
	virtual void execute();
	virtual void init();
	
protected:
	bool	finished;
	bool	obsolete;
	
	Controller_AI*			controller;
	Bomber*				bomber;
	ClanBomberApplication*	app;
};

class Job_Go : public Job
{
public:
	Job_Go( Controller_AI* _controller, int _dir, int _distance = 1 );
	virtual ~Job_Go();
	void execute();
	void init();
	
protected:
	int dir;
	int distance;
	int start;
};

class Job_PutBomb : public Job
{
public:
	Job_PutBomb( Controller_AI* _controller );
	virtual ~Job_PutBomb();
	void execute();
};

class Job_Wait : public Job
{
public:
	Job_Wait( Controller_AI* _controller, float _duration );
	virtual ~Job_Wait();
	void execute();
protected:
	float duration;
};

class Controller_AI : public Controller
{
friend class Job_Go;
friend class Job_PutBomb;
friend class Job_Wait;
public:
	Controller_AI ();
	virtual ~Controller_AI () {};

	void update();
	void reset();
	bool is_left();
	bool is_right();
	bool is_up();
	bool is_down();
	bool is_bomb();
	
	int	dir;
	bool	put_bomb;

private:
	CL_List<Job>			jobs;
	bool job_ready();
	void do_job();
	void find_new_jobs();
	void clear_all_jobs();
	bool is_hotspot( int x, int y );
	bool is_death( int x, int y );

	int bomber_rating( int x, int y );
	int extra_rating( int x, int y );
	void apply_bomb_rating( int x, int y, int power, float countdown, Direction dir );
	
	bool avoid_bombs();
	bool free_extras( int max_distance = 666 );

	void generate_rating_map();
	bool find_way( int dest_rating = 0, int avoid_rating = RATING_X, int max_distance = 666 );

	int	rmap[MAP_WIDTH][MAP_HEIGHT];
	Map	*map;
};



#endif
