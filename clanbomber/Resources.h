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


#ifndef RESOURCES_H
#define RESOURCES_H

#include <directfb.h>
#include <fusionsound.h>


#define NR_BOMBERSKINS 8

/**
  *@author Andreas Hundt, Denis Oliver Kropp
  */

class Resources
{
public:
	Resources();
	~Resources();
	
	class Surface
	{
	public:
		Surface( const char *filename, int sprite_width, int sprite_height );
		Surface( const char *filename, int sprite_width, int sprite_height, unsigned int color_key );
		~Surface();
		
		int get_height();
		
		void put_screen( int x, int y, int frame, __u8 opacity = 255 );
		void put_screen( int x, int y, float scale_x, float scale_y, int frame, __u8 opacity = 255 );
	
        void get_rect( int frame, DFBRectangle *ret_rect );
	
		IDirectFBSurface	*surface;

    private:
		bool				colorkeying;
		int					sprite_width;
		int					sprite_height;
		int					frames_per_row;
	};

	static void init();

	static IDirectFBFont*		Font_big();
	static IDirectFBFont*		Font_small();

	static IDirectFBSurface*		Titlescreen();

	static IDirectFBSurface*		Intro_fl_logo();
	static IFusionSoundBuffer*	Intro_typewriter();
	static IFusionSoundBuffer*	Intro_winlevel();

	static Surface*		Playersetup_teams();
	static Surface*		Playersetup_controls();
	static Surface*		Playersetup_teamlamps();
	static IDirectFBSurface*		Playersetup_background();

	static IDirectFBSurface*		Mapselector_background();
	static IDirectFBSurface*		Mapselector_not_available();
	
	static Surface*		Gamestatus_tools();
	static IFusionSoundBuffer*	Gamestatus_klatsch();
	static IDirectFBSurface* Gamestatus_background();

	static IFusionSoundBuffer*	Credits_forward();
	static IFusionSoundBuffer*	Credits_rewind();
	static IFusionSoundBuffer*	Credits_stop();


	static IDirectFBSurface*		Credits_horst_evil();
	
	static Surface*			Bombers(int nr);

	static Surface*			Extras_extras();
	static IFusionSoundBuffer*	Extras_wow();
	static IFusionSoundBuffer*	Extras_joint();
	static IFusionSoundBuffer*	Extras_horny();
	static IFusionSoundBuffer*	Extras_schnief();

	static IFusionSoundBuffer*	Menu_whoosh();
	static IFusionSoundBuffer*	Menu_break();
	static IFusionSoundBuffer*	Menu_clear();
	static IFusionSoundBuffer*	Menu_back();
	
	static Surface*				Observer_observer();
	static IFusionSoundBuffer*	Observer_hurry_up();
	static IFusionSoundBuffer*	Observer_time_over();
	static IFusionSoundBuffer*	Observer_crunch();

	static IFusionSoundBuffer*	Game_die();
	static IFusionSoundBuffer*	Game_explode();
	static IFusionSoundBuffer*	Game_putbomb();
	static IFusionSoundBuffer*	Game_deepfall();
	static Surface*		Game_maptiles();
	static Surface*		Game_maptile_addons();
	static Surface*		Game_bombs();
	static Surface*		Game_explosion();
	static IDirectFBSurface*		Game_cb_logo_small();
	
	static IDirectFBSurface*		MapEditor_background();

	static IFusionSoundBuffer*	Corpse_explode();
	static Surface*		Corpse_part();
	
	static IFusionSoundBuffer*	Splash(int nr);

protected:	

	static IDirectFBSurface* load_surface( char *filename );
	
	static int sample_read_chunk_header( int fd, char *magic);
	static DFBResult sample_read_file_header( int fd );
	static IFusionSoundBuffer* load_sample( char *filename );

	static IDirectFBFont *fnt_big;
	static IDirectFBFont *fnt_small;

	static IDirectFBSurface *sur_titlescreen;
	
	static IDirectFBSurface *sur_fl_logo;
	static IFusionSoundBuffer *sfx_typewriter;
	static IFusionSoundBuffer *sfx_winlevel;
	
	static Surface *sur_ps_teams;
	static Surface *sur_ps_controls;
	static Surface *sur_ps_teamlamps;
	static IDirectFBSurface *sur_ps_background;
	
	static IDirectFBSurface *sur_ls_background;
	static IDirectFBSurface *sur_ls_not_available;
	
	static Surface *sur_gs_tools;
	static IFusionSoundBuffer *sfx_klatsch;
	static IDirectFBSurface *sur_gs_background;
	
	static IFusionSoundBuffer *sfx_forward;
	static IFusionSoundBuffer *sfx_rewind;
	static IFusionSoundBuffer *sfx_stop;

	static IDirectFBSurface		*sur_horst_evil;
	
	static Surface			*sur_bombers[NR_BOMBERSKINS];
	
	static Surface			*sur_extras;
	static IFusionSoundBuffer *sfx_wow;
	static IFusionSoundBuffer *sfx_joint;
	static IFusionSoundBuffer *sfx_horny;
	static IFusionSoundBuffer *sfx_schnief;
	
	static IFusionSoundBuffer *sfx_whoosh;
	static IFusionSoundBuffer *sfx_break;
	static IFusionSoundBuffer *sfx_clear;
	static IFusionSoundBuffer *sfx_menu_back;
	
	static Surface *sur_observer;
	static IFusionSoundBuffer *sfx_hurry_up;
	static IFusionSoundBuffer *sfx_time_over;
	static IFusionSoundBuffer *sfx_crunch;
	
	static IFusionSoundBuffer *sfx_die;
	static IFusionSoundBuffer *sfx_explode;
	static IFusionSoundBuffer *sfx_putbomb;
	static IFusionSoundBuffer *sfx_deepfall;
	static Surface		 *sur_maptiles;
	static Surface		 *sur_maptile_addons;
	static Surface		 *sur_bombs;
	static Surface		*sur_explosion;
	static IDirectFBSurface *sur_cb_logo_small;

	static IDirectFBSurface *sur_me_background;

	static IFusionSoundBuffer *sfx_corpse_explode;
	static Surface *sur_corpse_part;
	
	static IFusionSoundBuffer *sfx_splash[2];
};

#endif
