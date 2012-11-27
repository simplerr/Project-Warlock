#pragma once
#include "RakPeerInterface.h"
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
	void SendAddProjectile(int owner, XMFLOAT3 startPos, XMFLOAT3 endPos);

	RakNet::RakPeerInterface* GetRaknetPeer();
	GLib::World* GetWorld();
	int GetPlayerId();
private:
	RakNet::RakPeerInterface*	mRaknetPeer;
	GLib::World*				mWorld;
	ClientSkillInterpreter*		mSkillInterpreter;
	Actor*		 mSelectedObject;
	Player*		 mPlayer;
	string		 mName;
};