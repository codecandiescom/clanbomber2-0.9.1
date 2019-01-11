/***************************************************************************
                          ClanBomber.cpp  -  description
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
// $Id: ClanBomber.cpp,v 1.80 2007/10/15 13:58:16 der_fisch Exp $

#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include <directfb.h>

#include "ClanBomber.h"

#include "Controller.h"
#include "Debug.h"
#include "Observer.h"
#include "Config.h"
#include "Menu.h"
#include "Timer.h"
#include "PlayerSetup.h"
#include "ServerSetup.h"
#include "Server.h"
#include "Chat.h"
#include "ClientSetup.h"
#include "Client.h"
#include "Map.h"
#include "Credits.h"
#include "MapEditor.h"
#include "MapTile.h"
#include "Bomber.h"
#include "MapSelector.h"
#include "Bomb.h"

#include <config.h>
#include <dirent.h>
#include <sys/stat.h>

#include <time.h>

#include "Mutex.h"
#include "Event.h"

ClanBomberApplication *app;

IDirectFB            *dfb       = NULL;
IFusionSound         *sound     = NULL;
IDirectFBSurface     *primary   = NULL;
IDirectFBInputDevice *keyboard  = NULL;
IDirectFBEventBuffer *keybuffer = NULL;
	
CL_String ClanBomberApplication::map_path;
CL_String ClanBomberApplication::local_map_path;

int ClanBomberApplication::server_frame_counter = 0;

static unsigned short next_object_id  = 0;
static int run_server_with_players_nr = 0;

ClanBomberApplication::ClanBomberApplication()
{
	cb_server = NULL;
	cb_client = NULL;
	server_status = 0;
	observer = NULL;
	map = NULL;
	cb_mutex = new Mutex();
	cb_event = new Event();
	client_setup_menu = NULL;
	server_setup_menu = NULL;
	pause_game = false;
	client_disconnected_from_server = false;
	client_connecting_to_new_server = false;
	bombers_received_by_client = false;
}

ClanBomberApplication::~ClanBomberApplication()
{
    cout<<"(+) clanbomber: deallocating resources..."<<endl;

    if (keybuffer) {
        keybuffer->Release( keybuffer );
    }
    
    if (sound) {
        sound->Release( sound );
    }

    if (dfb) {
        dfb->Release( dfb );
    }

    if (map) {
        delete map;
        map = NULL;
    }

    delete cb_event;
    cb_event = NULL;
    delete cb_mutex;
    cb_mutex = NULL;

    if (cb_server) {
        delete cb_server;
        cb_server = NULL;
    }

    if (server_setup_menu) {
        delete server_setup_menu;
        server_setup_menu = NULL;
    }

    if (cb_client) { 
        delete cb_client;
        cb_client = NULL;
    }

    if (client_setup_menu) {
        delete client_setup_menu;
        client_setup_menu = NULL;
    }
}

void ClanBomberApplication::init_dfb(int argc, char** argv)
{
	DFBCHECK(DirectFBInit( &argc, &argv ));

	/* create the super interface */
	DFBCHECK(DirectFBCreate( &dfb ));
		
	/* get an interface to the primary keyboard  */
	DFBCHECK(dfb->GetInputDevice( dfb, DIDID_KEYBOARD, &keyboard ));

	/* create an input buffer for key events */
	DFBCHECK(dfb->CreateInputEventBuffer( dfb, DICAPS_KEYS, DFB_FALSE, &keybuffer ));
	
	/* set our cooperative level to DFSCL_FULLSCREEN for exclusive access to the primary layer */
	dfb->SetCooperativeLevel( dfb, DFSCL_FULLSCREEN );

	/* set our desired video mode, ignore failure */
	dfb->SetVideoMode( dfb, 800, 600, 16 );

	/* get the primary surface, i.e. the surface of the primary layer we have exclusive access to */
	{
		DFBSurfaceDescription dsc;
		
		memset( &dsc, 0, sizeof(DFBSurfaceDescription) );
		
		dsc.flags = (DFBSurfaceDescriptionFlags)(DSDESC_CAPS);
		dsc.caps = (DFBSurfaceCapabilities)(DSCAPS_PRIMARY | DSCAPS_FLIPPING /*| DSCAPS_ALPHA*/);
		
		DFBCHECK(dfb->CreateSurface( dfb, &dsc, &primary ));
	}

	int width, height;

	primary->GetSize( primary, &width, &height );

	if (width < 800  ||  height < 600) {
		cerr << "ClanBomber needs at least 800x600!" << endl;
		exit(1);
	}

	dfb->GetInterface (dfb, "IFusionSound", NULL, NULL, (void**) &sound);

	primary->SetDrawingFlags( primary, DSDRAW_NOFX );
	primary->SetColor( primary, 0, 0, 0, 0xFF );
	
	primary->FillRectangle( primary, 0, 0, width, height );
	primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );
	primary->FillRectangle( primary, 0, 0, width, height );
	primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );

	DFBRegion screenrect = { 0, 0, 799, 599 };
	primary->SetClip( primary, &screenrect );		
}

void ClanBomberApplication::start_net_game()
{
    if (is_server()) {
        cb_server->send_SERVER_FULL_BOMBER_CONFIG();
        cb_server->send_SERVER_CONFIG();
    }
    init_net_game();
    run_game();
    deinit_game();
}

bool ClanBomberApplication::paused_game()
{
    return pause_game;
}

void ClanBomberApplication::set_pause_game(bool paused)
{
    pause_game = paused;
}

void ClanBomberApplication::reload_map()
{
	if(map) {
		delete map;
	}
	map = new Map(this);
}

unsigned short ClanBomberApplication::get_next_object_id()
{
    if (next_object_id++ > 65534) {
        next_object_id = 23;
    }
    return next_object_id;
}

bool ClanBomberApplication::is_client_disconnected_from_server()
{
	return client_disconnected_from_server;
}

void ClanBomberApplication::set_client_disconnected_from_server(bool d)
{
	client_disconnected_from_server = d;
}

void ClanBomberApplication::set_client_connecting_to_new_server(bool c)
{
	client_connecting_to_new_server = c;
}

bool ClanBomberApplication::is_client_connecting_to_new_server()
{
	return client_connecting_to_new_server;
}

void ClanBomberApplication::lock()
{
	app->cb_mutex->lock();
}

void ClanBomberApplication::unlock()
{
	app->cb_mutex->unlock();
}

void ClanBomberApplication::wait()
{
	app->cb_event->lock();
	app->cb_event->wait();
	app->cb_event->unlock();
}

void ClanBomberApplication::signal()
{
	app->cb_event->lock();
	app->cb_event->signal();
	app->cb_event->unlock();
}

