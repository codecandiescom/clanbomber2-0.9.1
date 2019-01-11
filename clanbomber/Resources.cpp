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

#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include <config.h>

#include "Resources.h"
#include "ClanBomber.h"


#ifdef WORDS_BIGENDIAN
#define SWAP16(a) (((a)>>8) | ((a) << 8))
#define SWAP32(a) (((a) >> 24) | (((a) & 0x00ff0000) >>  8) | (((a) & 0x0000ff00) <<  8) | (((a) << 24)))
#endif

IDirectFBFont*		Resources::fnt_big = NULL;
IDirectFBFont*		Resources::fnt_small = NULL;

IDirectFBSurface*		Resources::sur_titlescreen = NULL;

IDirectFBSurface*		Resources::sur_fl_logo = NULL;
IFusionSoundBuffer*	Resources::sfx_typewriter = NULL;
IFusionSoundBuffer*	Resources::sfx_winlevel = NULL;

Resources::Surface*		Resources::sur_ps_teams = NULL;
Resources::Surface*		Resources::sur_ps_controls = NULL;
Resources::Surface*		Resources::sur_ps_teamlamps = NULL;
IDirectFBSurface*	Resources::sur_ps_background = NULL;

IDirectFBSurface*		Resources::sur_ls_background = NULL;
IDirectFBSurface*		Resources::sur_ls_not_available = NULL;

Resources::Surface*		Resources::sur_gs_tools = NULL;
IFusionSoundBuffer*	Resources::sfx_klatsch = NULL;
IDirectFBSurface*		Resources::sur_gs_background = NULL;

IFusionSoundBuffer*	Resources::sfx_forward = NULL;
IFusionSoundBuffer*	Resources::sfx_rewind = NULL;
IFusionSoundBuffer*	Resources::sfx_stop = NULL;

IDirectFBSurface*	Resources::sur_horst_evil = NULL;

Resources::Surface*	Resources::sur_bombers[NR_BOMBERSKINS] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };

Resources::Surface*	Resources::sur_extras = NULL;
IFusionSoundBuffer*	Resources::sfx_wow = NULL;
IFusionSoundBuffer*	Resources::sfx_joint = NULL;
IFusionSoundBuffer*	Resources::sfx_horny = NULL;
IFusionSoundBuffer*	Resources::sfx_schnief = NULL;

IFusionSoundBuffer*	Resources::sfx_whoosh = NULL;
IFusionSoundBuffer*	Resources::sfx_break = NULL;
IFusionSoundBuffer*	Resources::sfx_clear = NULL;
IFusionSoundBuffer*	Resources::sfx_menu_back = NULL;

Resources::Surface*	Resources::sur_observer = NULL;
IFusionSoundBuffer*	Resources::sfx_hurry_up = NULL;
IFusionSoundBuffer*	Resources::sfx_time_over = NULL;
IFusionSoundBuffer*	Resources::sfx_crunch = NULL;

IFusionSoundBuffer*	Resources::sfx_die = NULL;
IFusionSoundBuffer*	Resources::sfx_explode = NULL;
IFusionSoundBuffer*	Resources::sfx_putbomb = NULL;
IFusionSoundBuffer*	Resources::sfx_deepfall = NULL;
Resources::Surface*	Resources::sur_maptiles = NULL;
Resources::Surface*	Resources::sur_maptile_addons = NULL;
Resources::Surface*	Resources::sur_bombs = NULL;
Resources::Surface*	Resources::sur_explosion = NULL;
IDirectFBSurface*	Resources::sur_cb_logo_small = NULL;

IDirectFBSurface*	Resources::sur_me_background = NULL;

IFusionSoundBuffer*	Resources::sfx_corpse_explode = NULL;
Resources::Surface*	Resources::sur_corpse_part = NULL;

IFusionSoundBuffer*	Resources::sfx_splash[2] = { NULL, NULL};

