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

class Actor;
class Player;
class UserInterface;
class Chat;
class ClientSkillInterpreter;
class ClientMessageHandler;
class ClientArena;

class Client
{
public:
	Client();
	~Client();

	void Update(GLib::Input* pInput, float dt);
	void Draw(GLib::Graphics* pGraphics);
	void DrawScores(GLib::Graphics* pGraphics);
	bool ConnectToServer(string ip);
	bool ListenForPackets();
	bool HandlePacket(RakNet::Packet* pPacket);
	
	void OnObjectAdded(GLib::Object3D* pObject);
	void OnObjectRemoved(GLib::Object3D* pObject);

	void RequestClientNames();
	void RequestCvarList();

	void EndRound(string winner);
	void StartRound();

	void SendServerMessage(RakNet::BitStream& bitstream);
	void SendAddTarget(int id, XMFLOAT3 pos, bool clear);
	void SetArenaState(CurrentState state);
	void SetScore(string name, int score);
	void SetLocalPlayer(Player* pPlayer);
	void SetSelectedPlayer(Player* pPlayer);
	void SetStatusText(string text, float duration);
	void AddChatText(string text, COLORREF color = RGB(0, 0, 0));

	//
	// Getters                                                             
	//
	RakNet::RakPeerInterface* GetRaknetPeer();
	GLib::World*	GetWorld();
	vector<Player*> GetPlayerList();
	Player*			GetLocalPlayer();
	Chat*			GetChat();
	ClientSkillInterpreter* GetSkillInterpreter();
	UserInterface*	GetUi();
	ClientArena*	GetArena();
	string			GetName();
	int				GetLocalPlayerId();
	CurrentState	GetArenaState();
	bool			IsLocalPlayerSelected();

	void MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	RakNet::RakPeerInterface*	mRaknetPeer;
	ClientArena*				mArena;
	ClientSkillInterpreter*		mSkillInterpreter;
	ClientMessageHandler*		mMessageHandler;
	UserInterface*				mUserInterface;
	GLib::StatusText*			mStatusText;
	ArenaState					mArenaState;
	map<string, int>			mScoreMap;
	string						mName;
	string						mWinner;
	bool						mRoundEnded;
};