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

#include <unistd.h>

#include "ClanBomber.h"
#include "MapEditor.h"

#include "Config.h"
#include "Map.h"

MapEditor::MapEditor( ClanBomberApplication *_app )
{
	app = _app;

	map = new Map(app);
	
	current_map = 0;
	map_at_top = min( current_map-8, map->get_map_count()-16 );
	if (map_at_top < 0) {
		map_at_top = 0;
	}
	
	list_width = 230;
	for (int i=0; i<map->get_map_count(); i++) {
		CL_String s = map->map_list[i]->get_name();
		s.to_upper();
		
		int width;
		Resources::Font_small()->GetStringWidth( Resources::Font_small(), (char*)s.get_string(), -1, &width);
		
		if (width+10 > list_width) {
			list_width = width+40;
		}
	}
	
	cur_x = 0;
	cur_y = 0;
	text_editor_mode = false;
}

MapEditor::~MapEditor()
{
	delete map;
}

void MapEditor::exec()
{
	draw_select_screen();
	
	while (1) {
		DFBInputEvent	evt;
		
		keybuffer->Reset( keybuffer );
		keybuffer->WaitForEvent( keybuffer );
		
		while (keybuffer->GetEvent( keybuffer, DFB_EVENT(&evt) ) == DFB_OK) {
			if (evt.type != DIET_KEYPRESS) {
				continue;
			}
		
			switch (DFB_LOWER_CASE(evt.key_symbol)) {
	            case DIKS_BACK:
				case DIKS_ESCAPE:
					return;
					break;
	            case DIKS_OK:
				case DIKS_ENTER:
					if (map->map_list[current_map]->is_writable()) {
						PLAY_CENTER(Resources::Menu_clear());
						edit_map( current_map );
					}
					break;
				case DIKS_SMALL_N:
					PLAY_CENTER(Resources::Menu_clear());
					if ( new_map() ) {
						edit_map( current_map );
					}
					break;
				case DIKS_SMALL_D:
					if (map->map_list[current_map]->is_writable()) {
						PLAY_CENTER(Resources::Menu_clear());
						current_map = map->delete_entry( current_map );
						map_at_top = min( current_map-8, map->get_map_count()-16 );
					}
					break;
				case DIKS_CURSOR_UP:
					if (current_map > 0) {
						current_map--;
						map_at_top = min( current_map-8, map->get_map_count()-16 );
						if (map_at_top < 0) {
							map_at_top = 0;
						}
						PLAY_CENTER(Resources::Menu_break());
					}
					break;
				case DIKS_CURSOR_DOWN:
					if (current_map < map->get_map_count()-1) {
						current_map++;
						map_at_top = min( current_map-8, map->get_map_count()-16 );
						if (map_at_top < 0) {
							map_at_top = 0;
						}
						PLAY_CENTER(Resources::Menu_break());
					}
					break;
			}
			draw_select_screen();
		}
	}
}

void MapEditor::draw_select_screen(bool flip)
{
	// opaque background
	primary->SetBlittingFlags( primary, DSBLIT_NOFX );	
	primary->Blit( primary, Resources::MapEditor_background(), NULL, 0, 0 );

	// draw the rest with alphablending
	primary->SetDrawingFlags( primary, DSDRAW_BLEND );
		
	// map list background
	primary->SetColor( primary, 75, 75, 75, 128 );
	primary->FillRectangle( primary, 45, 116, list_width, 400 );
	
	// highlight selected map name
	primary->SetColor( primary, 25, 100, 200, 128 );
	primary->FillRectangle( primary, 45, 116+(current_map-map_at_top)*25, list_width, 25 );
	
	// show up to twenty map names
	for (int i=0; i<min(16, map->get_map_count()); i++) {
		// writable ?
		if (!map->map_list[i+map_at_top]->is_writable()) {
			primary->SetColor( primary, 225, 25, 25, 100 );
			primary->FillRectangle( primary, 45, 116+i*25, list_width, 25 );
		}
		
		// show name
		CL_String s = map->map_list[i+map_at_top]->get_name();
		s.to_upper();
		primary->SetFont( primary, Resources::Font_small() );
		primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );
		primary->DrawString( primary, s, -1, 60, 118+i*25, DSTF_TOPLEFT );
	}
	
	primary->SetFont( primary, Resources::Font_big() );
	
	// show scroll indicators
	if (map_at_top > 0) {
		primary->DrawString( primary, "+", -1, 165, 85, DSTF_TOPCENTER );
	}
	if (map_at_top < map->get_map_count()-16) {
		primary->DrawString( primary, "-", -1, 165, 516, DSTF_TOPCENTER );
	}
	
	primary->DrawString( primary, "Select a map to edit and press Enter", -1, 520, 150, DSTF_TOPCENTER );
	primary->DrawString( primary, "Maps marked red are readonly", -1, 520, 190, DSTF_TOPCENTER );
	
	primary->DrawString( primary, "Press N to create a new map", -1, 520, 250, DSTF_TOPCENTER );
	primary->DrawString( primary, "Press D to delete a map", -1, 520, 290, DSTF_TOPCENTER );
	
	if (flip) {
		primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );
	}
}