typedef struct fmtChunk {
     __u16     encoding;
     __u16     channels;                /* 1 = mono, 2 = stereo */
     __u32     frequency;               /* One of 11025, 22050, or 44100 Hz */
     __u32     byterate;                /* Average bytes per second */
     __u16     blockalign;              /* Bytes per sample block */
     __u16     bitspersample;           /* One of 8, 12, 16, or 4 for ADPCM */
} fmtChunk;

Resources::Resources()
{
}

Resources::~Resources()
{
}

void Resources::init()
{
}

IDirectFBFont* Resources::Font_big()
{
	if (!fnt_big) {
		DFBFontDescription desc;
		
		desc.flags = DFDESC_HEIGHT;
		desc.height = 28;
		
		DFBCHECK(dfb->CreateFont( dfb, DATADIR"/decker.ttf", &desc, &fnt_big ));
	}
	return fnt_big;
}

IDirectFBFont* Resources::Font_small()
{
	if (!fnt_small) {
		DFBFontDescription desc;
		
		desc.flags = DFDESC_HEIGHT;
		desc.height = 18;
		DFBCHECK(dfb->CreateFont( dfb, DATADIR"/decker.ttf", &desc, &fnt_small ));

	}
	return fnt_small;
}

IDirectFBSurface* Resources::Titlescreen()
{
	if (!sur_titlescreen) {
//		if (rand()%2) {
			sur_titlescreen = load_surface(DATADIR"/pics/clanbomber_title_andi.png");
//		}
//		else {
//			sur_titlescreen = load_surface(DATADIR"/pics/clanbomber_title_dok.png");
//		}
	}
	return sur_titlescreen;
}

IDirectFBSurface* Resources::Intro_fl_logo()
{
	if (!sur_fl_logo) {
		sur_fl_logo = load_surface(DATADIR"/pics/fischlustig_logo.png");
	}
	return sur_fl_logo;
}

IFusionSoundBuffer* Resources::Intro_typewriter()
{
	if (!sfx_typewriter) {
		sfx_typewriter = load_sample(DATADIR"/wavs/typewriter.wav");
	}
	return sfx_typewriter;
}

IFusionSoundBuffer* Resources::Intro_winlevel()
{
	if (!sfx_winlevel) {
		sfx_winlevel = load_sample(DATADIR"/wavs/winlevel.wav");
	}
	return sfx_winlevel;
}

Resources::Surface* Resources::Playersetup_teams()
{
	if (!sur_ps_teams) {
		sur_ps_teams = new Surface( DATADIR"/pics/ps_teams.png", 125, 56, 0 );
	}
	return sur_ps_teams;
}

Resources::Surface* Resources::Playersetup_controls()
{
	if (!sur_ps_controls) {
		sur_ps_controls = new Surface( DATADIR"/pics/ps_controls.png", 125, 56, 0 );
	}
	return sur_ps_controls;
}

Resources::Surface* Resources::Playersetup_teamlamps()
{
	if (!sur_ps_teamlamps) {
		sur_ps_teamlamps = new Surface( DATADIR"/pics/ps_teamlamps.png", 30, 32, 0 );
	}
	return sur_ps_teamlamps;
}

IDirectFBSurface* Resources::Playersetup_background()
{
	if (!sur_ps_background) {
		sur_ps_background = load_surface(DATADIR"/pics/playersetup.png");
	}
	return sur_ps_background;
}

IDirectFBSurface* Resources::Mapselector_background()
{
	if (!sur_ls_background) {
		sur_ls_background = load_surface(DATADIR"/pics/level_selection.png");
	}
	return sur_ls_background;
}

IDirectFBSurface* Resources::Mapselector_not_available()
{
	if (!sur_ls_not_available) {
		sur_ls_not_available = load_surface(DATADIR"/pics/not_available.png");
	}
	return sur_ls_not_available;
}

Resources::Surface* Resources::Gamestatus_tools()
{
	if (!sur_gs_tools) {
		sur_gs_tools = new Surface( DATADIR"/pics/cup2.png", 40, 40, 0 );
	}
	return sur_gs_tools;
}

