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
// $Id: GameStatus_Team.cpp,v 1.25 2004/01/08 16:57:11 xmassx Exp $

#include <stdio.h>

#include "ClanBomber.h"
#include "GameStatus_Team.h"

#include "GameObject.h"
#include "Config.h"
#include "Timer.h"
#include "Bomber.h"
#include "Map.h"
#include "Client.h"
#include "ClientSetup.h"
#include "Server.h"
#include "ServerSetup.h"
#include "Chat.h"

static GameStatus_Team* game_status_team = NULL;
static SimpleTimer demo_mode_timer;

char* GameStatus_Team::team_names[4] = { "BLOOD TEAM", "GOLD TEAM", "WATER TEAM", "LEAF TEAM" };
	
#include <math.h>

GameStatus_Team::team_color GameStatus_Team::team_colors[4] = {	{255, 128, 128},
                                                                {255, 255, 0},
                                                                {128, 128, 255},
                                                                {128, 255, 128} };

GameStatus_Team::GameStatus_Team(ClanBomberApplication* _app) : GameStatus( _app)
{
	game_status_team = this;
}

void GameStatus_Team::show()
{
	demo_mode_timer.reset();
	if (end_of_game) {
		PLAY_CENTER(Resources::Gamestatus_klatsch());
	}
	bool server_acts = ClanBomberApplication::is_server();
  	bool client_acts = ClanBomberApplication::is_client();
  	keybuffer->Reset(keybuffer);
	keybuffer->Reset( keybuffer );
	bool space_pressed = false;
	bool escape_pressed = false;
	DFBInputDeviceKeyState space = DIKS_UP;
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
  	 	 	if (demo_mode_timer.elapsed() > NET_SERVER_PAUSE_MILLISECONDS_BETWEEN_MAPS) {
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
					break;
					case DIKS_SPACE:
						space_pressed = true;
					case DIKS_ESCAPE:
						escape_pressed = true;
					case DIKS_BACK:
						space = DIKS_DOWN;
						break;
					default:
						;
				}
			}
		}
		draw();
	}
	primary->SetColor( primary, 0, 0, 0, 0xFF );
	primary->SetDrawingFlags( primary, DSDRAW_NOFX );
	primary->FillRectangle( primary, 0, 0, 800, 600 );
	primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );			
	primary->FillRectangle( primary, 0, 0, 800, 600 );
	primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );			
}

GameStatus_Team::~GameStatus_Team()
{
}

void GameStatus_Team::analyze_game()
{
	team_count[0] = team_count[1] = team_count[2] = team_count[3] = 0;	
	team_points[0] = team_points[1] = team_points[2] = team_points[3] = 0;
	end_of_game = false;
	winner = NULL;
	CL_Iterator<Bomber> bomber_object_counter(app->bomber_objects);
	while (bomber_object_counter.next() != NULL) {
		if (bomber_object_counter()!=NULL) {
			if ((bomber_object_counter()->get_team() == 0) && !bomber_object_counter()->is_dead()) {
				winner = bomber_object_counter();
				if (bomber_object_counter()->get_points() == Config::get_points_to_win()) {
					end_of_game = true;
				}
			}
			if (bomber_object_counter()->get_team() != 0) {
				if (!bomber_object_counter()->is_dead()) {
					winner = bomber_object_counter();
				}
				team_count[bomber_object_counter()->get_team()-1] += 1;
				team_points[bomber_object_counter()->get_team()-1] += bomber_object_counter()->get_points();
			}
		}
	}
	for (int i=0; i<4; i++) {
		if ((team_count[i] > 0) && (team_points[i] >= team_count[i]*Config::get_points_to_win())) {
			end_of_game = true;		
		}
	}
}