bool MapEditor::new_map()
{
	keybuffer->Reset( keybuffer );
	
	CL_String new_string;
	
	while (1) {
		draw_select_screen(false);
	
		primary->SetColor( primary,  0, 0, 0, 128 );
		primary->SetDrawingFlags( primary, DSDRAW_BLEND );		
		primary->FillRectangle( primary, 200, 300, 400, 100 );

		primary->SetFont( primary, Resources::Font_big() );
		primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );
		primary->DrawString( primary, "Name:", -1, 230, 330, DSTF_TOPLEFT );
		primary->DrawString( primary, new_string, -1, 380, 330, DSTF_TOPLEFT );
		
		primary->SetFont( primary, Resources::Font_small() );
		primary->DrawString( primary, "PRESS ESC TO ABORT", -1, 400, 380, DSTF_TOPCENTER );

		primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );
		
		switch (enter_string( new_string )) {
			case 1:
				if (new_string.get_length()) {
					current_map = map->new_entry( new_string );
					map_at_top = min( current_map-8, map->get_map_count()-16 );
					return true;
				}
				return false;
			case -1:
				return false;
		}
	}
	return false; // impossible to reach this
}

void MapEditor::edit_map( int number )
{
	MapEntry *entry = map->map_list[number];
	map->load( number );

	draw_editor();
	primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );

	while (1) {
		DFBInputEvent	evt;
		
		keybuffer->Reset( keybuffer );
		keybuffer->WaitForEvent( keybuffer );
		
		while (keybuffer->GetEvent( keybuffer, DFB_EVENT(&evt) ) == DFB_OK) {
			if (evt.type == DIET_KEYRELEASE) {
				switch (evt.key_id) {
					case DIKI_ESCAPE:
						entry->write_back();
						return;
						break;
					case DIKI_F1: // help screen
						show_help();
						break;
					case DIKI_F2: // editor_mode
						text_editor_mode = !text_editor_mode;
						PLAY_CENTER(Resources::Menu_clear());
						break;
					default:
						break;
				}
			}
			else if (evt.type == DIET_KEYPRESS) {
				bool maptile_set = true;

				switch (evt.key_id) {
					case DIKI_BACKSPACE:
						maptile_set = false;
						PLAY_CENTER(Resources::Menu_clear());
						cur_x--;
						clip_cursor();
						entry->set_data(cur_x,cur_y,'-');			
						map->reload();
						break;
					case DIKI_PAGE_UP: // increase number of players
						maptile_set = false;
						PLAY_CENTER(Resources::Menu_clear());
						entry->set_max_players(entry->get_max_players()+1);
						map->reload();
						break;
					case DIKI_PAGE_DOWN: // decrease number of players
						maptile_set = false;
						PLAY_CENTER(Resources::Menu_clear());
						entry->set_max_players(entry->get_max_players()-1);
						map->reload();
						break;
					case DIKI_SPACE: // none
						PLAY_CENTER(Resources::Menu_clear());
						entry->set_data(cur_x,cur_y,'-');
						map->reload();
						break;
					case DIKI_A: // author
						maptile_set = false;
						PLAY_CENTER(Resources::Menu_break());
						entry->set_author( get_new_author() );
						break;
					case DIKI_G: // ground
						PLAY_CENTER(Resources::Menu_clear());
						entry->set_data(cur_x,cur_y,' ');
						map->reload();
						break;
					case DIKI_W: // wall
						PLAY_CENTER(Resources::Menu_clear());
						entry->set_data(cur_x,cur_y,'*');
						map->reload();
						break;
					case DIKI_B: // box
						PLAY_CENTER(Resources::Menu_clear());
						entry->set_data(cur_x,cur_y,'+');
						map->reload();
						break;
					case DIKI_R: // random box
						PLAY_CENTER(Resources::Menu_clear());
						entry->set_data(cur_x,cur_y,'R');
						map->reload();
						break;
					case DIKI_H: // arrow left
						PLAY_CENTER(Resources::Menu_clear());
						entry->set_data(cur_x,cur_y,'<');
						map->reload();
						break;
					case DIKI_K: // arrow right
						PLAY_CENTER(Resources::Menu_clear());
						entry->set_data(cur_x,cur_y,'>');
						map->reload();
						break;
					case DIKI_U: // arrow up
						PLAY_CENTER(Resources::Menu_clear());
						entry->set_data(cur_x,cur_y,'^');
						map->reload();
						break;
					case DIKI_J: // arrow down
						PLAY_CENTER(Resources::Menu_clear());
						entry->set_data(cur_x,cur_y,'v');
						map->reload();
						break;
					case DIKI_I: // ice
						PLAY_CENTER(Resources::Menu_clear());
						entry->set_data(cur_x,cur_y,'S');
						map->reload();
						break;
					case DIKI_O: // bomb trap
						PLAY_CENTER(Resources::Menu_clear());
						entry->set_data(cur_x,cur_y,'o');
						map->reload();
						break;
					case DIKI_1: // place first player
						PLAY_CENTER(Resources::Menu_clear());
						entry->set_bomber_pos(cur_x,cur_y, 0);
						map->reload();
						break;
					case DIKI_2:
						PLAY_CENTER(Resources::Menu_clear());
						entry->set_bomber_pos(cur_x,cur_y, 1);
						map->reload();
						break;
					case DIKI_3:
						PLAY_CENTER(Resources::Menu_clear());
						entry->set_bomber_pos(cur_x,cur_y, 2);
						map->reload();
						break;
					case DIKI_4:
						PLAY_CENTER(Resources::Menu_clear());
						entry->set_bomber_pos(cur_x,cur_y, 3);
						map->reload();
						break;
					case DIKI_5:
						PLAY_CENTER(Resources::Menu_clear());
						entry->set_bomber_pos(cur_x,cur_y, 4);
						map->reload();
						break;
					case DIKI_6:
						PLAY_CENTER(Resources::Menu_clear());
						entry->set_bomber_pos(cur_x,cur_y, 5);
						map->reload();
						break;
					case DIKI_7:
						PLAY_CENTER(Resources::Menu_clear());
						entry->set_bomber_pos(cur_x,cur_y, 6);
						map->reload();
						break;
					case DIKI_8:
						PLAY_CENTER(Resources::Menu_clear());
						entry->set_bomber_pos(cur_x,cur_y, 7);
						map->reload();
						break;
					case DIKI_LEFT:
						maptile_set = false;
						PLAY_CENTER(Resources::Menu_break());
						cur_x--;
						break;
					case DIKI_RIGHT:
						maptile_set = false;
						PLAY_CENTER(Resources::Menu_break());
						cur_x++;
						break;
					case DIKI_UP:
						maptile_set = false;
						PLAY_CENTER(Resources::Menu_break());
						cur_y--;
						break;
					case DIKI_DOWN:
						maptile_set = false;
						PLAY_CENTER(Resources::Menu_break());
						cur_y++;
						break;
					default:
						maptile_set = false;
				}

				if (maptile_set && text_editor_mode) {
					cur_x++;
				}

				clip_cursor();
			}

			draw_editor();

			primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );
		}
	}
}