IFusionSoundBuffer* Resources::Gamestatus_klatsch()
{
	if (!sfx_klatsch) {
		sfx_klatsch = load_sample( DATADIR"/wavs/klatsch.wav");
	}
	return sfx_klatsch;
}

IDirectFBSurface* Resources::Gamestatus_background()
{
	if (!sur_gs_background) {
		sur_gs_background = load_surface( DATADIR"/pics/game_status.png" );
	}
	return sur_gs_background;
}

IFusionSoundBuffer* Resources::Credits_forward()
{
	if (!sfx_forward) {
		sfx_forward = load_sample( DATADIR"/wavs/forward.wav");
	}
	return sfx_forward;
}

IFusionSoundBuffer* Resources::Credits_rewind()
{
	if (!sfx_rewind) {
		sfx_rewind = load_sample( DATADIR"/wavs/rewind.wav");
	}
	return sfx_rewind;
}

IFusionSoundBuffer* Resources::Credits_stop()
{
	if (!sfx_stop) {
		sfx_stop = load_sample( DATADIR"/wavs/stop.wav");
	}
	return sfx_stop;
}


IDirectFBSurface* Resources::Credits_horst_evil()
{
	if (!sur_horst_evil) {
		sur_horst_evil = load_surface( DATADIR"/pics/horst_evil.png" );
	}
	return sur_horst_evil;
}

Resources::Surface* Resources::Bombers(int nr)
{
	if (nr >= NR_BOMBERSKINS || nr < 0) {
		return NULL;
	}
	if (!sur_bombers[nr]) {
		sur_bombers[0] = new Surface( DATADIR"/pics/bomber_snake.png", 40, 60, 0x0000FF00 );
		sur_bombers[1] = new Surface( DATADIR"/pics/bomber_tux.png", 40, 60, 0x0000FF00 );
		sur_bombers[2] = new Surface( DATADIR"/pics/bomber_spider.png", 40, 40, 0x0000FF00 );
		sur_bombers[3] = new Surface( DATADIR"/pics/bomber_bsd.png", 40, 60, 0x0000FF00 );
		sur_bombers[4] = new Surface( DATADIR"/pics/bomber_dull_red.png", 40, 40, 0x0000FF00 );
		sur_bombers[5] = new Surface( DATADIR"/pics/bomber_dull_blue.png", 40, 40, 0x0000FF00 );
		sur_bombers[6] = new Surface( DATADIR"/pics/bomber_dull_yellow.png", 40, 40, 0x0000FF00 );
		sur_bombers[7] = new Surface( DATADIR"/pics/bomber_dull_green.png", 40, 40, 0x00FF00FF );
	//	sur_bombers[8] = new Surface( DATADIR"/pics/bomber_black.png", 40, 40, 0x00FF00FF );
	}
	return sur_bombers[nr];
}

Resources::Surface* Resources::Extras_extras()
{
	if (!sur_extras) {
		sur_extras = new Surface( DATADIR"/pics/extras2.png", 40, 40 );
	}
	return sur_extras;
}

IFusionSoundBuffer* Resources::Extras_wow()
{
	if (!sfx_wow) {
		sfx_wow = load_sample( DATADIR"/wavs/wow.wav");
	}
	return sfx_wow;
}

IFusionSoundBuffer* Resources::Extras_joint()
{
	if (!sfx_joint) {
		sfx_joint = load_sample(DATADIR"/wavs/joint.wav");
	}
	return sfx_joint;
}

IFusionSoundBuffer* Resources::Extras_horny()
{
	if (!sfx_horny) {
		sfx_horny = load_sample(DATADIR"/wavs/horny.wav");
	}
	return sfx_horny;
}

IFusionSoundBuffer* Resources::Extras_schnief()
{
	if (!sfx_schnief) {
		sfx_schnief = load_sample(DATADIR"/wavs/schnief.wav");
	}
	return sfx_schnief;
}

