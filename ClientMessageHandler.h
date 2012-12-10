#pragma once
#include "BitStream.h"

class Client;

class ClientMessageHandler
{
public:
	ClientMessageHandler(Client* pClient);
	~ClientMessageHandler();

	//
	// Handle packet functions.
	//
	void HandleWorldUpdate(RakNet::BitStream& bitstream);
	void HandleTargetAdded(RakNet::BitStream& bitstream);
	void HandleObjectRemvoed(RakNet::BitStream& bitstream);
	void HandleConnectionSuccess(RakNet::BitStream& bitstream);
	void HandleAddPlayer(RakNet::BitStream& bitstream);
	void HandlePlayerDisconnected(RakNet::BitStream& bitstream);
	void HandleGetConnectedPlayers(RakNet::BitStream& bitstream);
	void HandleSkillCasted(RakNet::BitStream& bitstream);
	void HandleProjectilePlayerCollision(RakNet::BitStream& bitstream);
	void HandleRoundStarted(RakNet::BitStream& bitstream);
	void HandleRoundEnded(RakNet::BitStream& bitstream);
	void HandleCvarList(RakNet::BitStream& bitstream);
	void HandleCvarChange(RakNet::BitStream& bitstream);
	void HandlePlayerEliminated(RakNet::BitStream& bitstream);
private:
	Client* mClient;
};