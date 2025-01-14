#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <cstdlib>
#include <map>

#include <NetworkStructs.h>
#include <TrapCodes.h>
#include <SpellCodes.h>
#include "RakNetIncludes.h"


class NetworkObject;

template<typename T>
class vector3d;
typedef vector3d<float> vector3df;
typedef vector3d<int> vector3di;

class Client{
public:

	// We pass the IP we're gonna connect to 
	// And the port we're talking to
	Client(std::string serverIp = "127.0.0.1", int serverPort = 6000, bool proprietary = false);
	~Client();

	// Sends game-level packages
	void SendPackage(RakNet::BitStream*, PacketPriority, PacketReliability);

	// Reads game-level packages
	void RecievePackages();

	// We manage the Object-Level packages
	void ModifyObject(RakNet::BitStream* bitstream);

	// We send a shoutdown to the server notifying we're disconnecting
	void SendShutdown();

	// We manage the network objects from server messages
	void CreateNetworkObject(int id, ObjectType type);
	void RemoveNetworkObject(int id);

	// We manage the players connected
	void AddPlayer(int id, RakNet::RakNetGUID guid);
	void RemovePlayer(int id);

	// Manage traps
	void EraseTrap(int idTrap, int playerAffectedId);

	std::map<int, NetworkObject*> GetNetworkObjects();
	std::map<int, NetworkObject*> GetNewNetworkObjects();
	std::map<int, NetworkObject*> GetToEraseNetworkObjects();

	// Send object state change
	void SetObjectBool(		int objectId, ObjectVariable k, bool v);
	void SetObjectInt(		int objectId, ObjectVariable k, int v);
	void SetObjectFloat(	int objectId, ObjectVariable k, float v);
	void SetObjectIntVec(	int objectId, ObjectVariable k, vector3di v);
	void SetObjectFloatVec(	int objectId, ObjectVariable k, vector3df v);
	void SetObjectString(	int objectId, ObjectVariable k, std::string v);

	// We get the ID of the Network Object of the Player One
	int GetPlayerOneId();
	
	// Name of the Client/Player
	void SetClientName(std::string name);
	std::string GetClientName();

	// Sync player spells and traps
	void SetPlayerTrap(int networkId, TrapEnum trap);
	void SetPlayerSpell(int networkId, int spellPosition, SPELLCODE spell);

private:
	
	int playerOneId;
	std::string name;
	bool proprietary;

	std::map<int, NetworkObject*> networkObjects;			// Vector of Network Objects on the Client
	std::map<int, NetworkObject*> newNetworkObjects;		// Vector of Network Objects on the Client that haven't been retrieved
	std::map<int, NetworkObject*> toEraseNetworkObjects;	// Map of NO Objects to be erased

	std::map<int, RakNet::RakNetGUID> networkPlayers;		// Vector of Players conected to the Server
	
	RakNet::RakPeerInterface *peer;						// Peer Interface
	RakNet::Packet *packet;								// Packet to send and recieve
	RakNet::SocketDescriptor descriptor;				// Socket Descriptor

	void SetTrap(int trapId, RakNet::BitStream* bitstream);
	void SendProprietary();

};

#endif