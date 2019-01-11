/***************************************************************************
                          ClanBomber.h  -  description
                             -------------------
    begin                : ?
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
// $Id: ClanBomber.h,v 1.34 2007/10/15 13:58:16 der_fisch Exp $

#ifndef ClanBomber_h
#define ClanBomber_h

#include "Resources.h"
#include <directfb.h>
#include <fusionsound.h>

class Menu;
class Debug;
class Observer;
class Map;
class Bomb;
class Bomber;
class GameObject;
class Server;
class Client;
class Mutex;
class Event;
class ClientSetup;
class ServerSetup;
class Chat;

#include <config.h>

extern IDirectFB *dfb;
extern IFusionSound *sound;

extern IDirectFBSurface *primary;
extern IDirectFBInputDevice *keyboard;
extern IDirectFBEventBuffer *keybuffer;

#include "clanstring.h"
#include "link.h"

enum
{
	MENU_GAME,
	MENU_OPTIONS,
	MENU_GRAPHICS,
	MENU_CREDITS,
	MENU_PLAYER_SETUP,
	MENU_EXTRA_VALUES,
	MENU_EXTRA_ONOFF,
	MENU_EXIT,
	MENU_CONTROLLER,
	MENU_HELP,
	MENU_DISEASE_ONOFF,
	MENU_MAP_EDITOR,
	MENU_TIMING,

	CONFIG_START_BOMBS,
	CONFIG_START_POWER,
	CONFIG_START_SKATES,
	CONFIG_START_KICK,
	CONFIG_START_GLOVE,

	CONFIG_BOMBS,
	CONFIG_POWER,
	CONFIG_SKATES,
	CONFIG_KICK,
	CONFIG_GLOVE,

	CONFIG_MAX_BOMBS,
	CONFIG_MAX_POWER,
	CONFIG_MAX_SKATES,

	CONFIG_POINTS,
	CONFIG_MAP_SEL,
	CONFIG_ROUND_TIME,

	CONFIG_JOINT,
	CONFIG_VIAGRA,
	CONFIG_KOKS,

	CONFIG_KIDS_MODE,
	CONFIG_SHAKE,
	CONFIG_CORPSE_PARTS,
	CONFIG_RANDOM_POSITIONS,
	CONFIG_RANDOM_MAP_ORDER,

	CONFIG_BOMB_COUNTDOWN,
	CONFIG_BOMB_DELAY,
	CONFIG_BOMB_SPEED,

	CONFIG_RANDOM_POSITIONS_DUPLICATE,
	CONFIG_RANDOM_MAP_ORDER_DUPLICATE,
	CONFIG_POINTS_DUPLICATE,
	CONFIG_ROUND_TIME_DUPLICATE,

	LOCALGAME_START,
	SERVERGAME_START,
	CLIENTGAME_START
};

typedef enum
{
	DIR_NONE  = -1,
	DIR_DOWN  = 0,
	DIR_LEFT  = 1,
	DIR_UP    = 2,
	DIR_RIGHT = 3
} Direction;


class ClanBomberApplication
{
friend class Debug;
public:
	ClanBomberApplication();
	~ClanBomberApplication();
	int main(int argc, char **argv);
	Map* map;
	CL_List<GameObject> objects;
	CL_List<Bomber> bomber_objects;

	bool init_server_game();
	bool init_client_game();
	int get_server_status();
	static int run_server_with_players();
	static bool is_server();
	static bool is_client();

	static Server* get_server();
	static Client* get_client();
	static ServerSetup* get_server_setup();
	static ClientSetup* get_client_setup();
	static Chat* get_chat();
	static Menu* get_menu();

	void set_client_disconnected_from_server(bool d);
	bool is_client_disconnected_from_server();
	void set_client_connecting_to_new_server(bool c);
	bool is_client_connecting_to_new_server();
	void client_to_server();
	void make_observer();
	void make_map();
	Observer* get_observer();
	Map* get_map();
	bool paused_game();
	void set_pause_game(bool paused);
	void reload_map();
	void delete_all_game_objects();
    
	GameObject* get_object_by_id(int object_id);
	Bomb* activate_suspended_client_bomb_by_id(int object_id);

	static unsigned short get_next_object_id();

	static CL_String get_map_path();
	static CL_String get_local_map_path();
	static void lock();
	static void unlock();
	static void wait();
	static void signal();

	bool bombers_received_by_client;
	static int get_server_frame_counter();
	static void inc_server_frame_counter();
	CL_List<Bomb> suspended_client_bombs;
protected:
	void init_dfb(int argc, char** argv);
	void init_audio();
	void run_intro();
	bool init_game(); // single player init
	void deinit_game();
	void show_tutorial();

	void run_game();
	void show_all();
	void act_all();
	void delete_some();
	
	void start_net_game();
	void init_net_game();

	Server* cb_server;
	Client* cb_client;
	ClientSetup* client_setup_menu;
	ServerSetup* server_setup_menu;
	Chat* chat;
	Menu* menu;

	Observer* observer;
//	CL_SoundBuffer* wav;
//	CL_SoundBuffer_Session* ses;
	
	static CL_String map_path;
	static CL_String local_map_path;

	bool key_F1;
	bool show_fps;
	int fps;
	int frame_count;
	float frame_time;
	int server_status;
	static int server_frame_counter;
	bool pause_game;
	bool client_disconnected_from_server;
	bool client_connecting_to_new_server;
	Mutex* cb_mutex;
	Event* cb_event;
};

#endif


// we like profanity in open source software, please read the following words carefully:
// fuck, pussy, dick, sperm, motherfucker
//
// hope our source code will now be censored by all governments that suck.


/* Global macros and functions (to be implemented in ClanLib?) */

// this is for reading strings from an ascii file

class CL_InputSource;
class CL_InputBuffer;
int enter_string( CL_String &string );


#define PLAY_PAN(st) \
                if(st) \
		{ \
			(st)->Play((st),FSPLAY_NOFX); \
		}


#define PLAY_CENTER(st) if((st))(st)->Play((st),FSPLAY_NOFX);
			

#ifdef WITH_DEBUG
 #define dout(dstring) cout << endl << "__ " << __FILE__ << ":" << __LINE__ << " __" << endl << dstring << endl
#else
 #ifdef WITH_DEBUG_SIMPLE
  #define dout(dstring) cout << dstring << endl
 #else
  #define dout(dstring)
 #endif
#endif

#define min(a,b) ((a<b) ? (a) : (b))
#define max(a,b) ((a>b) ? (a) : (b))



#define DFBCHECK(x...)	{							\
				DFBResult err = x;				\
				if (err != DFB_OK) {				\
					cerr <<  __FILE__ << __LINE__  << endl;	\
					DirectFBErrorFatal( #x, err );		\
				}						\
			}
						
