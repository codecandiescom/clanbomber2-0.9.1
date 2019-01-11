/***************************************************************************
                          Config.cpp  -  description
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
// $Id: Config.cpp,v 1.12 2007/10/15 13:58:16 der_fisch Exp $

#include <iostream>
#include <fstream>

#include "ClanBomber.h"
#include "Config.h"

#include "Controller.h"

#include <stdio.h>

CL_String Config::filename	= "clanbomber.cfg";
CL_String Config::path		= "";

CL_String Config::last_server= "intruder";

int	Config::round_time			= 90;
int Config::sound_enabled		= true;
int	Config::max_skateboards		= 5;
int	Config::max_power			= 12;
int	Config::max_bombs			= 9;

int	Config::start_skateboards	= 0;
int	Config::start_power			= 1;
int	Config::start_bombs			= 1;
int	Config::start_kick			= false;
int	Config::start_glove			= false;

int	Config::skateboards			= true;
int	Config::power				= true;
int	Config::bombs				= true;
int	Config::kick				= true;
int	Config::glove				= true;

int	Config::joint				= true;
int	Config::viagra				= true;
int	Config::koks				= true;

int	Config::start_map			= 0;
int Config::random_map_order	= false;

int	Config::music				= false;

int	Config::kids_mode			= false;
int	Config::shaky_explosions	= true;
int	Config::highlight_maptiles	= false;
int	Config::random_positions	= true;
int	Config::corpse_parts		= 10;

int	Config::bomb_countdown		= 25;
int	Config::bomb_delay			= 10;
int	Config::bomb_speed			= 160;

int	Config::points_to_win		= 5;
int	Config::theme				= 0;

BomberConfig Config::bomber[8];

BomberConfig::BomberConfig()
{
	local_client = true;
	server_bomber = false;
	client_index = -1;
	config_index = -1;
	client_ip = NULL;
	enabled = true;
	team = 0;
	skin = 0;
	controller = 0;
	highlight_maptile = true;
	name = "Fischlustig";
}

BomberConfig::~BomberConfig()
{
}

void BomberConfig::set_name(CL_String _name)
{
	if (_name.get_length()) {
		name = _name;
	}
}

CL_String BomberConfig::get_name()
{
	return name;
}

void BomberConfig::set_skin(int _skin)
{
	skin = _skin%NR_BOMBERSKINS;
}

int BomberConfig::get_skin()
{
	return skin;
}

void BomberConfig::set_team(int _team)
{
	team = _team%5;
}

int BomberConfig::get_team()
{
	return team;
}

void BomberConfig::set_controller(int _controller)
{
//FIXME	controller = _controller% (5 + CL_Input::joysticks.size());
	controller = _controller% (7);
}

int BomberConfig::get_controller()
{
	return controller;
}

void BomberConfig::enable()
{
	enabled = true;
}

void BomberConfig::disable()
{
	enabled = false;
}

void BomberConfig::set_highlight_maptile(bool _highlight_maptile)
{
	highlight_maptile = _highlight_maptile;
}

int BomberConfig::get_highlight_maptile()
{
	return highlight_maptile;
}

int BomberConfig::is_enabled()
{
	return enabled;
}

void BomberConfig::set_enabled(bool _enabled)
{
	enabled = _enabled;
}

void BomberConfig::set_client_index(int index)
{
	client_index = index;
}

int BomberConfig::get_client_index()
{
	return client_index;
}

int BomberConfig::get_config_index()
{
	return config_index;
}

void BomberConfig::set_config_index(int index)
{
	config_index=index;
}

char* BomberConfig::get_client_ip()
{
    return client_ip;
}

void BomberConfig::set_client_ip(char* ip_string)
{
    client_ip = ip_string;
}

int Config::get_number_of_players()
{
	int nr = 0;
	
	for (int i=0; i<8; i++) {
		if (bomber[i].is_enabled()) {
			nr++;
		}
	}
	
	return nr;
}

int Config::get_number_of_opponents()
{
	int nrs = 0;
	int team_cunt[4] = {0,0,0,0};
	
	for (int i=0; i<8; i++) {
		if (bomber[i].is_enabled()) {
			if (bomber[i].get_team() == 0) {
				nrs++;
			}
			else {
				if (team_cunt[bomber[i].get_team()-1] == 0) {
					nrs++;
				}
				team_cunt[bomber[i].get_team()-1]++;
			}
		}
	}
	
	return nrs;
}

void Config::set_round_time( int _round_time)
{
	round_time = _round_time;
}

void Config::set_random_map_order( int _random_map_order)
{
	random_map_order = _random_map_order;
}

void Config::set_max_skateboards(int _max_skateboards)
{
	max_skateboards = _max_skateboards;
}

void Config::set_max_power(int _max_power)
{
	max_power = _max_power;
}

void Config::set_max_bombs(int _max_bombs)
{
	max_bombs = _max_bombs;
}

void Config::set_start_skateboards(int _start_skateboards)
{
	start_skateboards = _start_skateboards;
}

void Config::set_start_power(int _start_power)
{
	start_power = _start_power;
}

void Config::set_start_bombs(int _start_bombs)
{
	start_bombs = _start_bombs;
}

void Config::set_start_kick( int _start_kick)
{
	start_kick = _start_kick;
}

void Config::set_start_glove( int _start_glove)
{
	start_glove = _start_glove;
}

void Config::set_skateboards(int _skateboards)
{
	skateboards = _skateboards;
}

void Config::set_power(int _power)
{
	power = _power;
}

void Config::set_bombs(int _bombs)
{
	bombs = _bombs;
}

void Config::set_kick( int _kick)
{
	kick = _kick;
}

void Config::set_glove( int _glove)
{
	glove = _glove;
}

void Config::set_joint( int _joint)
{
	joint = _joint;
}

void Config::set_viagra( int _viagra)
{
	viagra = _viagra;
}

void Config::set_koks( int _koks)
{
	koks = _koks;
}

void Config::set_start_map(int _start_map)
{
	start_map = _start_map;
}

void Config::set_points_to_win(int _points_to_win)
{
	points_to_win = _points_to_win;
}

void Config::set_theme(int _theme)
{
	theme = _theme;
}

void Config::set_filename(CL_String _filename)
{
	filename = _filename;
}

void Config::set_path(CL_String _path)
{
	path = _path;
}

void Config::set_music( int _music)
{
	music = _music;
}

void Config::set_kids_mode( int _kids_mode)
{
	kids_mode = _kids_mode;
}

void Config::set_corpse_parts( int _corpse_parts)
{
	corpse_parts = _corpse_parts;
}

void Config::set_shaky_explosions(int _shaky_explosions)
{
	shaky_explosions = _shaky_explosions;
}

void Config::set_highlight_maptiles(int _highlight_maptiles)
{
	highlight_maptiles =  _highlight_maptiles;
}

void Config::set_random_positions(int _random_positions)
{
	random_positions = _random_positions;
}


void Config::set_bomb_countdown(int _bomb_countdown)
{
	bomb_countdown = _bomb_countdown;
}

void Config::set_bomb_delay(int _bomb_delay)
{
	bomb_delay = _bomb_delay;
}

void Config::set_bomb_speed(int _bomb_speed)
{
	bomb_speed = _bomb_speed;
}

int Config::get_max_skateboards()
{
	return max_skateboards;
}

int Config::get_max_power()
{
	return max_power;
}

int Config::get_max_bombs()
{
	return max_bombs;
}

int Config::get_start_skateboards()
{
	return start_skateboards;
}

int Config::get_start_power()
{
	return start_power;
}

int Config::get_start_bombs()
{
	return start_bombs;
}

int Config::get_start_kick()
{
	return start_kick;
}

int Config::get_start_glove()
{
	return start_glove;
}

int Config::get_skateboards()
{
	return skateboards;
}

int Config::get_power()
{
	return power;
}

int Config::get_bombs()
{
	return bombs;
}

int Config::get_kick()
{
	return kick;
}

int Config::get_glove()
{
	return glove;
}

int Config::get_joint()
{
	return joint;
}

int Config::get_viagra()
{
	return viagra;
}

int Config::get_koks()
{
	return koks;
}

int Config::get_start_map()
{
	return start_map;
}

int Config::get_random_map_order()
{
	return random_map_order;
}

int Config::get_round_time()
{
	return round_time;
}

int Config::get_points_to_win()
{
	return points_to_win;
}

int Config::get_theme()
{
	return theme;
}

int Config::get_sound_enabled()
{
	return sound_enabled;
}

int Config::get_music()
{
	return music;
}

int Config::get_kids_mode()
{
	return kids_mode;
}

int Config::get_corpse_parts()
{
	return corpse_parts;
}

int Config::get_shaky_explosions()
{
	return shaky_explosions;
}

int Config::get_highlight_maptiles()
{
	return highlight_maptiles;
}


int Config::get_random_positions()
{
	return random_positions;
}

int Config::get_bomb_countdown()
{
	return bomb_countdown;
}

int Config::get_bomb_delay()
{
	return bomb_delay;
}

int Config::get_bomb_speed()
{
	return bomb_speed;
}

bool Config::save(bool init)
{
	if (init) {
		for (int i=0; i<8; i++) {
			bomber[i].set_skin(i);
		}
		bomber[0].set_name( "Are" );
		bomber[1].set_name( "You" );
		bomber[2].set_name( "Still" );
		bomber[3].set_name( "Watching" );
		bomber[4].set_name( "AIs" );
		bomber[5].set_name( "Playing" );
		bomber[6].set_name( "For" );
		bomber[7].set_name( "You" );
	}

	ofstream configfile(path+filename);

	configfile << CURRENT_CONFIGFILE_VERSION << std::endl; // version

	configfile << max_bombs			<< std::endl;
	configfile << max_power			<< std::endl;
	configfile << max_skateboards		<< std::endl;
	
	configfile << start_bombs		<< std::endl;
	configfile << start_power		<< std::endl;
	configfile << start_skateboards		<< std::endl;
	configfile << start_kick		<< std::endl;
	configfile << start_glove		<< std::endl;
	
	configfile << start_map			<< std::endl;
	configfile << points_to_win		<< std::endl;
	configfile << round_time		<< std::endl;
	configfile << theme			<< std::endl;
	configfile << music			<< std::endl;

	configfile << kids_mode			<< std::endl;
	configfile << corpse_parts		<< std::endl;
	configfile << shaky_explosions		<< std::endl;
	configfile << random_positions		<< std::endl;
	configfile << random_map_order		<< std::endl;
	
	configfile << bombs			<< std::endl;
	configfile << power			<< std::endl;
	configfile << skateboards		<< std::endl;
	configfile << kick			<< std::endl;
	configfile << glove			<< std::endl;
	
	configfile << joint			<< std::endl;
	configfile << viagra			<< std::endl;
	configfile << koks			<< std::endl;

	configfile << bomb_countdown		<< std::endl;
	configfile << bomb_delay		<< std::endl;
	configfile << bomb_speed		<< std::endl;
	
	for (int i=0; i<8; i++) {
		configfile << bomber[i].get_skin()		<< std::endl;
		configfile << bomber[i].get_name().get_string()	<< std::endl;
		configfile << bomber[i].get_team()		<< std::endl;
		configfile << bomber[i].get_controller()	<< std::endl;
		configfile << bomber[i].is_enabled()		<< std::endl;
		configfile << bomber[i].get_highlight_maptile()	<< std::endl;
	}

	configfile << last_server << std::endl;

	return true;
}

bool Config::load()
{
	FILE* configfile = fopen( path+filename, "r" );

	if (!configfile) {
		save(true);
		return false;
	}

	int version;
	fscanf( configfile, "%d\n", &version );

	if (version != CURRENT_CONFIGFILE_VERSION) {
		fclose( configfile );
		save(true);
		return false;
	}

	fscanf( configfile, "%d\n", &max_bombs );
	fscanf( configfile, "%d\n", &max_power );
	fscanf( configfile, "%d\n", &max_skateboards );
	
	fscanf( configfile, "%d\n", &start_bombs );
	fscanf( configfile, "%d\n", &start_power );
	fscanf( configfile, "%d\n", &start_skateboards );
	fscanf( configfile, "%d\n", &start_kick );
	fscanf( configfile, "%d\n", &start_glove );
	fscanf( configfile, "%d\n", &start_map );
	
	fscanf( configfile, "%d\n", &points_to_win );
	fscanf( configfile, "%d\n", &round_time );
	fscanf( configfile, "%d\n", &theme );
	fscanf( configfile, "%d\n", &music );

	fscanf( configfile, "%d\n", &kids_mode );
	fscanf( configfile, "%d\n", &corpse_parts );
	fscanf( configfile, "%d\n", &shaky_explosions );
	fscanf( configfile, "%d\n", &random_positions );
	fscanf( configfile, "%d\n", &random_map_order );

	fscanf( configfile, "%d\n", &bombs );
	fscanf( configfile, "%d\n", &power );
	fscanf( configfile, "%d\n", &skateboards );
	fscanf( configfile, "%d\n", &kick );
	fscanf( configfile, "%d\n", &glove );
	
	fscanf( configfile, "%d\n", &joint );
	fscanf( configfile, "%d\n", &viagra );
	fscanf( configfile, "%d\n", &koks );

	fscanf( configfile, "%d\n", &bomb_countdown );
	fscanf( configfile, "%d\n", &bomb_delay );
	fscanf( configfile, "%d\n", &bomb_speed );

	
	for (int i=0; i<8; i++) {
		fscanf( configfile, "%d\n", &version );
		bomber[i].set_skin(version);
		
		char c[2] = {0,0};
		CL_String player_name = "";
		while ((c[0] = fgetc(configfile)) != 10) {
			player_name += c;
		}
		bomber[i].set_name(player_name);
		
		fscanf( configfile, "%d\n", &version );
		bomber[i].set_team(version);
		
		fscanf( configfile, "%d\n", &version );
		bomber[i].set_controller(version);
	
		fscanf( configfile, "%d\n", &version );
		bomber[i].set_enabled(version!=0);

		fscanf( configfile, "%d\n", &version );
		bomber[i].set_highlight_maptile(version!=0);
	}
	
	char c[2] = {0,0};
	CL_String server_name = "";
	while ((c[0] = fgetc(configfile)) != 10) {  
		server_name += c;
	}
    set_last_server(server_name);

	fclose( configfile );

	return true;
}

void Config::set_last_server(CL_String server_name)
{
    last_server = server_name;
}
 
CL_String Config::get_last_server()
{
    return last_server;
}


void BomberConfig::set_local(bool _local)
{
	local_client=_local;
}


bool BomberConfig::is_local()
{
	return local_client;
}

bool BomberConfig::is_server_bomber()
{
	return server_bomber;
}

void BomberConfig::set_server_bomber(bool from_server)
{
	server_bomber = from_server;
}