IFusionSoundBuffer* Resources::Menu_whoosh()
{
	if (!sfx_whoosh) {
		sfx_whoosh = load_sample(DATADIR"/wavs/whoosh.wav");
	}
	return sfx_whoosh;
}

IFusionSoundBuffer* Resources::Menu_break()
{
	if (!sfx_break) {
		sfx_break = load_sample(DATADIR"/wavs/break.wav");
	}
	return sfx_break;
}

IFusionSoundBuffer* Resources::Menu_clear()
{
	if (!sfx_clear) {
		sfx_clear = load_sample(DATADIR"/wavs/clear.wav");
	}
	return sfx_clear;
}
	
IFusionSoundBuffer* Resources::Menu_back()
{
	if (!sfx_menu_back) {
		sfx_menu_back = load_sample(DATADIR"/wavs/menu_back.wav");
	}
	return sfx_menu_back;
}
	
Resources::Surface* Resources::Observer_observer()
{
	if (!sur_observer) {
   		sur_observer = new Surface( DATADIR"/pics/observer.png", 40, 40, 0 );
	}
	return sur_observer;
}

IFusionSoundBuffer* Resources::Observer_hurry_up()
{
	if (!sfx_hurry_up) {
		sfx_hurry_up = load_sample(DATADIR"/wavs/hurry_up.wav");
	}
	return sfx_hurry_up;
}

IFusionSoundBuffer* Resources::Observer_time_over()
{
	if (!sfx_time_over) {
		sfx_time_over = load_sample(DATADIR"/wavs/time_over.wav");
	}
	return sfx_time_over;
}

IFusionSoundBuffer* Resources::Observer_crunch()
{
	if (!sfx_crunch) {
		sfx_crunch = load_sample(DATADIR"/wavs/crunch.wav");
	}
	return sfx_crunch;
}

IFusionSoundBuffer* Resources::Game_die()
{
	if (!sfx_die) {
		sfx_die = load_sample(DATADIR"/wavs/die.wav");
	}
	return sfx_die;
}

IFusionSoundBuffer* Resources::Game_explode()
{
	if (!sfx_explode) {
		sfx_explode = load_sample(DATADIR"/wavs/explode.wav");
	}
	return sfx_explode;
}

IFusionSoundBuffer* Resources::Game_putbomb()
{
	if (!sfx_putbomb) {
		sfx_putbomb = load_sample(DATADIR"/wavs/putbomb.wav");
	}
	return sfx_putbomb;
}

IFusionSoundBuffer* Resources::Game_deepfall()
{
	if (!sfx_deepfall) {
		sfx_deepfall = load_sample(DATADIR"/wavs/deepfall.wav");
	}
	return sfx_deepfall;
}

Resources::Surface* Resources::Game_maptiles()
{
	if (!sur_maptiles) {
		sur_maptiles = new Surface( DATADIR"/pics/maptiles.png", 40, 40 );
	}
	return sur_maptiles;
}

Resources::Surface* Resources::Game_maptile_addons()
{
	if (!sur_maptile_addons) {
		sur_maptile_addons = new Surface( DATADIR"/pics/maptile_addons.png", 40, 40, 0 );
	}
	return sur_maptile_addons;
}

Resources::Surface* Resources::Game_bombs()
{
	if (!sur_bombs) {
		sur_bombs = new Surface( DATADIR"/pics/bombs.png", 40, 40, 0x00FF00FF );
	}
	return sur_bombs;
}

Resources::Surface* Resources::Game_explosion()
{
	if (!sur_explosion) {
		sur_explosion = new Surface( DATADIR"/pics/explosion2.png", 40, 40, 0 );
	}
	return sur_explosion;
}

IDirectFBSurface* Resources::Game_cb_logo_small()
{
	if (!sur_cb_logo_small) {
		sur_cb_logo_small = load_surface(DATADIR"/pics/cb_logo_small.png");
	}
	return sur_cb_logo_small;
}
	