void MapEditor::draw_editor()
{
	primary->SetBlittingFlags( primary, DSBLIT_NOFX );
	primary->Blit( primary, Resources::MapEditor_background(), NULL, 0, 0 );
	
	// show map
	map->show();
	map->show_random_boxes();
	map->show_start_positions();
		
	// show cursor
	primary->SetColor( primary,  150, 125, 25, 150 );
	primary->SetDrawingFlags( primary, DSDRAW_BLEND );
	primary->FillRectangle( primary, 60+cur_x*40, 40+cur_y*40, 40, 40 );
	
	// show map name
	primary->SetFont( primary, Resources::Font_big() );
	primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );
	primary->DrawString( primary, map->map_list[current_map]->get_name(), -1, 780, 3, DSTF_TOPRIGHT );
	
	// huh, what's this? ;)
	primary->SetFont( primary, Resources::Font_small() );
	primary->DrawString( primary, "PRESS F1 FOR HELP", -1, 10, 3, DSTF_TOPLEFT );

	if (text_editor_mode) {
		primary->DrawString( primary, "TEXT EDITOR MODE", -1, 10, 580, DSTF_TOPLEFT );
	}
	else {
		primary->DrawString( primary, "NORMAL EDITOR MODE", -1, 10, 580, DSTF_TOPLEFT );
	}

	primary->DrawString( primary, CL_String("NUMBER OF PLAYERS   ") + map->map_list[current_map]->get_max_players(), -1, 780, 580, DSTF_TOPRIGHT );
}

