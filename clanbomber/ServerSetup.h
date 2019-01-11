/***************************************************************************
                          ServerSetup.h  -  description                              
                             -------------------                                         
    begin                : Mon Jul 29 2001                                           
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

#ifndef _header_clanbomber_serversetup_
#define _header_clanbomber_serversetup_

#include "link.h"

#include "ClientSetup.h"

class ClanBomberApplication;
class Server;
class Client;
class BomberConfig;

class ServerSetup {
public: 
    ServerSetup(ClanBomberApplication* _app, Server* _server, Client* _client);
    ~ServerSetup();

    void exec();
    bool end_setup();

    static bool enter_chat_message(bool fick);
    static void show_chat_request();
    static void draw(bool fick=false);

    static void select_player(int index);  
    static void unselect_player(int index);
protected:
    ClanBomberApplication* app;
    int cur_row;
    int cur_col;
    Server* cb_server;
    Client* cb_client;
    void enter_name();
    void handle_enter();
    bool exit_setup;

    static void show_client_messages();
    static void show_player_selected(int index);
    static void show_player_unselected(int index);

    CL_List<BomberConfig> selected_players;
    CL_List<BomberConfig> unselected_players;
};

#endif