int ClanBomberApplication::main(int argc, char** argv)
{

	srand( (long)time(NULL) );
	init_dfb( argc, argv );	     	

	show_fps = false;
	key_F1 = false;

	map_path = DATADIR"/maps";

	if (getenv("HOME")) {
		CL_String path;
		path = getenv("HOME");
		path += "/.clanbomber/";
		if (!opendir(path)) {
			mkdir( path, 0755 );
		}

		Config::set_path( path );
		Config::set_filename( "/clanbomber.cfg" );
		
		path += "/maps";
		if (!opendir(path)) {
			mkdir( path, 0755 );
		}
		local_map_path = path;
	}
	
	Resources::init();
	Config::load();
    	
	run_intro();
     	
	primary->SetColor( primary, 0, 0, 0, 0xFF );
	primary->SetDrawingFlags( primary, DSDRAW_NOFX );
	primary->FillRectangle( primary, 0, 0, 800, 600 );
	primary->SetFont( primary, Resources::Font_big() );	
	primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );
	primary->DrawString( primary, "Loading...", -1, 400, 300, DSTF_CENTER );
	primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );
//	sleep (3);
     	
//	wav = NULL;
   	map = NULL;
   	observer = NULL;
	chat = new Chat(this);

   	menu = new Menu( CL_String("Main Menu"), this);

   	menu->add_item( CL_String("New Game"), MENU_GAME );
		menu->add_item( CL_String("Start Local"), LOCALGAME_START, MENU_GAME );
		menu->add_item( CL_String("Create Server"), SERVERGAME_START, MENU_GAME );
		menu->add_item( CL_String("Join Server"), CLIENTGAME_START, MENU_GAME );
		menu->add_item( CL_String("Local Player Setup"), MENU_PLAYER_SETUP, MENU_GAME);
		menu->add_item( CL_String("Map Selection"), CONFIG_MAP_SEL, MENU_GAME );
		menu->add_value( CL_String("Random Bomber Positions"), CONFIG_RANDOM_POSITIONS, MENU_GAME, 0, 1, Config::get_random_positions() );
		menu->add_value( CL_String("Random Map Order"), CONFIG_RANDOM_MAP_ORDER, MENU_GAME, 0, 1, Config::get_random_map_order() );
		menu->add_value( CL_String("Points to win"), CONFIG_POINTS, MENU_GAME, 1, 10, Config::get_points_to_win() );
		menu->add_value( CL_String("Round Time"), CONFIG_ROUND_TIME, MENU_GAME, 5, 300, Config::get_round_time() );

	menu->add_item( CL_String("Options"), MENU_OPTIONS );
		menu->add_item( CL_String("Start/Max Extras"), MENU_EXTRA_VALUES, MENU_OPTIONS );
			menu->add_value( CL_String("Start Bombs"), CONFIG_START_BOMBS, MENU_EXTRA_VALUES, 1, 15, Config::get_start_bombs() );
			menu->add_value( CL_String("Start Power"), CONFIG_START_POWER, MENU_EXTRA_VALUES, 1, 15, Config::get_start_power() );
			menu->add_value( CL_String("Start Skateboards"), CONFIG_START_SKATES, MENU_EXTRA_VALUES, 0, 10, Config::get_start_skateboards() );
			menu->add_value( CL_String("Start Kick"), CONFIG_START_KICK, MENU_EXTRA_VALUES, 0, 1, Config::get_start_kick() );
			menu->add_value( CL_String("Start Glove"), CONFIG_START_GLOVE, MENU_EXTRA_VALUES, 0, 1, Config::get_start_glove() );
			menu->add_value( CL_String("Max. Bombs"), CONFIG_MAX_BOMBS, MENU_EXTRA_VALUES, 1, 15, Config::get_max_bombs() );
			menu->add_value( CL_String("Max. Power"), CONFIG_MAX_POWER, MENU_EXTRA_VALUES, 1, 15, Config::get_max_power() );
			menu->add_value( CL_String("Max. Skateboards"), CONFIG_MAX_SKATES, MENU_EXTRA_VALUES, 0, 10, Config::get_max_skateboards() );
		menu->add_item( CL_String("Enable/Disable Extras"), MENU_EXTRA_ONOFF, MENU_OPTIONS );
			menu->add_value( CL_String("Bombs"), CONFIG_BOMBS, MENU_EXTRA_ONOFF, 0, 1, Config::get_bombs() );
			menu->add_value( CL_String("Power"), CONFIG_POWER, MENU_EXTRA_ONOFF, 0, 1, Config::get_power() );
			menu->add_value( CL_String("Skateboard"), CONFIG_SKATES, MENU_EXTRA_ONOFF, 0, 1, Config::get_skateboards() );
			menu->add_value( CL_String("Kick"), CONFIG_KICK, MENU_EXTRA_ONOFF, 0, 1, Config::get_kick() );
			menu->add_value( CL_String("Glove"), CONFIG_GLOVE, MENU_EXTRA_ONOFF, 0, 1, Config::get_glove() );
		menu->add_item( CL_String("Enable/Disable Diseases"), MENU_DISEASE_ONOFF, MENU_OPTIONS );
			menu->add_value( CL_String("Joint"), CONFIG_JOINT, MENU_DISEASE_ONOFF, 0, 1, Config::get_joint() );
			menu->add_value( CL_String("Viagra"), CONFIG_VIAGRA, MENU_DISEASE_ONOFF, 0, 1, Config::get_viagra() );
			menu->add_value( CL_String("Koks"), CONFIG_KOKS, MENU_DISEASE_ONOFF, 0, 1, Config::get_koks() );
		menu->add_item( CL_String("Bomb Timing and Speed"), MENU_TIMING, MENU_OPTIONS );
			menu->add_value( CL_String("Bomb Countdown (1/10 s)"), CONFIG_BOMB_COUNTDOWN, MENU_TIMING, 0, 50, Config::get_bomb_countdown() );
			menu->add_value( CL_String("Bomb Chain Reaction Delay (1/100 s)"), CONFIG_BOMB_DELAY, MENU_TIMING, 0, 50, Config::get_bomb_delay() );
			menu->add_value( CL_String("Moving Bombs Speed (pixels per second)"), CONFIG_BOMB_SPEED, MENU_TIMING, 10, 500, Config::get_bomb_speed() );
		menu->add_item( CL_String("Graphic Options"), MENU_GRAPHICS, MENU_OPTIONS);
			menu->add_value( CL_String("Kidz Mode"), CONFIG_KIDS_MODE, MENU_GRAPHICS, 0, 1, Config::get_kids_mode() );
			menu->add_value( CL_String("Corpse Parts"), CONFIG_CORPSE_PARTS, MENU_GRAPHICS, 0, 100, Config::get_corpse_parts() );
			menu->add_value( CL_String("Shaky Explosions"), CONFIG_SHAKE, MENU_GRAPHICS, 0, 1, Config::get_shaky_explosions() );
		menu->add_value( CL_String("Random Bomber Positions"), CONFIG_RANDOM_POSITIONS_DUPLICATE, MENU_OPTIONS, 0, 1, Config::get_random_positions() );
		menu->add_value( CL_String("Random Map Order"), CONFIG_RANDOM_MAP_ORDER_DUPLICATE, MENU_OPTIONS, 0, 1, Config::get_random_map_order() );
		menu->add_value( CL_String("Points to win"), CONFIG_POINTS_DUPLICATE, MENU_OPTIONS, 1, 10, Config::get_points_to_win() );
		menu->add_value( CL_String("Round Time"), CONFIG_ROUND_TIME_DUPLICATE, MENU_OPTIONS, 5, 300, Config::get_round_time() );

	menu->backup_options_values();
	menu->save_common_options (0, false, true);

	menu->add_item( CL_String("Map Editor"), MENU_MAP_EDITOR );
	menu->add_item( CL_String("Show Credits"), MENU_CREDITS );
	menu->add_item( CL_String("Help Screen"), MENU_HELP );
	menu->add_item( CL_String("Quit Game"), MENU_EXIT );
	menu->scroll_in();

	bool already_started_auto_server = false;
	while (1) {
		int result;
		if (is_client_disconnected_from_server()) {
			menu->go_to_game_menu(true);
			result = SERVERGAME_START;
		}
		else if (is_client_connecting_to_new_server()) {
			result = CLIENTGAME_START;
		}
		else if (ClanBomberApplication::run_server_with_players() && !already_started_auto_server) {
			menu->go_to_game_menu(false);
			result = SERVERGAME_START;
			already_started_auto_server = true;
		}
		else {
			result = menu->execute(false);
		}

		MenuItem* item = menu->get_item_by_id( result );
		switch (result) {
			case MENU_EXIT:
				menu->scroll_out();
				delete menu;
				return 0;
			case MENU_PLAYER_SETUP:
				menu->scroll_out();
 				{
					PlayerSetup ps(this);
					ps.exec();
 				}
				menu->scroll_in();
				break;
			case CONFIG_START_BOMBS:
				Config::set_start_bombs( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_START_POWER:
				Config::set_start_power( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_START_SKATES:
				Config::set_start_skateboards( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_START_KICK:
				Config::set_start_kick( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_START_GLOVE:
				Config::set_start_glove( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_BOMBS:
				Config::set_bombs( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_POWER:
				Config::set_power( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_SKATES:
				Config::set_skateboards( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_KICK:
				Config::set_kick( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_GLOVE:
				Config::set_glove( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_JOINT:
				Config::set_joint( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_VIAGRA:
				Config::set_viagra( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_KOKS:
				Config::set_koks( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_MAX_BOMBS:
				Config::set_max_bombs( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_MAX_POWER:
				Config::set_max_power( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_MAX_SKATES:
				Config::set_max_skateboards( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case MENU_CREDITS:
				menu->scroll_out();
				{
					Credits credits(this);
					credits.exec();
				}
				menu->scroll_in();
				break;
			case MENU_MAP_EDITOR:
				menu->scroll_out();
				{
					MapEditor me(this);
					me.exec();
				}
				menu->scroll_in();
				break;
			case MENU_HELP:
				menu->scroll_out();
				show_tutorial();				
				menu->scroll_in();
				break;
			case CONFIG_MAP_SEL:
				menu->scroll_out();
				{
					MapSelector ms(this);
					ms.exec();
				}
				menu->scroll_in();
				break;
			case CONFIG_POINTS:
				Config::set_points_to_win( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_POINTS_DUPLICATE:
				Config::set_points_to_win( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_ROUND_TIME:
				if (((MenuItem_Value*)item)->get_value()%5 == 1) {
					((MenuItem_Value*)item)->set_value( ((MenuItem_Value*)item)->get_value() + 4 );
				}
				else if (((MenuItem_Value*)item)->get_value()%5 == 4) {
					((MenuItem_Value*)item)->set_value( ((MenuItem_Value*)item)->get_value() - 4 );
				}
				Config::set_round_time( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_ROUND_TIME_DUPLICATE:
				if (((MenuItem_Value*)item)->get_value()%5 == 1) {
					((MenuItem_Value*)item)->set_value( ((MenuItem_Value*)item)->get_value() + 4 );
				}
				else if (((MenuItem_Value*)item)->get_value()%5 == 4) {
					((MenuItem_Value*)item)->set_value( ((MenuItem_Value*)item)->get_value() - 4 );
				}
				Config::set_round_time( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_KIDS_MODE:
				Config::set_kids_mode( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_CORPSE_PARTS:
				Config::set_corpse_parts( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_SHAKE:
		  		Config::set_shaky_explosions( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_BOMB_COUNTDOWN:
		  		Config::set_bomb_countdown( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_BOMB_DELAY:
		  		Config::set_bomb_delay( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_BOMB_SPEED:
				if (((MenuItem_Value*)item)->get_value()%10 == 1) {
					((MenuItem_Value*)item)->set_value( ((MenuItem_Value*)item)->get_value() + 9 );
				}
				else if (((MenuItem_Value*)item)->get_value()%10 == 9) {
					((MenuItem_Value*)item)->set_value( ((MenuItem_Value*)item)->get_value() - 9 );
				}
		  		Config::set_bomb_speed( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_RANDOM_POSITIONS:
		  		Config::set_random_positions( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_RANDOM_POSITIONS_DUPLICATE:
				Config::set_random_positions( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_RANDOM_MAP_ORDER:
		  		Config::set_random_map_order( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case CONFIG_RANDOM_MAP_ORDER_DUPLICATE:
				Config::set_random_map_order( ((MenuItem_Value*)item)->get_value() );
				Config::save();
				break;
			case LOCALGAME_START:
				menu->scroll_out();
				if (Config::get_number_of_opponents() > 1) {
					if (init_game()) {
						run_game();
						deinit_game();
					}
				}
				else {
					menu->scroll_in();
				}
				break;
			case SERVERGAME_START:
				menu->scroll_out();
				Config::save();
				Config::set_filename( "clanbomber_net.cfg" );
				Config::load();
				{
					if (!init_server_game()) {
						break;
					}
				}
				Config::save();
				Config::set_filename( "clanbomber.cfg" );
				Config::load();
				menu->scroll_in();
				menu->set_left_netgame_setup();
				break;
			case CLIENTGAME_START:
				Config::save();
				Config::set_filename( "clanbomber_net.cfg" );
				Config::load();
				menu->scroll_out();
				{
					if (!init_client_game()) {
						break;
					}
				}
				Config::save();
				Config::set_filename( "clanbomber.cfg" );
				Config::load();
				menu->scroll_in();
				menu->set_left_netgame_setup();
				break;
			default:
				cout << result << endl;
				break;
		}
	}	
}

bool ClanBomberApplication::init_server_game()
{
    make_map();
    if (!map->any_valid_map()) {
        delete map;
        return false;
    }

    int previous_client_index = -1;
    
    server_status = 1;
    
    for (int i = 0; i < 8; i++) {   
        Config::bomber[i].disable();
    }

    Config::save();

    cb_server = new Server(this);
    if (cb_server->get_last_error() != NULL) {
        delete cb_server;
        cb_server = NULL;
        return false;
    }

    if (is_client_disconnected_from_server()) {
        previous_client_index = cb_client->get_my_index();
        cb_client->init_server_from_client_info();
        cb_server->send_SERVER_CONNECT_CLIENT(previous_client_index);
        delete cb_client;
        cb_client = NULL;
    }

    cb_client = new Client(this, "localhost");
    if (cb_client->get_last_error() != NULL) {
        delete cb_client;
        cb_client = NULL;
        return false;
    }

    Chat::set_client (cb_client);

    server_setup_menu = new ServerSetup(this, cb_server, cb_client);
    cb_server->start_receiving_from_clients();
    cb_server->start_updating_objects();
    cb_client->start_receiving_from_server();
    cb_client->start_updating_objects();

    if (is_client_disconnected_from_server()) {
        cb_client->send_old_bomber_config_to_new_server(previous_client_index);
        set_client_disconnected_from_server(false);
    }

    while (!server_setup_menu->end_setup()) {
        server_setup_menu->exec();
        Config::save();
        cb_server->stop_accepting_connections();
        if (cb_client->server_started_game()) {
            start_net_game();
        }
        cb_server->reset();
        cb_client->reset();
    }

    if (cb_client != NULL) {
        delete cb_client;   
        cb_client = NULL;
    }

    if (cb_server != NULL) {
        delete cb_server;  
        cb_server = NULL;  
    }

    if (server_setup_menu != NULL) {
        delete server_setup_menu;
        server_setup_menu = NULL;   
    }

    server_status = 0;
    return true;
}

bool ClanBomberApplication::init_client_game()
{
    server_status = 2;
    int previous_client_index = -1;
    CL_String new_string = Config::get_last_server();
    if (is_client_connecting_to_new_server()) {
        previous_client_index = cb_client->get_previous_client_index();
        new_string = CL_String(cb_client->get_new_server_name());
        cb_mutex->lock();
        cb_client->stop_updating_objects();
        cb_mutex->unlock();
        delete cb_client;  
        cb_client = NULL;  
        SimpleTimer ct;    
        ct.reset();
        while (ct.elapsed() < 999) {
            primary->SetColor( primary, 130, 220, 128, 255);
            primary->FillRectangle( primary, 200, 170, 400, 40 );
            primary->SetFont( primary, Resources::Font_big());   
            primary->SetColor (primary, 100, 100, 200, 255);     
            primary->DrawString( primary, "enter server NAME or IP:", -1, 400, 150, DSTF_CENTER);
            primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );
            primary->DrawString( primary, new_string, -1, 400, 200, DSTF_CENTER);
            primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC);
        }
    }
    else {
        int enter_status = 0;
        while (1) {
            primary->SetColor( primary, 130, 220, 128, 255);
            primary->FillRectangle( primary, 200, 170, 400, 40 );
            primary->SetFont( primary, Resources::Font_big() );  
            primary->SetColor (primary, 100, 100, 200, 255);     
            primary->DrawString( primary, "enter server NAME or IP:", -1, 400, 150, DSTF_CENTER);
            primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );
            primary->DrawString( primary, new_string, -1, 400, 200, DSTF_CENTER);
            primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );
            enter_status = enter_string( new_string );
            if (enter_status) {
                break;
            }
        }
        if (enter_status < 0) {
            return false;
        }
    }

    cb_client = new Client(this, new_string.get_string());
    if (cb_client->get_last_error() != NULL) {
        delete cb_client;
        cb_client = NULL;
        return false;
    }

    Chat::set_client (cb_client);

    Config::set_last_server(CL_String(cb_client->get_server_name()));
    client_setup_menu = new ClientSetup(this, cb_client);
    cb_client->start_receiving_from_server();
    cb_client->start_updating_objects();
    cb_client->send_CLIENT_HANDSHAKE(); 

    make_map();

    if (!client_setup_menu->is_disconnected()) {
        cb_client->send_CLIENT_MAP_CHECKSUMS(); 
    }

    if (is_client_connecting_to_new_server()) {
        cb_client->send_old_bomber_config_to_new_server(previous_client_index);
        set_client_connecting_to_new_server(false);
    }
    while (!client_setup_menu->end_setup()) {
        if (client_setup_menu->is_disconnected() || is_client_disconnected_from_server()) {
            break;
        }
        client_setup_menu->exec();
        Config::save();
        if (cb_client->server_started_game()) {
            start_net_game();
        }
        cb_client->reset();
    }
    if (cb_client != NULL) {
        if(!is_client_disconnected_from_server() && !is_client_connecting_to_new_server()) {
            delete cb_client;
            cb_client = NULL;
        }
    }
    if (client_setup_menu != NULL) {
        delete client_setup_menu;   
        client_setup_menu = NULL;   
    }
    server_status = 0;
    return true;
}

int ClanBomberApplication::get_server_frame_counter()
{
	return server_frame_counter;
}

void ClanBomberApplication::inc_server_frame_counter()
{
    server_frame_counter++;
}

void ClanBomberApplication::run_game()
{
	primary->SetColor( primary, 0, 0, 0, 0xFF );
	primary->SetDrawingFlags( primary, DSDRAW_NOFX );
	primary->FillRectangle( primary, 0, 0, 800, 600 );
	primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );
	primary->FillRectangle( primary, 0, 0, 800, 600 );
	primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );
	DFBInputDeviceKeyState escape = DIKS_UP;

	if (!is_server() && is_client()) {
		cb_event->lock();
		if (!map->is_received_by_client) {
			cb_event->wait();
		}
		cb_event->unlock();

		cb_event->lock();
		if (!bombers_received_by_client) {
			cb_event->wait();
		}
		bombers_received_by_client = false;
		observer->make_client_game_status();
		cb_event->unlock();
	}

	Timer::reset();
	keybuffer->Reset( keybuffer );
	while (escape == DIKS_UP) {
		if(is_server()) {
			cb_server->disconnect_dead_clients();
			cb_server->send_SERVER_KEEP_ALIVE();
			cb_server->update_traffic_statistics();
		}
		else if(is_client()) {
			cb_client->disconnect_from_server();
			cb_client->send_CLIENT_KEEP_ALIVE();
			cb_client->update_traffic_statistics();
		}

		Timer::time_elapsed(true);

		DFBInputEvent evt;
		while (keybuffer->GetEvent( keybuffer, DFB_EVENT(&evt) ) == DFB_OK) {
			if (evt.type == DIET_KEYPRESS) {
				switch (DFB_LOWER_CASE(evt.key_symbol)) {
					case DIKS_SMALL_P:
						if (is_server() || !is_client()) {
							pause_game = !pause_game;
							if (is_server()) {
								cb_server->send_SERVER_PAUSE_GAME(pause_game);
							}
						}
						break;
					case DIKS_F1:
						show_fps = !show_fps;
						break;
					case DIKS_SMALL_T:
						if (is_client()) {
							CL_Iterator<Bomber> bomber_object_counter(bomber_objects);
							while (bomber_object_counter.next() != NULL) {
								if (bomber_object_counter() != NULL) {
									if (Config::bomber[bomber_object_counter()->get_number()].is_local() &&
										(bomber_object_counter()->controller->get_type() == Controller::KEYMAP_1 ||
										 bomber_object_counter()->controller->get_type() == Controller::KEYMAP_2 ||
										 bomber_object_counter()->controller->get_type() == Controller::KEYMAP_3)) {
										bomber_object_counter()->controller->deactivate();
									}
								}
							}
							CL_Iterator<Bomber> bomber_object_counter2(bomber_objects);
							while (bomber_object_counter2.next() != NULL) {
								if (bomber_object_counter2() != NULL) {
									if (Config::bomber[bomber_object_counter2()->get_number()].is_local() &&
										(bomber_object_counter2()->controller->get_type() == Controller::KEYMAP_1 ||
										 bomber_object_counter2()->controller->get_type() == Controller::KEYMAP_2 ||
										 bomber_object_counter2()->controller->get_type() == Controller::KEYMAP_3)) {
										bomber_object_counter2()->controller->activate();
									}
								}
							}
						}
						break;
					case DIKS_ESCAPE:
					case DIKS_BACK:
						if (is_server()) {
							if (!pause_game) {
								escape = DIKS_DOWN;
							}
						}
						else {
							escape = DIKS_DOWN;
							if (is_client()) {
								ClientSetup::end_session();
								cb_client->send_CLIENT_DISCONNECT();
							}
						}
						break;
					default: ;
				}
			}
		}

		if (pause_game) {
			if (is_server()) {
				cb_server->send_update_messages_to_clients(server_frame_counter);
			}
			show_all();
			primary->SetColor(primary, 0, 0, 0, 123);
			primary->SetDrawingFlags(primary, DSDRAW_BLEND);  
			primary->FillRectangle(primary, 0, 0, 800, 600);
			primary->SetColor(primary, 255, 255, 255, 255);
			primary->SetFont(primary, Resources::Font_big());
	 		primary->DrawString(primary, "- PAUSED -", -1, 400, 300, DSTF_TOPCENTER);
			primary->SetFont(primary, Resources::Font_small());
			primary->Flip(primary, NULL, DSFLIP_WAITFORSYNC);
			continue;
		}

		if (observer != NULL) {
			observer->act();
			if (observer->end_of_game_requested()) {
				if (is_server()) {
					cb_server->send_update_messages_to_clients(server_frame_counter);
				}
				break;
			}
		}

		delete_some();
		act_all();
		show_all();

		if (is_server()) {
			cb_server->send_update_messages_to_clients(server_frame_counter);
		}
		else if (is_client() && cb_client->end_game()) {
			break;
		}

		primary->Flip(primary, NULL, DSFLIP_WAITFORSYNC);
		frame_count++;

		frame_time += Timer::time_elapsed();
		if (frame_time > 2) {
			fps = (int)(frame_count/frame_time + 0.5f);
			frame_time = 0;
			frame_count = 0;
		}		

		server_frame_counter++;
		if (server_frame_counter > 777777777) {
			server_frame_counter = 0;
		}
	}

	if (is_server()) {
		cb_server->send_SERVER_END_OF_GAME();
		cb_server->send_update_messages_to_clients(server_frame_counter);
	}
}

int ClanBomberApplication::get_server_status()
{
	return server_status;
}

Server* ClanBomberApplication::get_server()
{
	return app->cb_server;
}

Client* ClanBomberApplication::get_client()
{
	return app->cb_client;
}

ServerSetup* ClanBomberApplication::get_server_setup()
{
	return app->server_setup_menu;
}
                                                                                                                                                                                                                                
ClientSetup* ClanBomberApplication::get_client_setup()
{
	return app->client_setup_menu;
}

Chat* ClanBomberApplication::get_chat()
{
	return app->chat;
}
                                                                                                                                                                                                                                
Menu* ClanBomberApplication::get_menu()
{
	return app->menu;
}

int ClanBomberApplication::run_server_with_players()
{
    return run_server_with_players_nr;
}

/**
	is_server() returns true for server.
	is_client() returns true for both server and client.
	conclusions:	!is_client() ==> local game
					(!is_server() && is_client()) ==> client only
					(is_server() || !is_client()) ==> local or server ...
**/

bool ClanBomberApplication::is_server()
{
	return (app->get_server_status() == 1 && app->get_server() != NULL && app->get_client() != NULL);
}

bool ClanBomberApplication::is_client()
{
	return ((app->get_server_status() == 1 || app->get_server_status() == 2) && app->get_client() != NULL);
}

void ClanBomberApplication::make_observer()
{
	if (observer == NULL) {
		observer = new Observer(0, 0, this);
	}
}

void ClanBomberApplication::make_map()
{
	if (map == NULL) {
		map = new Map(this);
	}
}

Observer* ClanBomberApplication::get_observer()
{
	return observer;
}

Map* ClanBomberApplication::get_map()
{
	return map;
}

void ClanBomberApplication::delete_all_game_objects()
{
	objects.clear();
	bomber_objects.clear();
}

GameObject* ClanBomberApplication::get_object_by_id(int object_id)
{
	CL_Iterator<Bomber> bomber_object_counter(bomber_objects);
	while (bomber_object_counter.next() != NULL) {
		if (bomber_object_counter() != NULL) {
			if (bomber_object_counter()->get_object_id() == object_id) {
				return bomber_object_counter();
			}
		}
	}
	CL_Iterator<GameObject> game_object_counter(objects);
	while (game_object_counter.next() != NULL) {
		if (game_object_counter() != NULL) {
			if (game_object_counter()->get_object_id() == object_id) {
				return game_object_counter();
			}
		}
	}
	return NULL;
}

Bomb* ClanBomberApplication::activate_suspended_client_bomb_by_id(int object_id)
{
	CL_Iterator<Bomb> bomb_object_counter(suspended_client_bombs);
	while (bomb_object_counter.next() != NULL) {
		if (bomb_object_counter() != NULL) {
			if (bomb_object_counter()->get_object_id() == object_id) {
				Bomb* b = bomb_object_counter();
				objects.add(b);
				bomb_object_counter.remove();
				return b;
			}
		}
	}

	return NULL;
}

void ClanBomberApplication::act_all()
{
	// Map acts first
	cb_mutex->lock();
	if (map != NULL) {
		map->act();
	}
	// Let them do their stuff
	CL_Iterator<GameObject> object_counter(objects);
	while (object_counter.next() != NULL) {
		if(object_counter() != NULL) {
			if (!is_server() && is_client()) {
				if(object_counter()->get_type() == GameObject::BOMB && !object_counter()->is_flying() && object_counter()->get_maptile()->get_type() == MapTile::TRAP) {
					Bomb* sb = (Bomb*)object_counter();
					if (cb_client->get_server_frame_counter() > sb->get_reactivation_frame_counter()) {
						suspended_client_bombs.add(sb);
						object_counter.remove();
					}
					else {
						sb->set_reactivation_frame_counter(sb->get_reactivation_frame_counter()-1);
					}
					continue;
				}
			}
			object_counter()->act();
		}
	}
	CL_Iterator<Bomber> bomber_object_counter(bomber_objects);
	while (bomber_object_counter.next() != NULL) {
		if (bomber_object_counter() != NULL) {
			if (!is_client()) {
				bomber_object_counter()->act();
			}
			else {
				bomber_object_counter()->act_net();
			}
			if (is_server()) {
				if (bomber_object_counter()->direction_has_changed(true)) {
					cb_server->send_SERVER_UPDATE_BOMBER(bomber_object_counter()->get_x(),
						bomber_object_counter()->get_y(),
						bomber_object_counter()->get_server_send_dir(),
						bomber_object_counter()->get_object_id());
				}
			}
			else if (is_client()) {
				if (bomber_object_counter()->direction_has_changed(false) && Config::bomber[bomber_object_counter()->get_number()].is_local()) {
					cb_client->send_CLIENT_BOMBER_DIR(bomber_object_counter()->get_number(), bomber_object_counter()->get_client_dir());
					bomber_object_counter()->set_local_dir(bomber_object_counter()->get_client_dir());
				}
			}
		}
	}
	cb_mutex->unlock();
}

void ClanBomberApplication::delete_some()
{
	int nr=0;
	int obj_ids[66];
	// Check if we should delete some
	cb_mutex->lock();
	CL_Iterator<GameObject> object_counter(objects);
	while (object_counter.next() != NULL) {
		if (object_counter() != NULL) {
			if (object_counter()->delete_me) {
				obj_ids[nr++] = object_counter()->get_object_id();
				delete object_counter();
				object_counter.remove();
			}
		}
	}
	CL_Iterator<Bomber> bomber_object_counter(bomber_objects);
	while (bomber_object_counter.next() != NULL) {
		if (bomber_object_counter() != NULL) {
			if (bomber_object_counter()->delete_me) {
				if (is_client()) {
					bomber_object_counter()->reset();
					bomber_object_counter()->controller->deactivate();
					bomber_object_counter()->set_dead();
				}
				obj_ids[nr++] = bomber_object_counter()->get_object_id();
			}
		}
	}
	cb_mutex->unlock();
	if (is_server() && nr > 0) {
		ClanBomberApplication::get_server()->send_SERVER_DELETE_OBJECTS(nr, obj_ids);
	}
}

void ClanBomberApplication::show_all()
{
	cb_mutex->lock();
	GameObject* draw_list[objects.get_num_items()+bomber_objects.get_num_items()];
	int n = 0;
	int i;
	primary->SetDrawingFlags( primary, DSDRAW_NOFX );
	primary->SetColor( primary, 0, 0, 0, 0xFF );
	primary->FillRectangle( primary, 0, 0, 800, 40 );	// clear top
	if (show_fps) {
		primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );
		primary->DrawString( primary, CL_String(fps) + " fps", -1, 535, 4, DSTF_TOPCENTER);
	}
	DFBRegion rect = { 60, 0, 59 + MAP_WIDTH*40, 39+MAP_HEIGHT*40 };
	primary->SetClip( primary, &rect );

	CL_Iterator<GameObject> object_counter(objects);
	while (object_counter.next() != NULL) {
		if(object_counter() != NULL) {
			draw_list[n] = object_counter();
			n++;
		}
	}
	CL_Iterator<Bomber> bomber_object_counter(bomber_objects);
	while (bomber_object_counter.next() != NULL) {
		if(bomber_object_counter() != NULL) {
			draw_list[n] = bomber_object_counter();
			n++;
		}
	}
	bool sort = true;
	GameObject* obj;
	while (sort) {
		sort = false;
		for( i=0; i<n-1; i++ ) {
			if (draw_list[i]->get_z() > draw_list[i+1]->get_z()) {
				obj = draw_list[i];
				draw_list[i] = draw_list[i+1];
				draw_list[i+1] = obj;
				sort = true;
			}
		}
	}

	if (map != NULL) {
		map->refresh_holes();
	}
	bool drawn_map = false;
	for( i=0; i<n; i++ ) {
		if (map != NULL && draw_list[i]->get_z() >= Z_GROUND && drawn_map == false) {
			map->show();
			drawn_map = true;
		}
		if (draw_list[i] != NULL) {
			draw_list[i]->show();
		}
	}
	if (map != NULL && !drawn_map) {
		map->show();
	}
	if (observer != NULL) {
		DFBRegion screenrect = { 0, 0, 799, 599 };
		if (observer->get_round_time() > 0) {
			primary->SetClip( primary, &screenrect );		
			observer->show();
		}
		else {
			observer->show();
			primary->SetClip( primary, &screenrect );		
		}
	}
	cb_mutex->unlock();
}

void ClanBomberApplication::init_net_game()
{
	frame_count = 0;
	frame_time = 0; 
	fps = 0;

	client_disconnected_from_server = false;
	client_connecting_to_new_server = false;

	// init map
	cb_mutex->lock();
	if (is_server()) {
		if (Config::get_random_map_order()) {
			map->load_random_valid();
		}
		else {
			if (Config::get_start_map() > map->get_map_count()-1) {
				Config::set_start_map( map->get_map_count() -1 );
			}
			map->load_next_valid( Config::get_start_map() );
		}
		if (Config::get_random_positions()) {
			map->randomize_bomber_positions();
		}
		int j = 0;
		for (int i=0; i< 8 ; i++) {
			if (Config::bomber[i].is_enabled()) {
				CL_Vector pos = map->get_bomber_pos(j++);
					new Bomber((int)(pos.x*40),
								(int)(pos.y*40),
								(Bomber::COLOR)(Config::bomber[i].get_skin()),
								Controller::create((Controller::CONTROLLER_TYPE)Config::bomber[i].get_controller()),
								Config::bomber[i].get_name(),
								Config::bomber[i].get_team(),
								i,
								this);
			}
		}
		// this is for removing teams which only one player is in
		int team_count[] = { 0,0,0,0};
		CL_Iterator<Bomber> bomber_object_counter(cb_server->bomber_objects);
		for (int team = 0;team<4;team++) {
			while (bomber_object_counter.next() != NULL) {
				if (bomber_object_counter() != NULL) {
					if (bomber_object_counter()->get_team()-1 == team) {
						team_count[team]++;
					}
				}
			}	 
		}
		while (bomber_object_counter.next() != NULL) {
			if (bomber_object_counter() != NULL) {
				if (bomber_object_counter()->get_team() != 0) {
					if (team_count[bomber_object_counter()->get_team()-1] == 1) {
						bomber_object_counter()->set_team (0);
					}
				}
				bomber_object_counter()->set_pos(350, 270);
				bomber_object_counter()->controller->deactivate();
			}
		}
		cb_server->send_SERVER_ADD_BOMBER();
		cb_server->send_SERVER_UPDATE_MAP();
		cb_server->send_SERVER_START_NEW_LEVEL();
		cb_server->reset_traffic_statistics();
		CL_Iterator<Bomber> bomber_object_counter_tmp(cb_server->bomber_objects);
		while (bomber_object_counter_tmp.next() != NULL) {
			if (bomber_object_counter_tmp() != NULL) {
				bomber_object_counter_tmp()->fly_to(bomber_object_counter_tmp()->get_orig_x(), bomber_object_counter_tmp()->get_orig_y(), 300);
			}
		}
	}
	if (observer == NULL) {
		observer = new Observer( 0, 0, this );
	}
	Config::set_highlight_maptiles(false);
	for (int j=0; j<8; j++) {
		if (Config::bomber[j].is_enabled() && Config::bomber[j].get_highlight_maptile()) {
			Config::set_highlight_maptiles(true);
			break;
		}
	}
	cb_mutex->unlock();
}

bool ClanBomberApplication::init_game()
{
	frame_count = 0;
	frame_time = 0;
	fps = 0;

	// init map
	if (map == NULL) {
		map = new Map(this);
	}
	
	if (!map->any_valid_map()) {
		//throw CL_Error("No valid maps selected!");
		delete map;
		return false;
	}
	
	if (Config::get_random_map_order()) {
		map->load_random_valid();
	}
	else {
		if (Config::get_start_map() > map->get_map_count()-1) {
			Config::set_start_map( map->get_map_count() -1 );
		}
		map->load_next_valid( Config::get_start_map() );
	}
	// init GameObjects
	if (Config::get_random_positions()) {
		map->randomize_bomber_positions();
	}
	CL_Vector pos;
	int j = 0;
	for (int i=0; i< 8 ; i++) {
		if (Config::bomber[i].is_enabled()) {
			pos = map->get_bomber_pos(j++);
			new Bomber( (int)(pos.x*40), (int)(pos.y*40), (Bomber::COLOR)(Config::bomber[i].get_skin()), Controller::create((Controller::CONTROLLER_TYPE)Config::bomber[i].get_controller()), Config::bomber[i].get_name(), Config::bomber[i].get_team(),i, this );
			bomber_objects.get_last()->set_pos( 350, 270 );
			bomber_objects.get_last()->fly_to( (int)(pos.x*40), (int)(pos.y*40), 300 );
			bomber_objects.get_last()->controller->deactivate();
		}		
	}
	// this is for removing teams which only one player is in
	int team_count[] = { 0,0,0,0};
	CL_Iterator<Bomber> bomber_object_counter(bomber_objects);
	for (int team = 0;team < 4; team++) {
		while (bomber_object_counter.next() != NULL) {
			if (bomber_object_counter() != NULL) {
				if (bomber_object_counter()->get_team()-1 == team) {
					team_count[team]++;
				}
			}
		}		
	}
	while (bomber_object_counter.next() != NULL) {
		if (bomber_object_counter() != NULL) {
			if (bomber_object_counter()->get_team() != 0) {
				if (team_count[bomber_object_counter()->get_team()-1] == 1) {
					bomber_object_counter()->set_team (0);
				}
			}
		}
	}

	//look whether we have to enable maptile_highlighting
	Config::set_highlight_maptiles(false);
	for (j=0; j<8; j++) {
		if (Config::bomber[j].is_enabled() && Config::bomber[j].get_highlight_maptile()) {
			Config::set_highlight_maptiles(true);
			break;
		}
	}

#ifdef WITH_DEBUG	
	new Debug( 0, 0, this );
#endif

	observer = new Observer( 0, 0, this );

	return true;
}

void ClanBomberApplication::deinit_game()
{
	// delete all GameObjects
	cb_mutex->lock();
	CL_Iterator<GameObject> object_counter(objects);
	while (object_counter.next() != NULL) {
		if (object_counter() != NULL) {
			delete object_counter();
		}
	}
	objects.clear();
	CL_Iterator<Bomber> bomber_object_counter(bomber_objects);
	while (bomber_object_counter.next() != NULL) {
		if (bomber_object_counter() != NULL) {
			delete bomber_object_counter();
		}
	}
	bomber_objects.clear();
	if (is_server()) {
		cb_server->bomber_objects.clear();
	}
	if (observer) {
		delete observer;
		observer = NULL;
	}
	cb_mutex->unlock();
}

void ClanBomberApplication::run_intro()
{
	
	DFBInputDeviceKeyState escape = DIKS_UP;
	float alpha = 0;
	CL_String domination("A WORLD  DOMINATION PROJECT");
	
	sleep( 2 );

	Timer::reset();
		
	PLAY_CENTER(Resources::Intro_winlevel());
	while (escape == DIKS_UP) {
		primary->SetColor( primary, 0, 0, 0, 0xFF );
		primary->SetDrawingFlags( primary, DSDRAW_NOFX );
		primary->FillRectangle( primary, 0, 0, 800, 600 );

		keyboard->GetKeyState( keyboard, DIKI_ESCAPE, &escape );

		primary->SetBlittingFlags( primary, DSBLIT_NOFX );
		if (alpha < 255) {
			primary->Blit( primary, Resources::Intro_fl_logo(), NULL, 100, 250);

			primary->SetColor( primary, 0, 0, 0, 255-(unsigned char)ceil(alpha) );
			primary->SetDrawingFlags( primary, DSDRAW_BLEND );
			primary->FillRectangle( primary, 100, 250, 600, 106 );
			primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );
	
			alpha += Timer::time_elapsed(true) * 130.0f;
		}
		else {
			primary->Blit( primary, Resources::Intro_fl_logo(), NULL, 100, 250 );
			primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );
			break;
		}
	}
	
	int stringwidth;
	
	Resources::Font_small()->GetStringWidth( Resources::Font_small(), (char*)domination.get_string(), -1, &stringwidth );
	primary->SetFont( primary, Resources::Font_small());
	primary->SetDrawingFlags( primary, DSDRAW_NOFX );
	
	if (escape == DIKS_UP) {
		usleep( 500000 );
		
		for (int domispell=0;domispell<=domination.get_length();domispell++) {
			primary->SetColor( primary, 0, 0, 0, 0xFF );
			primary->FillRectangle( primary, 0, 0, 800, 600 );
		
			keyboard->GetKeyState( keyboard, DIKI_ESCAPE, &escape );
			if (escape == DIKS_DOWN) {
				break;
			}
			
			if (domination.mid(domispell,1) != " ") {
				PLAY_CENTER(Resources::Intro_typewriter());
			}
			primary->Blit( primary, Resources::Intro_fl_logo(), NULL, 100, 250 );
		
			primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );
			primary->DrawString( primary, (char*)domination.mid(0,domispell).get_string(), -1, 400-stringwidth/2, 360, DSTF_TOPLEFT );
			primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );		
			
			usleep( rand()%100000 + 80000 );
		}

		if (escape == DIKS_UP) {
			usleep( 1500000 );
		}

		// Scroll out
		PLAY_CENTER(Resources::Menu_back());
		Timer::reset();
		float scroll = 100;
		while (escape == DIKS_UP && scroll < 800) {
			primary->SetColor( primary, 0, 0, 0, 0xFF );
			primary->FillRectangle( primary, 0, 0, 800, 600 );

			keyboard->GetKeyState( keyboard, DIKI_ESCAPE, &escape );			
		
			primary->Blit( primary, Resources::Intro_fl_logo(), NULL, (int)scroll, 250 );
			
			primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );
			primary->DrawString( primary, (char*)domination.get_string(), -1, 400, (int)(330+scroll*3/10), DSTF_TOPCENTER );
				
			primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );
		
			scroll += Timer::time_elapsed(true)*1100.0f;
		}
	}
}

void ClanBomberApplication::show_tutorial()
{
	int y = 25;
	primary->SetBlittingFlags( primary, DSBLIT_NOFX );
	primary->Blit( primary, Resources::Titlescreen(), NULL, 0, 0 );
	primary->SetDrawingFlags( primary, DSDRAW_BLEND );
	primary->SetColor( primary, 0, 0, 0, 128 );
	primary->FillRectangle( primary, 0, 0, 800, 600 );
		
	primary->SetFont( primary, Resources::Font_big() );
	primary->SetColor( primary, 0xD0, 0xD0, 0x00, 0xFF );
	primary->DrawString( primary, "ClanBomber Extras", -1, 400, y, DSTF_TOPCENTER );

	primary->SetColor( primary, 0xB0, 0xB0, 0x00, 0xFF );			
	y+=80;	
	Resources::Extras_extras()->put_screen( 15,y-5,0);
	primary->DrawString( primary, "Bomb:", -1, 70, y, DSTF_TOPLEFT );
	primary->DrawString( primary, "You can place an additional bomb", -1, 250, y, DSTF_TOPLEFT );

	y+=50;
	Resources::Extras_extras()->put_screen( 15,y-5,1);
	primary->DrawString( primary, "Power:", -1, 70, y, DSTF_TOPLEFT );
	primary->DrawString( primary, "Explosions grow one field in each direction", -1, 250, y, DSTF_TOPLEFT );

	y+=50;
	Resources::Extras_extras()->put_screen( 15,y-5,2);
	primary->DrawString( primary, "Skateboard:", -1, 70, y, DSTF_TOPLEFT );
	primary->DrawString( primary, "Lets you move faster", -1, 250, y, DSTF_TOPLEFT );

	y+=50;
	Resources::Extras_extras()->put_screen( 15,y-5,3);
	primary->DrawString( primary, "Kick:", -1, 70, y, DSTF_TOPLEFT );
	primary->DrawString( primary, "Kick bombs if you walk against one", -1, 250, y, DSTF_TOPLEFT );
	
	y+=50;
	Resources::Extras_extras()->put_screen( 15,y-5,4);
	primary->DrawString( primary, "Throw:", -1, 70, y, DSTF_TOPLEFT );
	primary->DrawString( primary, "Throw Bombs if you press the button twice", -1, 250, y, DSTF_TOPLEFT );
	primary->DrawString( primary, "without moving", -1, 250, y+40, DSTF_TOPLEFT );	// TODO: fontheight
		
	primary->SetColor( primary, 0xF0, 0xF0, 0xF0, 0xFF );
	primary->DrawString( primary, "Press any key", -1, 400, 520, DSTF_TOPCENTER );
	primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );		
	
	keybuffer->Reset( keybuffer );
	keybuffer->WaitForEvent( keybuffer );
	
	
	y = 25;
	primary->Blit( primary, Resources::Titlescreen(), NULL, 0, 0 );	
	primary->SetDrawingFlags( primary, DSDRAW_BLEND );
	primary->SetColor( primary, 0, 0, 0, 128 );
	primary->FillRectangle( primary, 0, 0, 800, 600 );

	primary->SetColor( primary, 0xFF, 0x50, 0x00, 0xFF );		
	primary->DrawString( primary, "ClanBomber Drugs", -1, 400, y, DSTF_TOPCENTER );

	primary->SetColor( primary, 0xE0, 0x50, 0x00, 0xFF );
	y+=80;	
	Resources::Extras_extras()->put_screen( 15,y-5,5);
	primary->DrawString( primary, "Joint:", -1, 70, y, DSTF_TOPLEFT );
	primary->DrawString( primary, "Controller will be reversed", -1, 250, y, DSTF_TOPLEFT );

	y+=50;
	Resources::Extras_extras()->put_screen( 15,y-5,6);
	primary->DrawString( primary, "Viagra:", -1, 70, y, DSTF_TOPLEFT );
	primary->DrawString( primary, "Autofire, this can be very dangerous!", -1, 250, y, DSTF_TOPLEFT );

	y+=50;
	Resources::Extras_extras()->put_screen( 15,y-5,7);
	primary->DrawString( primary, "Cocaine:", -1, 70, y, DSTF_TOPLEFT );
	primary->DrawString( primary, "Lets you move very fast!! (too fast)", -1, 250, y, DSTF_TOPLEFT );

	primary->DrawString( primary, "Press any key", -1, 400, 520, DSTF_TOPCENTER );
	primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );		

	usleep( 250000 );
	keybuffer->Reset( keybuffer );
	keybuffer->WaitForEvent( keybuffer );
}

CL_String ClanBomberApplication::get_map_path()
{
	return map_path;
}

CL_String ClanBomberApplication::get_local_map_path()
{
	return local_map_path;
}

int enter_string( CL_String &new_string )
{
	DFBInputEvent evt;
	
	while (keybuffer->GetEvent( keybuffer, DFB_EVENT(&evt) )  ==  DFB_OK) {
		if (evt.type != DIET_KEYPRESS) {
			continue;
		}
		
		switch (evt.key_id) {
			case DIKI_ENTER:
				return 1;
			case DIKI_ESCAPE:
				return -1;
			case DIKI_BACKSPACE:
				new_string = new_string.mid( 0, new_string.get_length()-1 );
				break;
			default:
				if (evt.key_symbol < 0x100 && evt.key_symbol > 0) {
					char temp[2] = {0,0};
					temp[0] = evt.key_symbol;
					new_string+= temp;
				}
		}
	}
	
	return 0;
}

int main (int argc, char **argv)
{
    /* parse command line options */
    for (int i = 1; i < argc; i++) {
        if (!strcmp (argv[i], "--help")) {
            printf("\n    usage: clanbomber2 [--runserver=X] [directfb-options]");
            printf("\n           (X: start server and run game when X players are enabled)\n\n");
            return 0;
        }
        else if (strstr (argv[i], "--runserver=") == argv[i]) {
            run_server_with_players_nr = atoi (1 + index (argv[i], '='));
            if (run_server_with_players_nr < 2 || run_server_with_players_nr > 8) {
                printf("\n    --runserver expects a number between 2 and 8\n\n");
                return -666;
            }
        }
    }

    app = new ClanBomberApplication();
    app->main( argc, argv );
    delete app;

    return 0;
}
