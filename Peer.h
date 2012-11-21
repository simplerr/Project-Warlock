#pragma once
#include "RakPeerInterface.h"
#include "d3dUtil.h"

namespace GLib {
	class Input;
	class Graphics;
}

class Arena;

enum MessageId
{
	WORLD_UPDATE = 150,
	TARGET_ADDED,
	TARGET_ADDED_RECEIVED
};

class Peer
{
public:
	Peer();
	~Peer();

	void Update(GLib::Input* pInput, float dt);
	void Draw(GLib::Graphics* pGraphics);
	bool ConnectToServer(string ip);
	bool ListenForPackets();
	bool HandlePacket(RakNet::Packet* pPacket);
	bool IsHost();
	RakNet::RakPeerInterface* GetRaknetPeer();

	void SendAddTarget(int id, XMFLOAT3 pos, bool clear);
private:
	RakNet::RakPeerInterface* mRaknetPeer;
	Arena*	mArena;
	bool	mRunningServer;
	string	mName;
};