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

#include "ClanBomber.h"
#include "MapSelector.h"

#include "Config.h"
#include "Map.h"

MapSelector::MapSelector( ClanBomberApplication *_app )
{
	app = _app;

	map = new Map(app);
	
	if (Config::get_start_map() > map->get_map_count()-1) {
		Config::set_start_map( map->get_map_count() -1 );
	}
	current_map = Config::get_start_map();
	map_at_top = min( current_map-5, map->get_map_count()-11 );
	if (map_at_top < 0) map_at_top = 0;
	
	current_theme = Config::get_theme();
	
	map->load( current_map );

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
}

MapSelector::~MapSelector()
{
	delete map;
}

void MapSelector::exec()
{
	draw();
	
	keybuffer->Reset( keybuffer );
	
	while (1) {
		DFBInputEvent evt;
		
		keybuffer->WaitForEvent( keybuffer );
		while (keybuffer->GetEvent( keybuffer, DFB_EVENT(&evt) ) == DFB_OK) {
			if (evt.type != DIET_KEYPRESS) {
				continue;
			}
			
			switch (DFB_LOWER_CASE(evt.key_symbol)) {
                case DIKS_BACK:
				case DIKS_ESCAPE:
					Config::set_start_map( current_map );
					Config::save();
					map->save_selection();
					return;
					break;
                case DIKS_OK:
				case DIKS_SPACE:
					{
						map->map_list[current_map]->toggle();
						PLAY_CENTER(Resources::Menu_clear());
					}
					break;
				case DIKS_SMALL_A:
					{
						for (int i=0; i<map->get_map_count(); i++) {
							map->map_list[i]->enable();
						}
						PLAY_CENTER(Resources::Menu_clear());
					}
					break;
				case DIKS_SMALL_S:
					{
						for (int i=0; i<map->get_map_count(); i++) {
							map->map_list[i]->disable();
						}
						map->map_list[current_map]->enable();
						PLAY_CENTER(Resources::Menu_clear());
					}
					break;
				case DIKS_CURSOR_UP:
					if (current_map > 0) {
						current_map--;
						map_at_top = min( current_map-5, map->get_map_count()-11 );
						if (map_at_top < 0) {
							map_at_top = 0;
						}
						PLAY_CENTER(Resources::Menu_break());
						map->load( current_map );
					}
					break;
				case DIKS_CURSOR_DOWN:
					if (current_map < map->get_map_count()-1) {
						current_map++;
						map_at_top = min( current_map-5, map->get_map_count()-11 );
						if (map_at_top < 0) {
							map_at_top = 0;
						}
						PLAY_CENTER(Resources::Menu_break());
						map->load( current_map );
					}
					break;
				case DIKS_CURSOR_LEFT:
					if (current_theme > 0) {
						current_theme--;
						PLAY_CENTER(Resources::Menu_clear());
						Config::set_theme( current_theme );
						map->load( current_map );
					}
					break;
				case DIKS_CURSOR_RIGHT:
					if (current_theme < 5) {
						current_theme++;
						PLAY_CENTER(Resources::Menu_clear());
						Config::set_theme( current_theme );
						map->load( current_map );
					}
					break;
			}
		}
		draw();
	}
}

void MapSelector::draw()
{
	primary->SetBlittingFlags( primary, DSBLIT_NOFX );
	primary->Blit( primary, Resources::Mapselector_background(), NULL, 0, 0 );

	// show map name
	primary->SetFont( primary, Resources::Font_big() );	
	primary->SetColor( primary, 0x30, 0x80, 0xD0, 0xFF );
	primary->DrawString( primary, map->get_name(), -1, 510, 50, DSTF_TOPCENTER );
	
	primary->SetFont( primary, Resources::Font_small() );
	primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );	
	primary->DrawString( primary, "SPACE   TOGGLE CURRENT SELECTION", -1, 10, 530, DSTF_TOPLEFT );
	primary->DrawString( primary, "A   SELECT ALL MAPS", -1, 10, 550, DSTF_TOPLEFT );
	primary->DrawString( primary, "S   SELECT CURRENT MAP ONLY", -1, 10, 570, DSTF_TOPLEFT );
	
	// show author name
	CL_String author("BY ");
	author += map->get_author();
	author.to_upper();
	primary->DrawString( primary, author, -1, 510, 90, DSTF_TOPCENTER );
	
	// highlight selected theme
	primary->SetColor( primary, 255, 0, 0, 128 );
	primary->SetDrawingFlags( primary, DSDRAW_BLEND );
	primary->FillRectangle( primary, 30+ current_theme*120, 423, 120, 30 );

	primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );
	// show all themes
	primary->DrawString( primary, "Ice", -1, 60+30, 428, DSTF_TOPCENTER );
	primary->DrawString( primary, "Jungle", -1, 180+30, 428, DSTF_TOPCENTER );
	primary->DrawString( primary, "Jungle Arena", -1, 300+30, 428, DSTF_TOPCENTER );
	primary->DrawString( primary, "Strange Kitchen", -1, 420+30, 428, DSTF_TOPCENTER );
	primary->DrawString( primary, "Green Crypt", -1, 540+30, 428, DSTF_TOPCENTER );
	primary->DrawString( primary, "Melting Metal", -1, 660+30, 428, DSTF_TOPCENTER );

	// show small map preview
	map->show_preview( 350, 125, 0.5f );

	primary->SetDrawingFlags( primary, DSDRAW_BLEND );
	// display max players info
	if (map->get_max_players() < Config::get_number_of_players()) {
		primary->SetBlittingFlags( primary, DSBLIT_SRC_COLORKEY );
		primary->Blit( primary, Resources::Mapselector_not_available(), NULL, 402, 128 );
		primary->SetColor( primary, 255, 0, 0, 128 );
		primary->FillRectangle( primary, 200, 494, 400, 27 );
		primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );
		primary->DrawString( primary, CL_String("THIS MAP IS ONLY FOR UP TO ") + map->get_max_players() + " PLAYERS", -1, 400, 500, DSTF_TOPCENTER );
	}
	else {
		primary->DrawString( primary, CL_String("THIS MAP IS FOR UP TO ") + map->get_max_players() + " PLAYERS", -1, 400, 500, DSTF_TOPCENTER );
	}
	
	// map list background
	primary->SetColor( primary, 76, 76, 76, 128 );
	primary->FillRectangle( primary, 15, 116, list_width, 391-116 );
	
	// highlight selected map name
	primary->SetColor( primary, 25, 80, 200, 128 );
	primary->FillRectangle( primary, 15, 116+(current_map-map_at_top)*25, list_width, 141-116 );
	
	// show up to ten map names
	for (int i=0; i<min(11, map->get_map_count()); i++) {
		// enabled ?
		if (map->map_list[i+map_at_top]->is_enabled()) {
			primary->SetColor( primary, 200, 200, 0, 100 );
			primary->FillRectangle( primary, 15, 116+i*25, list_width, 141-116 );
		}
		
		// show name
		CL_String s = map->map_list[i+map_at_top]->get_name();
		s.to_upper();
		primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );
		primary->DrawString( primary, s, -1, 25, 118+i*25, DSTF_TOPLEFT );
	}
	
	// show scroll indicators
	if (map_at_top > 0) {
		primary->DrawString( primary, "+", -1, 128, 90, DSTF_TOPCENTER );
	}
	if (map_at_top < map->get_map_count()-11) {
		primary->DrawString( primary, "-", -1, 128, 391, DSTF_TOPCENTER );
	}
	
	primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );
}
