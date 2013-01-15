#pragma once
#include <vector>
#include <map>
#include "BaseArena.h"
using namespace std;

namespace GLib {
	class World;
	class Player;
	class Object3D;
	class Input;
	class Graphics;
}

class Client;
class Player;
class BloodPSystem;
class PlayerModule;

class ClientArena : public BaseArena
{
public:
	ClientArena(Client* pClient);
	~ClientArena();

	void Update(GLib::Input* pInput, float dt);
	void Draw(GLib::Graphics* pGraphics);
	void PollSelection(GLib::Input* pInput);

	void OnObjectAdded(GLib::Object3D* pObject);
	void OnObjectRemoved(GLib::Object3D* pObject);

	void RemovePlayer(int id);
	void ResetPlayers();

	void SetSelectedPlayer(Player* pPlayer);
	void SetLocalModule(PlayerModule* pModule);

	GLib::World*	GetWorld();
	PlayerModule*	GetLocalPlayerModule();
	Player*			GetLocalPlayer();
	vector<Player*> GetPlayerList();
	PlayerModule*	GetPlayerModule(int id);
	bool			IsLocalPlayerSelected();
private:
	Client*				mClient;	
	Player*				mSelectedPlayer;
	PlayerModule*		mPlayer;
	map<int, PlayerModule*> mModuleList;
};