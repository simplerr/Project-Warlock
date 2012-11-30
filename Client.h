#pragma once
#include "RakPeerInterface.h"
#include "BitStream.h"
#include "d3dUtil.h"
#include <string>

using namespace std;

namespace GLib {
	class Input;
	class Graphics;
	class World;
}

class ClientSkillInterpreter;
class Actor;
class Player;
class UserInterface;

class Client
{
public:
	Client();
	~Client();

	void Update(GLib::Input* pInput, float dt);
	void Draw(GLib::Graphics* pGraphics);
	void PollSelection(GLib::Input* pInput);
	bool ConnectToServer(string ip);
	bool ListenForPackets();
	bool HandlePacket(RakNet::Packet* pPacket);
	
	void RequestClientNames();
	void SendServerMessage(RakNet::BitStream& bitstream);
	void SendAddTarget(int id, XMFLOAT3 pos, bool clear);

	void SetSelectedPlayer(Player* pPlayer);

	RakNet::RakPeerInterface* GetRaknetPeer();
	GLib::World* GetWorld();
	Player* GetPlayer();
	int GetPlayerId();
	bool IsLocalPlayerSelected();

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

private:
	RakNet::RakPeerInterface*	mRaknetPeer;
	GLib::World*				mWorld;
	ClientSkillInterpreter*		mSkillInterpreter;
	UserInterface*				mUserInterface;
	Player*		 mSelectedPlayer;
	Player*		 mPlayer;
	string		 mName;
};