IDirectFBSurface* Resources::MapEditor_background()
{
	if (!sur_me_background) {
		sur_me_background = load_surface(DATADIR"/pics/map_editor.png");
	}
	return sur_me_background;
}

IFusionSoundBuffer* Resources::Corpse_explode()
{
	if (!sfx_corpse_explode) {
		sfx_corpse_explode = load_sample(DATADIR"/wavs/corpse_explode.wav");
	}
	return sfx_corpse_explode;
}

Resources::Surface* Resources::Corpse_part()
{
	if (!sur_corpse_part) {
		sur_corpse_part = new Surface( DATADIR"/pics/corpse_parts.png", 40, 40, 0 );
	}
	return sur_corpse_part;
}

IFusionSoundBuffer* Resources::Splash(int nr)
{
	if (nr > 1 || nr < 0) {
		return NULL;
	}
	if (!sfx_splash[nr]) {
		sfx_splash[0] = load_sample(DATADIR"/wavs/splash1a.wav");
		sfx_splash[1] = load_sample(DATADIR"/wavs/splash2a.wav");
	}
	return sfx_splash[nr];
}

Resources::Surface::Surface( const char *filename, int sprite_width, int sprite_height )
{
	IDirectFBImageProvider *provider;
	DFBSurfaceDescription dsc;
	DFBSurfacePixelFormat pixelformat;
					
	DFBCHECK(dfb->CreateImageProvider( dfb, filename, &provider ));
	
	DFBCHECK(provider->GetSurfaceDescription( provider, &dsc ));
	
	primary->GetPixelFormat( primary, &pixelformat );

	dsc.pixelformat = pixelformat;
	
	DFBCHECK(dfb->CreateSurface( dfb, &dsc, &surface ));
	
	DFBCHECK(provider->RenderTo( provider, surface, NULL ));
	provider->Release( provider );
	
	int surface_width;
	surface->GetSize( surface, &surface_width, NULL );
	frames_per_row = surface_width/sprite_width;
	
	this->sprite_width = sprite_width;
	this->sprite_height = sprite_height;

	colorkeying = false;
}

Resources::Surface::Surface( const char *filename, int sprite_width, int sprite_height, unsigned int color_key )
{
	IDirectFBImageProvider *provider;
	DFBSurfaceDescription dsc;
	DFBSurfacePixelFormat pixelformat;

	DFBCHECK(dfb->CreateImageProvider( dfb, filename, &provider ));
	
	DFBCHECK(provider->GetSurfaceDescription( provider, &dsc ));
	
	primary->GetPixelFormat( primary, &pixelformat );

	dsc.pixelformat = pixelformat;

	dfb->CreateSurface( dfb, &dsc, &surface );
	
	DFBCHECK(provider->RenderTo( provider, surface, NULL ));
	provider->Release( provider );
	
	int surface_width;
	surface->GetSize( surface, &surface_width, NULL );
	frames_per_row = surface_width/sprite_width;
	
	this->sprite_width = sprite_width;
	this->sprite_height = sprite_height;

	surface->SetSrcColorKey( surface, (color_key & 0x00FF0000) >> 16,
	                                  (color_key & 0x0000FF00) >> 8,
	                                  (color_key & 0x000000FF));
	                                  
	colorkeying = true;
}

Resources::Surface::~Surface()
{
	surface->Release( surface );
	free( surface );
}

int Resources::Surface::get_height()
{
	return sprite_height;
}

void Resources::Surface::put_screen( int x, int y, int frame, __u8 opacity )
{
	DFBRectangle rect;
	
	rect.x = (frame % frames_per_row) * sprite_width;
	rect.y = (frame / frames_per_row) * sprite_height;
	rect.w = sprite_width;
	rect.h = sprite_height;
	
	primary->SetBlittingFlags( primary, (DFBSurfaceBlittingFlags)
                               ((colorkeying ? DSBLIT_SRC_COLORKEY : DSBLIT_NOFX) |
                                (opacity < 255 ? DSBLIT_BLEND_COLORALPHA : DSBLIT_NOFX)) );

	if (opacity < 255) {
		primary->SetColor( primary, 0, 0, 0, opacity );
	}

	primary->Blit( primary, surface, &rect, x, y );
}

