/***************************************************************************
                          Chat.h  -  description
                             -------------------
    begin		 : Mon Nov 17 2003
    email		 : clanbomber@fischlustig.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _header_clanbomber_chat_
#define _header_clanbomber_chat_

#include "Client.h"

#define CHAT_MESSAGES_COUNT  17
#define CHAT_MESSAGE_WIDTH  222

#define MSG_Y_OFF 35
#define MSG_X_OFF 74

typedef char msgparts[256][1024];

class ClanBomberApplication;
class Mutex;
class Client;

class Chat
{
    public:
        Chat(ClanBomberApplication* app);
        ~Chat();
        static void set_client(Client* cl);
        static void reset();
        static bool enabled();
        static void draw();
        static void show();
        static void hide();
        static void add(char* message);
        static void enter();
        static void leave();
    public:
        ClanBomberApplication* cb_app;
        Client* cb_client;
        list<char*> messages;
        int num_messages;
        Mutex* exclusion;
        bool is_enabled;
};

#endif
