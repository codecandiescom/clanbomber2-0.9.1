SUBDIRS = maps pics wavs

clanbomber2dir = $(DATADIR)/$(PACKAGE)

clanbomber2_DATA = decker.ttf

EXTRA_DIST = $(clanbomber2_DATA)

CLEANFILES = *~ DEADJOE

INCLUDES = $(DIRECTFB_CFLAGS) $(FUSIONSOUND_CFLAGS)
LDADD = $(DIRECTFB_LIBS) $(FUSIONSOUND_LIBS)

AM_CPPFLAGS = \
	-DDATADIR=\"@DATADIR@/@PACKAGE@/\"


bin_PROGRAMS = clanbomber2

clanbomber2_SOURCES = \
	Bomb.cpp \
	Bomb.h \
	Bomber.cpp \
	Bomber.h \
	Bomber_Corpse.cpp \
	Bomber_Corpse.h \
	Chat.cpp \
	Chat.h \
	ClanBomber.cpp \
	ClanBomber.h \
	Client.cpp \
	Client.h \
	ClientSetup.cpp \
	ClientSetup.h \
	Config.cpp \
	Config.h \
	Controller.cpp \
	Controller.h \
	Controller_AI.cpp \
	Controller_AI.h \
	Controller_AI_mass.cpp \
	Controller_AI_mass.h \
	Controller_Joystick.cpp \
	Controller_Joystick.h \
	Controller_RCMouse.cpp \
	Controller_RCMouse.h \
	Controller_Keyboard.cpp \
	Controller_Keyboard.h \
	Corpse_Part.cpp \
	Corpse_Part.h \
	Credits.cpp \
	Credits.h \
	Debug.cpp \
	Debug.h \
	Disease.cpp \
	Disease.h \
	Disease_Fast.cpp \
	Disease_Fast.h \
	Disease_Frozen.cpp \
	Disease_Frozen.h \
	Disease_PutBomb.cpp \
	Disease_PutBomb.h \
	Disease_Stoned.cpp \
	Disease_Stoned.h \
	Explosion.cpp \
	Explosion.h \
	Extra.cpp \
	Extra.h \
	Extra_Bomb.cpp \
	Extra_Bomb.h \
	Extra_Glove.cpp \
	Extra_Glove.h \
	Extra_Joint.cpp \
	Extra_Joint.h \
	Extra_Kick.cpp \
	Extra_Kick.h \
	Extra_Koks.cpp \
	Extra_Koks.h \
	Extra_Power.cpp \
	Extra_Power.h \
	Extra_Skateboard.cpp \
	Extra_Skateboard.h \
	Extra_Viagra.cpp \
	Extra_Viagra.h \
	Event.cpp \
	Event.h \
	GameObject.cpp \
	GameObject.h \
	GameStatus.cpp \
	GameStatus.h \
	GameStatus_Team.cpp \
	GameStatus_Team.h \
	Map.cpp \
	Map.h \
	MapEditor.cpp \
	MapEditor.h \
	MapEntry.cpp \
	MapEntry.h \
	MapSelector.cpp \
	MapSelector.h \
	MapTile.cpp \
	MapTile.h \
	MapTile_Arrow.cpp \
	MapTile_Arrow.h \
	MapTile_Box.cpp \
	MapTile_Box.h \
	MapTile_Ground.cpp \
	MapTile_Ground.h \
	MapTile_Ice.cpp \
	MapTile_Ice.h \
	MapTile_None.cpp \
	MapTile_None.h \
	MapTile_Trap.cpp \
	MapTile_Trap.h \
	MapTile_Wall.cpp \
	MapTile_Wall.h \
	Menu.cpp \
	Menu.h \
	Mutex.cpp \
	Mutex.h \
	Observer.cpp \
	Observer.h \
	PlayerSetup.cpp \
	PlayerSetup.h \
	Resources.cpp \
	Resources.h \
	Server.cpp \
	Server.h \
	ServerSetup.cpp \
	ServerSetup.h \
	Timer.cpp \
	Timer.h \
	Thread.cpp \
	Thread.h \
	array.h \
	cl_vector.cpp \
	cl_vector.h \
	clanstring.h \
	link.h