CL_String MapEditor::get_new_author()
{
	CL_String author = map->map_list[current_map]->get_author();

	keybuffer->Reset( keybuffer );
	
	primary->SetDrawingFlags( primary, DSDRAW_BLEND );
	while (1) {
		draw_editor();
	
		primary->SetColor( primary, 0, 0, 0, 200 );
		primary->FillRectangle( primary, 150, 300, 400, 100 );
		
		primary->SetFont( primary, Resources::Font_big() );
		primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );
		primary->DrawString( primary, "Author:", -1, 180, 330, DSTF_TOPLEFT );
		primary->DrawString( primary, author, -1, 330, 330, DSTF_TOPLEFT );
		
		primary->SetFont( primary, Resources::Font_small() );
		primary->DrawString( primary, "PRESS ESC TO ABORT", -1, 400, 380, DSTF_TOPCENTER );
		
		primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );
		
		keybuffer->WaitForEvent( keybuffer );
		
		switch (enter_string( author )) {
			case 1:
				if (author.get_length()) {
					PLAY_CENTER(Resources::Menu_clear());
					return author;
				}
			case -1:
				PLAY_CENTER(Resources::Menu_break());
				return map->map_list[current_map]->get_author();
		}
	}

	// cannot reach this!
	return map->map_list[current_map]->get_author();
}

