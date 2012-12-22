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
class RoundHandler;

class Client
{
public:
	Client();
	~Client();

	void Update(GLib::Input* pInput, float dt);
	void Draw(GLib::Graphics* pGraphics);
	
	bool ConnectToServer(string ip);
	bool ListenForPackets();
	bool HandlePacket(RakNet::Packet* pPacket);
	void SendServerMessage(RakNet::BitStream& bitstream);

	void EndRound(string winner);
	void StartRound();

	void SetSelectedPlayer(Player* pPlayer);
	void AddChatText(string text, COLORREF color = RGB(0, 0, 0));

	//
	// Getters                                                             
	//
	GLib::World*	GetWorld();
	vector<Player*> GetPlayerList();
	Player*			GetLocalPlayer();
	Chat*			GetChat();
	UserInterface*	GetUi();
	RoundHandler*	GetRoundHandler();
	string			GetName();
	CurrentState	GetArenaState();
	bool			IsLocalPlayerSelected();

	void MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	RakNet::RakPeerInterface*	mRaknetPeer;
	ClientArena*				mArena;
	ClientMessageHandler*		mMessageHandler;
	UserInterface*				mUserInterface;
	RoundHandler*				mRoundHandler;
	string						mName;
};