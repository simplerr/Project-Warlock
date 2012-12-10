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
class Chat;
class ClientMessageHandler;

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

	void EndRound(string winner);
	void StartRound();

	void SendServerMessage(RakNet::BitStream& bitstream);
	void SendAddTarget(int id, XMFLOAT3 pos, bool clear);
	void SetArenaState(GameState state);
	void SetSelectedPlayer(Player* pPlayer);
	void SetScore(string name, int score);
	void SetLocalPlayer(Player* pPlayer);
	void SetStatusText(string text, float duration);
	void AddChatText(string text, COLORREF color = RGB(0, 0, 0));

	RakNet::RakPeerInterface* GetRaknetPeer();
	GLib::World*	GetWorld();
	Player*			GetPlayer();
	Chat*			GetChat();
	ClientSkillInterpreter* GetSkillInterpreter();
	string			GetName();
	int				GetPlayerId();
	GameState		GetArenaState();
	bool			IsLocalPlayerSelected();

	void MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	RakNet::RakPeerInterface*	mRaknetPeer;
	GLib::World*				mWorld;
	ClientSkillInterpreter*		mSkillInterpreter;
	ClientMessageHandler*		mMessageHandler;
	UserInterface*				mUserInterface;
	Player*						mSelectedPlayer;
	Player*						mPlayer;
	vector<Player*>				mPlayerList;
	GLib::StatusText*			mStatusText;
	map<string, int>			mScoreMap;
	string						mName;

	bool						mRoundEnded;
	string						mWinner;

	ArenaState					mArenaState;
};