void Resources::Surface::put_screen( int x, int y, float scale_x, float scale_y, int frame, __u8 opacity )
{
	DFBRectangle sr;
	DFBRectangle dr;
	
	sr.x = (frame % frames_per_row) * sprite_width;
	sr.y = (frame / frames_per_row) * sprite_height;
	sr.w = sprite_width;
	sr.h = sprite_height;
	
	dr.x = x;
	dr.y = y;
	dr.w = (int)(scale_x * sprite_width + 0.5f);
	dr.h = (int)(scale_y * sprite_height + 0.5f);
	
	primary->SetBlittingFlags( primary, (DFBSurfaceBlittingFlags)
                               ((colorkeying ? DSBLIT_SRC_COLORKEY : DSBLIT_NOFX) |
                                (opacity < 255 ? DSBLIT_BLEND_COLORALPHA : DSBLIT_NOFX)) );

	if (opacity < 255) {
		primary->SetColor( primary, 0, 0, 0, opacity );
	}

	primary->StretchBlit( primary, surface, &sr, &dr );
}

void Resources::Surface::get_rect( int frame, DFBRectangle *ret_rect )
{
     ret_rect->x = (frame % frames_per_row) * sprite_width;
     ret_rect->y = (frame / frames_per_row) * sprite_height;
     ret_rect->w = sprite_width;
     ret_rect->h = sprite_height;
}

IDirectFBSurface* Resources::load_surface(char *filename)
{
	
	IDirectFBImageProvider *provider;
	DFBSurfaceDescription dsc;
	IDirectFBSurface *surface;
	DFBSurfacePixelFormat pixelformat;
					
	DFBCHECK(dfb->CreateImageProvider( dfb, filename, &provider ));
	
	DFBCHECK(provider->GetSurfaceDescription( provider, &dsc ));
    
	primary->GetPixelFormat( primary, &pixelformat );

	dsc.pixelformat = pixelformat;
	
	dfb->CreateSurface( dfb, &dsc, &surface );
	
	DFBCHECK(provider->RenderTo( provider, surface, NULL ));
	provider->Release( provider );					

	return surface;
}

DFBResult Resources::sample_read_file_header (int fd)
{
     char buf[12];

     if (read (fd, buf, 12) < 12) {
          cerr <<  "Could not read at least 12 bytes!" << endl;
          return DFB_IO;
     }

     if (buf[0] != 'R' || buf[1] != 'I' || buf[2] != 'F' || buf[3] != 'F') {
          cerr << "No RIFF header found!" << endl;
          return DFB_UNSUPPORTED;
     }

     if (buf[8] != 'W' || buf[9] != 'A' || buf[10] != 'V' || buf[11] != 'E') {
          cerr << "Not a WAVE!" << endl;
          return DFB_UNSUPPORTED;
     }

     return DFB_OK;
}

int Resources::sample_read_chunk_header (int fd, char *magic)
{
     unsigned char buf[8];

     if (read (fd, buf, 8) < 8) {   
          cerr << "Could not read 8 bytes of chunk header!" << endl;
          return -1;
     }

     strncpy (magic, (char*)buf, 4);

     return buf[4] | (buf[5] << 8) | (buf[6] << 16) | (buf[7] << 24);

}

