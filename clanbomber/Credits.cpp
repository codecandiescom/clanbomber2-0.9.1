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
#include "Config.h"

#include "Credits.h"

#include "Corpse_Part.h"
#include "Timer.h"

#define SCROLL_SPEED 400

Credits::Credits( ClanBomberApplication *_app )
{
    app = _app;

    text.add( new CL_String( "ClanBomber2 Credits" ) );
    text.add( new CL_String( "" ) );
    text.add( new CL_String( "Game Design" ) );
    text.add( new CL_String( "Andreas Hundt" ) );
    text.add( new CL_String( "Denis Oliver Kropp" ) );
    text.add( new CL_String( "" ) );
    text.add( new CL_String( "Coding" ) );
    text.add( new CL_String( "Denis Oliver Kropp" ) );
    text.add( new CL_String( "Andreas Hundt" ) );
    text.add( new CL_String( "mass (network code and oza stuf)" ) );
    text.add( new CL_String( "" ) );
    text.add( new CL_String( "Graphics" ) );
    text.add( new CL_String( "Andreeshchev Eugeni" ) );
    text.add( new CL_String( "Denis Oliver Kropp" ) );
    text.add( new CL_String( "Andreas Hundt" ) );
    text.add( new CL_String( "" ) );
    text.add( new CL_String( "Thanks go out to:" ) );
    text.add( new CL_String( "Magnus Norddahl" ) );
    text.add( new CL_String( "(24h ClanLib support)" ) );
    text.add( new CL_String( "Fredrik Hallenberg" ) );
    text.add( new CL_String( "(Debian package maintainer)" ) );
    text.add( new CL_String( "non" ) );
    text.add( new CL_String( "(for creating horst (the guy to the left))" ) );
    text.add( new CL_String( "clanner and resix" ) );
    text.add( new CL_String( "(for creating maps and playing with us)" ) );
    text.add( new CL_String( "Ivar" ) );
    text.add( new CL_String( "(for creating maps)" ) );
    text.add( new CL_String( "Magnus Reftel" ) );
    text.add( new CL_String( "(disable shaky explosions patch)" ) );
    text.add( new CL_String( "the xtux creators" ) );
    text.add( new CL_String( "(for the original tux and bsd-devil graphics)" ) );
    text.add( new CL_String( "SuSE Linux AG" ) );
    text.add( new CL_String( "(for donating free SuSE Linux Professionial packages)" ) );
    text.add( new CL_String( "" ) );
    text.add( new CL_String( "" ) );
    text.add( new CL_String( "Everyone else supporting this game..." ) );
    text.add( new CL_String( "... and playing it" ) );

    yoffset = 50;

/*
    ss_forward = Resources::Credits_forward()->prepare();
    ss_forward.stop(); // workaround
    ss_forward.set_looping(true);

    ss_rewind = Resources::Credits_rewind()->prepare();
    ss_rewind.stop(); // workaround
    ss_rewind.set_looping(true);
*/
}

Credits::~Credits()
{
    for (int i=0; i<text.get_num_items(); i++) {
        delete text[i];
    }
}

