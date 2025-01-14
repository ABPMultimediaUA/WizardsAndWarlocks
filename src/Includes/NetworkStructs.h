#ifndef NETWORKSTRUCTS_H
#define NETWORKSTRUCTS_H

#include <iostream>
#include <raknet/MessageIdentifiers.h>

struct ServerInfo{
    bool isServer = false;
	bool inGameServer = false;
	std::string serverName = "";
};

struct ServerData{
	int playerCount;
	bool lobbyState;
	std::string name;
	std::string ip;
};

enum GameMessages{
	ID_PLAYER_JOIN			= ID_USER_PACKET_ENUM +  1,
	ID_EXISTING_PLAYER		= ID_USER_PACKET_ENUM +  2,
	ID_PLAYER_DISCONNECT	= ID_USER_PACKET_ENUM +  3,
	ID_OBJECT_STATUS_CHAGED	= ID_USER_PACKET_ENUM +  4,
	ID_MATCH_ENDED			= ID_USER_PACKET_ENUM +  5,
	ID_CREATE_PLAYER_ONE	= ID_USER_PACKET_ENUM +  6,
	ID_ERASE_TRAP			= ID_USER_PACKET_ENUM +  7,
	ID_CHANGE_TRAP			= ID_USER_PACKET_ENUM +  8,
	ID_CHANGE_SPELL			= ID_USER_PACKET_ENUM +  9,
	ID_DOOR_INTERACTED		= ID_USER_PACKET_ENUM + 10,
	ID_POTION_INTERACTED	= ID_USER_PACKET_ENUM + 11,
	ID_DOOR_FORCE_OPEN		= ID_USER_PACKET_ENUM + 12,
	ID_REFRESH_POTION		= ID_USER_PACKET_ENUM + 13,
	ID_INIT_TRAPS			= ID_USER_PACKET_ENUM + 14,
	ID_IDENTIFY_PROPRIETARY	= ID_USER_PACKET_ENUM + 15,
	ID_SEND_SEED			= ID_USER_PACKET_ENUM + 16
};

enum VariableMapID{
	ID_CREATE			= 1,
	ID_EXISTING_OBJECT	= 2,
	ID_REMOVE			= 3,
	
	ID_CHANGE_BOOL		= 4,
	ID_CHANGE_INT		= 5,
	ID_CHANGE_FLOAT		= 6,
	ID_CHANGE_VECINT	= 7,
	ID_CHANGE_VECFLOAT	= 8,
	ID_CHANGE_STRING	= 9
};

// ####################### GAME SPECIFIC MESSAGES #######################

enum ObjectType{
	ID_NO_OBJ 			= -1,
	ID_PLAYER_O			= 1,
	ID_POTION_O			= 2,
	ID_FOUNTAIN_O 		= 3,
	ID_DOOR_O 			= 4,
	ID_SWITCH_O 		= 5,
	ID_GRAIL_O 			= 6,
	ID_TRAP_O			= 7,
	ID_PROYECTIL_O		= 8,
	ID_MULTIGAME_O		= 9
};

enum ObjectVariable{
	ID_NO_VAR = -1,

	// ######################### PLAYER #########################
	PLAYER_MOVE_UP				=	1,
	PLAYER_MOVE_DOWN			=	2,
	PLAYER_MOVE_LEFT			=	3,
	PLAYER_MOVE_RIGHT			=	4,
	PLAYER_RAYCAST				=	5,
	PLAYER_JUMP					=	6,
	PLAYER_USE_OBJECT			=	7,
	PLAYER_DROP_OBJECT			=	8,
	PLAYER_SHOOT				=	9,
	PLAYER_POSITION				=	10,
	PLAYER_ROTATION				=	11,
	PLAYER_DEPLOY_TRAP 			=	12,
	PLAYER_ALLIANCE				=	13,
	PLAYER_SET_ALL_INPUT		=	14,
	PLAYER_MANA					=	15,
	PLAYER_LIFE					=	16,
	PLAYER_RESPAWN				=	17,
	PLAYER_CREATE_CHAR			=	18,
	PLAYER_READY				=	19,
	PLAYER_SPELL				=	20,
	PLAYER_NAME					=	21,
	PLAYER_RUN					=	22,
	PLAYER_STAMINA				=	23,
	PLAYER_DANCE				=	24,
	// ########################## MULTIGAME #####################
	MULTIGAME_CHANGE			=	25,
	MULTIGAME_WINNER_ALLIANCE	=	26
	
};


#endif