IFusionSoundBuffer* Resources::load_sample(char *filename)
{
     DFBResult            ret;
     int                  fd;
     FSBufferDescription  desc;
     IFusionSoundBuffer  *buffer;
     void                *data;
     fmtChunk             fmt;
     int                  len;

     if (!sound)
          return 0;

     cout << "(+) Loading Sample: " << filename << endl;
     fd = open (filename, O_RDONLY);
     if (fd < 0) {
          perror (filename);
          return NULL;
     }

     if (sample_read_file_header (fd)) {
          close (fd);
          return NULL;
     }

     while (DFB_TRUE) {
          char magic[4];

          len = sample_read_chunk_header (fd, magic);
          if (len <= 0) {
               cerr << "Could not find format chunk!" << endl;
               close (fd);
               return NULL;
          }

          if (magic[0] == 'f' || magic[1] == 'm' || magic[2] == 't') {
               if (len < sizeof(fmtChunk)) {
                    cerr << "Format chunk has invalid size (" << len << "/" << sizeof(fmtChunk) << ")" << endl;
                    close (fd);
                    return NULL;
               }

               if (read (fd, &fmt, len) < len) {
                    cerr << "Could not read format chunk!" << endl;
                    close (fd);
                    return NULL;
               }

               if (lseek (fd, len - sizeof(fmtChunk), SEEK_CUR) == (off_t) -1) {
                    cerr << "Could not seek past chunk!" << endl;
                    close (fd);
                    return NULL;
               }
                
               break;
          }
          else {
               if (lseek (fd, len, SEEK_CUR) == (off_t) -1) {
                    cerr << "Could not seek past chunk!" << endl;
                    close (fd);
                    return NULL;
               }
          }
     }

#ifdef WORDS_BIGENDIAN
     fmt.encoding = SWAP16(fmt.encoding);
     fmt.channels = SWAP16(fmt.channels);
     fmt.frequency = SWAP32(fmt.frequency);
     fmt.byterate = SWAP32(fmt.byterate);  
     fmt.blockalign = SWAP16(fmt.blockalign);
     fmt.bitspersample = SWAP16(fmt.bitspersample);
#endif

     if (fmt.encoding != 1) {
          cerr << "Only PCM supported, yet!" << endl;
          close (fd);
          return NULL;
     }

     if (fmt.bitspersample != 16 && fmt.bitspersample != 8) {
          cerr << "Only 16 or 8 bit supported, yet!" << endl;
          close (fd);
          return NULL;
     }
      
      
     desc.flags        = (FSBufferDescriptionFlags)(FSBDF_LENGTH | FSBDF_CHANNELS |
                                                    FSBDF_SAMPLEFORMAT | FSBDF_SAMPLERATE);
     desc.channels     = fmt.channels;
     desc.sampleformat = (fmt.bitspersample == 8) ? FSSF_U8 : FSSF_S16;
     desc.samplerate   = fmt.frequency;

     while (DFB_TRUE) {
          char magic[4];
          
          len = sample_read_chunk_header (fd, magic);
          if (len <= 0) {
               cerr << "Could not find data chunk" << endl;
               close (fd);
               return NULL;
          }
           
          if (magic[0] == 'd' && magic[1] == 'a' &&
              magic[2] == 't' && magic[3] == 'a')  
          {
               desc.length = len / fmt.blockalign;
               break;
          }
          else {
               if (lseek (fd, len, SEEK_CUR) == (off_t) -1) {
                    cerr << "Could not seek past chunk!" << endl;
                    close (fd);
                    return NULL;
               }
          }
     }
      
     ret = sound->CreateBuffer (sound, &desc, &buffer);
     if (ret) {
          DirectFBError ("IFusionSound::CreateBuffer", ret);
          close (fd);
          return NULL;
     }

     buffer->Lock (buffer, &data, NULL, NULL);

     if (read (fd, data, len) < len)
          cerr << "Warning: Could not read all data bytes!" << endl;

#ifdef WORDS_BIGENDIAN
     if (fmt.bitspersample == 16) {
         __u16 *tmpdata = (__u16*)data;
         len/=2;
         while (len--) {
              __u16 tmp = *tmpdata;
              *tmpdata++ = SWAP16(tmp);
         }
     }
#endif

     close (fd);

     buffer->Unlock (buffer);

     return buffer;
}
