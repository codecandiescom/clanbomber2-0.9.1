/***************************************************************************
                          GameStatus.cpp  -  description                              
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
// $Id: GameStatus.cpp,v 1.28 2004/01/08 16:57:11 xmassx Exp $

#include <stdio.h>
#include <math.h>

#include <directfb.h>

#include "ClanBomber.h"
#include "GameStatus.h"
#include "ServerSetup.h"
#include "Client.h"
#include "GameObject.h"
#include "Config.h"
#include "Timer.h"
#include "Bomber.h"
#include "Map.h"
#include "Server.h"
#include "Chat.h"

static GameStatus* game_status = NULL;
static SimpleTimer demo_mode_timer;
static float pokal_scroll_in = 800;

GameStatus::GameStatus(ClanBomberApplication* _app)
{
	app = _app;
	game_status = this;
}

void GameStatus::show()
{
	pokal_scroll_in = 800;
	demo_mode_timer.reset();
	if (end_of_game) {
		PLAY_CENTER(Resources::Gamestatus_klatsch());
	}
	bool server_acts = ClanBomberApplication::is_server(); 
	bool client_acts = ClanBomberApplication::is_client();
	bool space_pressed = false;
	bool escape_pressed = false;
	keybuffer->Reset( keybuffer );
	DFBInputDeviceKeyState space = DIKS_UP;
	keybuffer->Reset(keybuffer);
	while (((server_acts || !client_acts) && space == DIKS_UP) ||
           (!server_acts && client_acts && !ClanBomberApplication::get_client()->server_started_new_map())) {
		if (server_acts) {
			ClanBomberApplication::get_server()->disconnect_dead_clients();
			ClanBomberApplication::get_server()->send_SERVER_KEEP_ALIVE();
		}
		else if (client_acts) {
			if (escape_pressed && !space_pressed) {
				ClientSetup::end_session();
				ClanBomberApplication::get_client()->send_CLIENT_DISCONNECT();
				break;
			}
			else {
				ClanBomberApplication::get_client()->disconnect_from_server();
				ClanBomberApplication::get_client()->send_CLIENT_KEEP_ALIVE();
			}
			space_pressed = false;
			escape_pressed = false;
		}
		if (!end_of_game && server_acts && ClanBomberApplication::get_server()->is_in_demo_mode()) {
			if (demo_mode_timer.elapsed()>NET_SERVER_PAUSE_MILLISECONDS_BETWEEN_MAPS) {
				break;
			}
		}
		if (!server_acts && client_acts && ClanBomberApplication::get_client()->end_game()) {
			break;
		}
		DFBInputEvent evt;
		while (keybuffer->GetEvent( keybuffer, DFB_EVENT(&evt) ) == DFB_OK) {
			if (evt.type == DIET_KEYPRESS) {
				switch (DFB_LOWER_CASE(evt.key_symbol)) {
					case DIKS_BACKSPACE:
						if (server_acts) {
							Chat::show();
							ServerSetup::enter_chat_message(true);
							ClanBomberApplication::get_client()->reset_new_chat_message_arrived();
							Chat::hide();
						}
						else if (client_acts) {
							Chat::show();
							ClientSetup::enter_chat_message(true);
							ClanBomberApplication::get_client()->reset_new_chat_message_arrived();
							Chat::hide();
						}
						break;
					case DIKS_SPACE:
						space_pressed = true;
					case DIKS_ESCAPE:
						escape_pressed = true;
					case DIKS_BACK:
						space = DIKS_DOWN;
						break;
					default: ;
				}
			}
		}
		draw();
	}
	primary->SetColor( primary, 0, 0, 0, 0xFF );
	primary->FillRectangle( primary, 0, 0, 800, 600 );
	primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );
	primary->FillRectangle( primary, 0, 0, 800, 600 );
	primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );
}

GameStatus::~GameStatus()
{
}

void GameStatus::analyze_game()
{
	end_of_game = false;
	winner = NULL;
	CL_Iterator<Bomber> bomber_object_counter(app->bomber_objects);
	while (bomber_object_counter.next() != NULL) {
		if (bomber_object_counter()!=NULL) {
			if (!bomber_object_counter()->is_dead()) {
				winner = bomber_object_counter();
			}
			if (bomber_object_counter()->get_points() == Config::get_points_to_win() ) {
				end_of_game = true;
			}
		}
	}
}

bool GameStatus::get_end_of_game()
{
	return end_of_game;
}

void GameStatus::draw()
{
	bool server_acts = ClanBomberApplication::is_server();
	bool client_acts = ClanBomberApplication::is_client();
 	primary->SetBlittingFlags( primary, DSBLIT_NOFX );
	primary->Blit( primary, Resources::Gamestatus_background(), NULL, 0, 0 );
	primary->SetFont( primary, Resources::Font_small() );
 	primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );
	if (server_acts || !client_acts) {
		if (server_acts && Chat::enabled()) {
			if (ClanBomberApplication::get_server()->is_in_demo_mode() && !game_status->end_of_game) {
				float seconds = (NET_SERVER_PAUSE_MILLISECONDS_BETWEEN_MAPS - demo_mode_timer.elapsed()) / 1000;
				char secs[10] = {0,0,0,0,0,0,0,0,0,0};
				sprintf (secs, "%.02f", seconds);
				primary->SetFont( primary, Resources::Font_small() );
				primary->DrawString( primary, CL_String("PRESS SPACE TO CONTINUE  (autostart ") + CL_String(secs) + CL_String(" s)"), -1, 360, 570, DSTF_TOPCENTER );
			}
			else {
				primary->DrawString( primary, "PRESS SPACE TO CONTINUE", -1, 400, 570, DSTF_TOPCENTER );
			}
		}
		else {
			primary->DrawString( primary, "PRESS SPACE TO CONTINUE", -1, 400, 570, DSTF_TOPCENTER );
		}
	}
	primary->SetFont( primary, Resources::Font_big() );
	if (!game_status->winner) {
   		primary->DrawString( primary, "Draw Game", -1, 500, 40, DSTF_TOPCENTER );
	}
	else {
		primary->DrawString( primary, game_status->winner->get_name() + " won", -1, 500, 40, DSTF_TOPCENTER );
   	}
   	int nr = 0;
	CL_Iterator<Bomber> bomber_object_counter(game_status->app->bomber_objects);
	while ((bomber_object_counter.next() != NULL) && (nr < 8)) {
 	 	if (bomber_object_counter() != NULL) {
	 	        nr++;
			if (!client_acts) {
				bomber_object_counter()->act();
			}
			primary->DrawString( primary, bomber_object_counter()->get_name(), -1, 70, 157+nr*43, DSTF_TOPLEFT );
			if (bomber_object_counter() == game_status->winner) {
				if (!game_status->end_of_game) {
					GameObject* obj = (GameObject*)bomber_object_counter();
		           	 	if (obj != NULL) {
						obj->show( 5+(nr%2)*20, 150+nr*43);
					}
   			       	 	for (int i=0; i<bomber_object_counter()->get_points()-1; i++) {
		      		 	 	Resources::Gamestatus_tools()->put_screen( 300+i*43, 150+nr*43, 0 );
		       		 	}
					Resources::Gamestatus_tools()->put_screen( max(257+bomber_object_counter()->get_points()*43, (int)pokal_scroll_in), 150+nr*43, 0 );
   		        	}
       			 	else {   
			    	        float scalefactor;
   				        scalefactor = (float)(sin(Timer::get_time()/100.0f)+2.0f)/2;
				        GameObject* obj = (GameObject*)bomber_object_counter();
			     	        if (obj != NULL) {
				                obj->show( (int)(5+(800-pokal_scroll_in)/3), (int)(150+nr*43),scalefactor);
		    		        }
  		        		for (int i=0; i<bomber_object_counter()->get_points()-1; i++) {
						Resources::Gamestatus_tools()->put_screen((int)(320+i*43-20*scalefactor), (int)(170+nr*43-20*scalefactor),scalefactor,scalefactor, 0 );
		      	       		}
					Resources::Gamestatus_tools()->put_screen( max((int)(277+bomber_object_counter()->get_points()*43-20*scalefactor), (int)pokal_scroll_in), (int)(170+nr*43-20*scalefactor), scalefactor,scalefactor, 0 );
				}
			}
			else {   
				GameObject* obj = (GameObject*)bomber_object_counter();
				if (obj != NULL) {
					obj->show( 5+(nr%2)*20, 150+nr*43);
				}
				for (int i=0; i<bomber_object_counter()->get_points(); i++) {
			               	Resources::Gamestatus_tools()->put_screen( 300+i*43, 150+nr*43, 0 );
				}
			}
		}
	}
	if (game_status->end_of_game) {
	 	primary->SetFont( primary, Resources::Font_big() );
	 	primary->DrawString( primary, "the Match", -1, 500, 80, DSTF_TOPCENTER );
	}
	else {
	        if (server_acts) {
			game_status->app->get_server()->send_update_messages_to_clients(ClanBomberApplication::get_server_frame_counter());
		}
		primary->SetFont( primary, Resources::Font_small() );
		primary->DrawString( primary, "NEXT LEVEL", -1, 785, 10, DSTF_TOPRIGHT );
		if (!server_acts && client_acts) {
			CL_String temp_string = game_status->app->map->current_server_map_name;
			temp_string.to_upper();
			primary->DrawString( primary, temp_string, -1, 785, 125, DSTF_TOPRIGHT );
		}
		else {
			CL_String temp_string = game_status->app->map->get_name();
	 	        temp_string.to_upper();
	 	        primary->DrawString( primary, temp_string, -1, 785, 125, DSTF_TOPRIGHT );
		}
		game_status->app->map->show_preview(790-119,30,0.18f);
	}
	Chat::draw();
        if ((server_acts || client_acts) && !Chat::enabled()) {
		primary->SetFont(primary, Resources::Font_small());
	        primary->SetColor(primary, 255, 255, 255, 255);
		primary->DrawString(primary, "BACKSPACE:", -1, 640, 580, DSTF_TOPLEFT);
		primary->DrawString(primary, "CHAT", -1, 745, 580, DSTF_TOPLEFT);
 	}
	if (server_acts) {
		if (!Chat::enabled() && pokal_scroll_in <= 100) {
			ServerSetup::show_chat_request();
		}
	}
	else if (client_acts) {
		if (!Chat::enabled() && pokal_scroll_in <= 100) {
			ClientSetup::show_chat_request();
		}
	}
	primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );
	if (pokal_scroll_in > 100) {
	 	if (!game_status->end_of_game) {
 		 	pokal_scroll_in -= Timer::time_elapsed(true)*1000.0f;
 		}
 		else {   
			pokal_scroll_in -= Timer::time_elapsed(true)*333.0f;
 		}
	}
}