void GameStatus_Team::draw()
{
	bool server_acts=ClanBomberApplication::is_server();
 	bool client_acts=ClanBomberApplication::is_client();
	CL_String temp_string;
	game_status_team->team_count[0] = game_status_team->team_count[1] = game_status_team->team_count[2] = game_status_team->team_count[3] = 0;
	primary->SetBlittingFlags( primary, DSBLIT_NOFX );
	primary->Blit( primary, Resources::Gamestatus_background(), NULL, 0, 0 );
	primary->SetFont( primary, Resources::Font_small() );
	primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );
	if (server_acts || !client_acts) {
		if (server_acts && !Chat::enabled()) {
			if (ClanBomberApplication::get_server()->is_in_demo_mode() && !game_status_team->end_of_game) {
				float seconds = (NET_SERVER_PAUSE_MILLISECONDS_BETWEEN_MAPS - demo_mode_timer.elapsed()) / 1000;
				char secs[10] = {0,0,0,0,0,0,0,0,0,0};
				sprintf (secs, "%.02f", seconds);
				primary->DrawString( primary, CL_String("PRESS SPACE TO CONTINUE  (autostart  ") + CL_String(secs) + CL_String(" s)"), -1, 360, 570, DSTF_TOPCENTER );
			}
			else {
				primary->DrawString( primary, "PRESS SPACE TO CONTINUE", -1, 400, 570, DSTF_TOPCENTER );
			}
		}
		else {
			primary->DrawString( primary, "PRESS SPACE TO CONTINUE", -1, 400, 570, DSTF_TOPCENTER );
		}
	}
	int window_x_offset = 0;
	int window_y_offset = 170;
	primary->SetDrawingFlags( primary, DSDRAW_BLEND );
	for (int team = 0;team<4;team++) {
		CL_Iterator<Bomber> bomber_object_counter(game_status_team->app->bomber_objects);
		while (bomber_object_counter.next() != NULL) {
			if (bomber_object_counter()->get_team()-1 == team) {
				primary->SetColor( primary, team_colors[team].r, team_colors[team].g, team_colors[team].b, 110 );
				primary->FillRectangle( primary, window_x_offset,  window_y_offset+30+game_status_team->team_count[team]*30, 380, 30 );
				((GameObject*)bomber_object_counter())->show( window_x_offset,window_y_offset+25+game_status_team->team_count[team]*30, 0.5f);
				temp_string = bomber_object_counter()->get_name();
				temp_string.to_upper();
				primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );
				primary->DrawString( primary, temp_string, -1, window_x_offset+35,window_y_offset+35+game_status_team->team_count[team]*30, DSTF_TOPLEFT ); 
				game_status_team->team_count[team]++;
			}
		}
		if (game_status_team->team_count[team] > 0) {
			primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );
			DFBRegion clip = {window_x_offset+180,
					  window_y_offset+4,
					  (int)((game_status_team->team_points[team]/(float)game_status_team->team_count[team])*20 + 0.5f)+ window_x_offset+179,
					  window_y_offset+23};
			DFBRegion screenrect = { 0, 0, 799, 599 };
			Resources::Playersetup_teamlamps()->put_screen( window_x_offset,window_y_offset,  team);
			primary->DrawString( primary, team_names[team], -1, window_x_offset+35,window_y_offset+6, DSTF_TOPLEFT );
			primary->SetClip( primary, &clip );
			for (int i=0 ; i<(game_status_team->team_points[team]/game_status_team->team_count[team])+1; i++ ) {
				Resources::Gamestatus_tools()->put_screen( window_x_offset+180+i*20, window_y_offset+4, 0.5f, 0.5f, 0  );
			}
			primary->SetClip( primary, &screenrect );				
			if (game_status_team->end_of_game && game_status_team->winner->get_team()-1 == team) {
				float scalefactor = (float)(sin(Timer::get_time()/100.0f)+2.0f);
				Resources::Gamestatus_tools()->put_screen((int)(window_x_offset+200-20*scalefactor), (int)(window_y_offset+70-20*scalefactor),scalefactor,scalefactor, 0 );
			}
			window_x_offset+=400;
			if (window_x_offset == 800) {
				window_x_offset=0;
				window_y_offset+=200;
			}
		}
	}
   	CL_Iterator<Bomber> bomber_object_counter2(game_status_team->app->bomber_objects);		
	int i = 0;
	while (bomber_object_counter2.next() != NULL) {
		if (bomber_object_counter2()->get_team() == 0) {
			primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );
			primary->DrawString( primary, "TEAMLESS HEROES", -1, window_x_offset+35,window_y_offset+6, DSTF_TOPLEFT );
			primary->SetColor( primary,  140, 140, 140, 110 );
			primary->FillRectangle( primary, window_x_offset,  window_y_offset+30+i*30,380, 30 );
			((GameObject*)bomber_object_counter2())->show( window_x_offset,window_y_offset+25+i*30, 0.5f);
			temp_string = bomber_object_counter2()->get_name();
			temp_string.to_upper();				
			primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );
			primary->DrawString( primary, temp_string, -1, window_x_offset+35,window_y_offset+35+i*30, DSTF_TOPLEFT );
			for (int j=0 ; j<bomber_object_counter2()->get_points() ; j++ ) {
				Resources::Gamestatus_tools()->put_screen( window_x_offset+180+j*20, window_y_offset+34+i*30,0.5f, 0.5f, 0  );
			}
			if (game_status_team->end_of_game && game_status_team->winner == bomber_object_counter2() ) {
				float scalefactor = (float)(sin(Timer::get_time()/100.0f)+2.0f);
				Resources::Gamestatus_tools()->put_screen((int)(window_x_offset+200-20*scalefactor), (int)(window_y_offset+34+i*30-20*scalefactor),scalefactor,scalefactor, 0 );
			}
			i++;
		}
	}
	primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );
	primary->SetFont( primary, Resources::Font_big() );
	if (!game_status_team->winner) {
		primary->DrawString( primary, "Draw Game", -1, 500, 40, DSTF_TOPCENTER );			
	}
	else {
		if (game_status_team->winner->get_team() == 0) {
			primary->DrawString( primary, game_status_team->winner->get_name() + " won", -1, 500, 40, DSTF_TOPCENTER );			
		}
		else {
			primary->DrawString( primary, CL_String( team_names[game_status_team->winner->get_team()-1] ) + " won", -1, 500, 40, DSTF_TOPCENTER );
		}				
	}
	if (game_status_team->end_of_game) {
		primary->DrawString( primary, "the Match" , -1, 500, 80, DSTF_TOPCENTER );					
	}
	else {
		if (server_acts) {
 	 	 	game_status_team->app->get_server()->send_update_messages_to_clients(ClanBomberApplication::get_server_frame_counter());
   	 	}
		primary->SetFont( primary, Resources::Font_small() );
		primary->DrawString( primary, "NEXT LEVEL", -1, 785, 10, DSTF_TOPRIGHT );
		if (!server_acts && client_acts) { 
			CL_String temp_string = game_status_team->app->map->current_server_map_name;
			temp_string.to_upper();
			primary->DrawString( primary, temp_string, -1, 785, 125, DSTF_TOPRIGHT );
		}
		else {
			CL_String temp_string = game_status_team->app->map->get_name();
			temp_string.to_upper();
			primary->DrawString( primary, temp_string, -1, 785, 125, DSTF_TOPRIGHT );
		}
		game_status_team->app->map->show_preview(790-119,30,0.18f);
	}
	Chat::draw();
	if ((server_acts || client_acts) && !Chat::enabled()) {
		primary->SetFont(primary, Resources::Font_small());  
 	 	primary->SetColor(primary, 255, 255, 255, 255);
		primary->DrawString(primary, "BACKSPACE:", -1, 640, 580, DSTF_TOPLEFT);
		primary->DrawString(primary, "CHAT", -1, 745, 580, DSTF_TOPLEFT);
	}
	if (server_acts) {
		ServerSetup::show_chat_request();
 	}
 	else if (client_acts) {
		ClientSetup::show_chat_request();
 	}
	primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );
}
