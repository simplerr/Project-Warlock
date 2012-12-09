#pragma once
#include "RakPeerInterface.h"
#include "BitStream.h"
#include "d3dUtil.h"
#include "States.h"
#include <string>
#include <map>

using namespace std;

namespace GLib {
	class Input;
	class Graphics;
	class World;
	class Object3D;
	class StatusText;
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
	void DrawScores(GLib::Graphics* pGraphics);
	void PollSelection(GLib::Input* pInput);
	bool ConnectToServer(string ip);
	bool ListenForPackets();
	bool HandlePacket(RakNet::Packet* pPacket);
	
	void OnObjectAdded(GLib::Object3D* pObject);
	void OnObjectRemoved(GLib::Object3D* pObject);
	void RemovePlayer(int id);

	void RequestClientNames();
	void RequestCvarList();

	void SendServerMessage(RakNet::BitStream& bitstream);
	void SendAddTarget(int id, XMFLOAT3 pos, bool clear);

	void SetSelectedPlayer(Player* pPlayer);

	RakNet::RakPeerInterface* GetRaknetPeer();
	GLib::World*	GetWorld();
	Player*			GetPlayer();
	string			GetName();
	int				GetPlayerId();
	GameState		GetArenaState();
	bool			IsLocalPlayerSelected();

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

	void MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	RakNet::RakPeerInterface*	mRaknetPeer;
	GLib::World*				mWorld;
	ClientSkillInterpreter*		mSkillInterpreter;
	UserInterface*				mUserInterface;
	Player*						mSelectedPlayer;
	Player*						mPlayer;
	vector<Player*>				mPlayerList;
	GLib::StatusText*			mStatusText;
	map<string, int>			mScoreMap;
	string						mName;

	bool						mRoundEnded;
	string						mWinner;

	ArenaState	mArenaState;
};