void MapEditor::show_help()
{
	primary->SetBlittingFlags( primary, DSBLIT_NOFX );
	primary->Blit( primary, Resources::MapEditor_background(), NULL, 0, 0 );
	
	Resources::Game_maptiles()->put_screen( 40, 70, Config::get_theme()*4 + 0 );
	primary->DrawString( primary, "PRESS G FOR A GROUND TILE", -1, 110, 78, DSTF_TOPLEFT );
	
	Resources::Game_maptiles()->put_screen( 40, 110, Config::get_theme()*4 + 1 );
	primary->DrawString( primary, "PRESS W FOR A WALL", -1, 110, 118, DSTF_TOPLEFT );
	
	Resources::Game_maptiles()->put_screen( 40, 150, Config::get_theme()*4 + 2 );
	primary->DrawString( primary, "PRESS B FOR A BOX", -1, 110, 158, DSTF_TOPLEFT );
	
	Resources::Game_maptiles()->put_screen( 40, 190, Config::get_theme()*4 + 2 );
	Resources::Game_maptile_addons()->put_screen( 40, 190, 5 );
	primary->DrawString( primary, "PRESS R FOR A RANDOM TILE", -1, 110, 198, DSTF_TOPLEFT );
	
	Resources::Game_maptiles()->put_screen( 40, 230, Config::get_theme()*4 + 0 );
	Resources::Game_maptile_addons()->put_screen( 40, 230, 7 );
	primary->DrawString( primary, "PRESS O FOR A BOMB TRAP", -1, 110, 238, DSTF_TOPLEFT );

	Resources::Game_maptiles()->put_screen( 40, 270, Config::get_theme()*4 + 0 );
	Resources::Game_maptile_addons()->put_screen( 40, 270, 0 );
	primary->DrawString( primary, "PRESS I FOR AN ICE TILE", -1, 110, 278, DSTF_TOPLEFT );
	
	Resources::Game_maptiles()->put_screen( 40, 320, Config::get_theme()*4 + 0 );
	Resources::Game_maptile_addons()->put_screen( 40, 320, 6 );
	primary->DrawString( primary, "PRESS NUMBER KEYS FOR BOMBER POSITIONS", -1, 110, 328, DSTF_TOPLEFT );
	
	// show arrows
	primary->DrawString( primary, "THE FOLLOWING KEYS PRODUCE ARROWS", -1, 40, 398, DSTF_TOPLEFT );

	Resources::Game_maptiles()->put_screen( 150, 460, Config::get_theme()*4 + 0 );
	Resources::Game_maptile_addons()->put_screen( 150, 460, 3 );
	primary->DrawString( primary, "U", -1, 170, 428, DSTF_TOPCENTER );
	
	Resources::Game_maptiles()->put_screen( 110, 500, Config::get_theme()*4 + 0 );
	Resources::Game_maptile_addons()->put_screen( 110, 500, 2 );
	primary->DrawString( primary, "J", -1, 98, 510, DSTF_TOPCENTER );
	
	Resources::Game_maptiles()->put_screen( 150, 500, Config::get_theme()*4 + 0 );
	Resources::Game_maptile_addons()->put_screen( 150, 500, 1 );
	primary->DrawString( primary, "K", -1, 170, 550, DSTF_TOPCENTER );
	
	Resources::Game_maptiles()->put_screen( 190, 500, Config::get_theme()*4 + 0 );
	Resources::Game_maptile_addons()->put_screen( 190, 500, 4 );
	primary->DrawString( primary, "L", -1, 242, 510, DSTF_TOPCENTER );
	
	// misc stuff
	primary->DrawString( primary, "PRESS SPACE BAR FOR A HOLE", -1, 380, 448, DSTF_TOPLEFT );
	primary->DrawString( primary, "PAGE UP AND DOWN SET NR OF PLAYERS", -1, 380, 468, DSTF_TOPLEFT );
	primary->DrawString( primary, "PRESS A FOR CHANGING THE AUTHOR", -1, 380, 488, DSTF_TOPLEFT );
	primary->DrawString( primary, "PRESSING F2 SWITCHES EDITOR MODE", -1, 380, 508, DSTF_TOPLEFT );
	
	primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );

		
	// wait for the "any key"
	keybuffer->Reset( keybuffer );
	keybuffer->WaitForEvent( keybuffer );
	keybuffer->Reset( keybuffer );
	keybuffer->WaitForEvent( keybuffer );
	keybuffer->Reset( keybuffer );
}

void MapEditor::clip_cursor()
{
	if (text_editor_mode) {
		if (cur_x <0) {
			cur_x = MAP_WIDTH-1;
			cur_y--;
		}
		if (cur_x > MAP_WIDTH-1) {
			cur_x=0;
			cur_y++;
		}
	}
	else {
		if (cur_x <0) {
			cur_x = 0;
		}
		if (cur_x > MAP_WIDTH-1) {
			cur_x = MAP_WIDTH-1;
		}
	}
	if (cur_y <0) {
		cur_y = 0;
	}
	if (cur_y > MAP_HEIGHT-1) {
		cur_y = MAP_HEIGHT-1;
	}
}
