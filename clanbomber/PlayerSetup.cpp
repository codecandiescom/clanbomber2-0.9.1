/***************************************************************************
                          PlayerSetup.cpp  -  description                              
                             -------------------                                         
    begin                : Fri May 14 1999                                           
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
#include "PlayerSetup.h"

#include "Config.h"
#include "Timer.h"
#include "Controller.h"

PlayerSetup::PlayerSetup(ClanBomberApplication* _app)
{
	app = _app;
	cur_row = 0;
	cur_col = 0;
}

PlayerSetup::~PlayerSetup()
{
}

void PlayerSetup::exec()
{
	draw();
	primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC  );	

	DFBInputEvent event;

	keybuffer->Reset(keybuffer);
	
	while (1) {
		keybuffer->WaitForEvent( keybuffer );
				
		if (keybuffer->GetEvent( keybuffer, DFB_EVENT(&event) ) == DFB_OK) {
			if (event.type != DIET_KEYPRESS ) {
				continue;
			}
		}	
	
		switch (DFB_LOWER_CASE(event.key_symbol)) {
            case DIKS_BACK:
			case DIKS_ESCAPE:
				return;
				break;
            case DIKS_OK:
			case DIKS_ENTER:
				PLAY_CENTER(Resources::Menu_clear());
				handle_enter();
				Config::save();
				break;
			case DIKS_SPACE:
				if (Config::bomber[cur_row].is_enabled()) {
					Config::bomber[cur_row].disable();
				}
				else {
					Config::bomber[cur_row].enable();
				}
				Config::save();
				PLAY_CENTER(Resources::Menu_clear());
				break;
			case DIKS_SMALL_H:
				Config::bomber[cur_row].set_highlight_maptile(!Config::bomber[cur_row].get_highlight_maptile());
				Config::save();
				PLAY_CENTER(Resources::Menu_clear());
				break;
			case DIKS_CURSOR_DOWN:
				cur_row += (cur_row<7) ? 1 : 0;
				PLAY_CENTER(Resources::Menu_break());
				break;
			case DIKS_CURSOR_UP:
				cur_row -= (cur_row>0) ? 1 : 0;
				PLAY_CENTER(Resources::Menu_break());
				break;
			case DIKS_CURSOR_LEFT:
				cur_col -= (cur_col>0) ? 1 : 0;
				PLAY_CENTER(Resources::Menu_break());
				break;
			case DIKS_CURSOR_RIGHT:
				cur_col += (cur_col<3) ? 1 : 0;
				PLAY_CENTER(Resources::Menu_break());
				break;
		}
	
		draw();
		primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC  );
	}
}

void PlayerSetup::draw(bool fick)
{
	int cg[5] = { 17, 95, 340, 500, 720 };

	primary->SetBlittingFlags( primary, DSBLIT_NOFX );
	primary->Blit( primary, Resources::Playersetup_background(), NULL, 0, 0 );
	
	primary->SetFont( primary, Resources::Font_small() );	
	primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );
	primary->DrawString( primary, "SKIN", -1, 55, 40, DSTF_CENTER);
	primary->DrawString( primary, "NAME", -1, 130, 40, DSTF_CENTER);
	primary->DrawString( primary, "TEAM", -1, 418, 40, DSTF_CENTER);
	primary->DrawString( primary, "CONTROLLER", -1, 612, 40, DSTF_CENTER);
	
	primary->DrawString( primary, "SPACE ENABLES OR DISABLES A PLAYER, H TOGGLES HIGHLIGHTING", -1, 12, 580, DSTF_TOPLEFT);
		
	primary->SetColor( primary, 55, 110, 220, 70 );
	primary->SetDrawingFlags( primary, DSDRAW_BLEND );
	primary->FillRectangle( primary, 0, 70+cur_row*63, 800, 63 );
	
	if (!fick) {
		primary->SetColor( primary, 40, 40, 240, 120 );
		primary->FillRectangle( primary, cg[cur_col], 70+cur_row*63, cg[cur_col+1] - cg[cur_col], 63 );
	}

	for (int i=0; i<8; i++) {
		if (Config::bomber[i].get_highlight_maptile()) {
			primary->SetColor( primary, 0x00, 0x00, 0xFF, 50 );
			primary->FillRectangle( primary, 34, 90+i*63, 40, 40 );
		}
		Resources::Bombers(Config::bomber[i].get_skin())->put_screen( 35, 130+i*63 - Resources::Bombers(Config::bomber[i].get_skin())->get_height(), 0 );

		if (!fick || cur_row != i) {
			primary->SetFont( primary, Resources::Font_big() );
		}
		primary->SetColor( primary, 0x20, 0x90, 0xF0, 0xFF );
		primary->DrawString( primary, Config::bomber[i].get_name(), -1, 130, 90 + i*63, DSTF_TOPLEFT );
		
		Resources::Playersetup_teams()->put_screen( 360, 72 + i*63, Config::bomber[i].get_team() );
		
		primary->SetFont( primary, Resources::Font_small() );
		primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );
		switch ( Config::bomber[i].get_controller() ) {
			case Controller::AI:
				Resources::Playersetup_controls()->put_screen( 550, 72 + i*63,0);
				primary->DrawString( primary, "DOK", -1, 650, 102 + i*63, DSTF_TOPRIGHT );
				break;
			case Controller::AI_mass:
				Resources::Playersetup_controls()->put_screen( 550, 72 + i*63,0);
				primary->DrawString( primary, "MASS", -1, 650, 102 + i*63, DSTF_TOPRIGHT );
				break;
			case Controller::KEYMAP_1:
			case Controller::KEYMAP_2:
			case Controller::KEYMAP_3:
				Resources::Playersetup_controls()->put_screen( 550, 72 + i*63,Config::bomber[i].get_controller()-Controller::KEYMAP_1+1);
				break;
			case Controller::JOYSTICK_1:
			case Controller::JOYSTICK_2:
			case Controller::JOYSTICK_3:
			case Controller::JOYSTICK_4:
			case Controller::JOYSTICK_5:
			case Controller::JOYSTICK_6:
			case Controller::JOYSTICK_7:
				Resources::Playersetup_controls()->put_screen( 550, 72 + i*63,4);
//				Resources::Font_small()->print_right( 650,102 + i*63,CL_String(Config::bomber[i].get_controller()-Controller::JOYSTICK_1+1));
				break;
		}

		if (!Config::bomber[i].is_enabled()) {
			primary->SetColor( primary, 0, 0, 0, 170 );
			primary->SetDrawingFlags( primary, DSDRAW_BLEND );
			primary->FillRectangle( primary, 0, 70+i*63, 800, 63 );
		}
	}
}

void PlayerSetup::handle_enter()
{
	switch (cur_col) {
		case 0:	Config::bomber[cur_row].set_skin( Config::bomber[cur_row].get_skin()+1 ); break;
		case 1: enter_name(); PLAY_CENTER(Resources::Menu_clear()); break;
		case 2:	Config::bomber[cur_row].set_team( Config::bomber[cur_row].get_team()+1 ); break;
		case 3:	Config::bomber[cur_row].set_controller( Config::bomber[cur_row].get_controller()+1 ); break;
	}
}

void PlayerSetup::enter_name()
{
	float alpha = 0;
			
	keybuffer->Reset( keybuffer );
	CL_String new_string = Config::bomber[cur_row].get_name();
	
	Timer::reset();
	
	primary->SetDrawingFlags( primary, DSDRAW_BLEND );
	while (1) {
		alpha += Timer::time_elapsed(true);
		while (alpha > 0.5f) {
			alpha -= 0.5f;
		}
		draw(true);
		primary->SetColor( primary, 50, 220, 128, (alpha>0.25f) ? (unsigned char)((0.5f-alpha)*3.0f*255) : (unsigned char)(alpha*3.0f*255) );
		primary->FillRectangle( primary, 95, 70+cur_row*63, 245, 63 );
		primary->SetFont( primary, Resources::Font_big() );
		primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );
		primary->DrawString( primary, new_string, -1, 130, 90 + cur_row*63, DSTF_TOPLEFT);		

		primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC  );
		
		
		if (enter_string( new_string )) {
			Config::bomber[cur_row].set_name( new_string );
			return;
		}		
	}
}