void Credits::exec()
{
    DFBInputDeviceKeyState escape = DIKS_UP;
    DFBInputDeviceKeyState up = DIKS_UP;
    DFBInputDeviceKeyState down = DIKS_UP;

    float t = 0;

    Timer::reset();

    draw();

    while (1) {
        CL_Iterator<GameObject> counter(app->objects);

        keyboard->GetKeyState( keyboard, DIKI_UP, &up );
        keyboard->GetKeyState( keyboard, DIKI_DOWN, &down );
        keyboard->GetKeyState( keyboard, DIKI_ESCAPE, &escape );

        if (escape) {
            while (counter.next() != NULL) {
                delete counter();
                counter.remove();
            }
            return;
        }
        if (up) {
/*
            if (!ss_rewind.is_playing() && !stopped)  // workaround for looping {
                ss_rewind.play();
            }
*/
            speed = -SCROLL_SPEED;
        }
		else if (down) {
/*
            if (!ss_forward.is_playing() && !stopped)  // workaround for looping {
                ss_forward.play();
            }
*/
            speed = SCROLL_SPEED;
        }
		else {
/*
            ss_forward.stop();
            ss_rewind.stop();
*/
            speed = 40;
        }

        yoffset -= speed * Timer::time_elapsed(true);
        if (yoffset > 50) {
            if (speed != 20 && !Config::get_kids_mode()) {
                t += Timer::time_elapsed();
                for (; t>0.04f; t-=0.04f) {
                    Corpse_Part* cp = new Corpse_Part( rand()%800-60, -40, app );
                    cp->fly_to( rand()%800-60, 540 );
                    PLAY_CENTER(Resources::Splash(rand()%2));
                }
            }
            yoffset = 50;
            speed = 20;

            if (!stopped) {
                PLAY_CENTER(Resources::Credits_stop());
                //ss_rewind.stop();
            }
            stopped = true;
        }
		else if (yoffset < -text.get_num_items()*40 + 100) {
            if (speed != 40 && !Config::get_kids_mode()) {
                t += Timer::time_elapsed();
                for (; t>0.04f; t-=0.04f) {
                    Corpse_Part* cp = new Corpse_Part( rand()%800-60, -40, app );
                    cp->fly_to( rand()%800-60, 540 );
                    PLAY_CENTER(Resources::Splash(rand()%2));
                }
            }
            yoffset = -text.get_num_items()*40 + 100;

            if (!stopped) {
                PLAY_CENTER(Resources::Credits_stop());
 //               ss_forward.stop();
            }
            stopped = true;
        }
		else {
            stopped = false;
        }

        // Let them do their stuff
        CL_Iterator<GameObject> object_counter(app->objects);
        while (object_counter.next() != NULL) {
			if (object_counter() != NULL) {
				object_counter()->act();
			}
        }
        // Check if we should delete some
        while (object_counter.next() != NULL) {
			if (object_counter() != NULL) {
				if (object_counter()->delete_me) {
					delete object_counter();
					object_counter.remove();
				}
			}
        }

        draw();
    }
}

void Credits::draw()
{
    primary->SetBlittingFlags( primary, DSBLIT_NOFX );
    primary->Blit( primary, Resources::Credits_horst_evil(), NULL, 0, 0 );

    primary->SetBlittingFlags( primary, DSBLIT_SRC_COLORKEY );
    primary->Blit( primary, Resources::Game_cb_logo_small(), NULL,  10, (int)yoffset+10 );
    primary->Blit( primary, Resources::Game_cb_logo_small(), NULL, 640, (int)yoffset+10 );

    primary->Blit( primary, Resources::Intro_fl_logo(), NULL , 100, (int)yoffset + text.get_num_items()*40 + 130 );

    primary->SetFont( primary, Resources::Font_big() );
    primary->SetColor( primary, 0xFF, 0xFF, 0xFF, 0xFF );

    for (int i=0; i<text.get_num_items(); i++) {
        primary->DrawString( primary, *text[i], -1, 400, (int)yoffset + i*40 + 60, DSTF_CENTER );
    }

    if (yoffset < -10) {
        primary->DrawString( primary, "+", -1, 10, 9, DSTF_TOPLEFT );
        primary->DrawString( primary, "+", -1, 790, 9, DSTF_TOPRIGHT );
    }

    if (yoffset > -text.get_num_items()*40 + 350) {
        primary->DrawString( primary, "-", -1, 10, 560, DSTF_TOPLEFT );
        primary->DrawString( primary, "-", -1, 790, 560, DSTF_TOPRIGHT );
    }

    draw_objects();

    primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC );
}

void Credits::draw_objects()
{
    GameObject** draw_list;
    draw_list = new GameObject*[app->objects.get_num_items()];

    int n = 0;
    int i;

    CL_Iterator<GameObject> object_counter(app->objects);
    while (object_counter.next() != NULL) {
        draw_list[n] = object_counter();
        n++;
    }

    bool sort = true;
    GameObject* obj;
    while(sort) {
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

    for( i=0; i<n; i++ ) {
        draw_list[i]->show();
    }
}
