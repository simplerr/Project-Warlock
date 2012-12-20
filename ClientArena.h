#pragma once
#include <vector>
#include <map>
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

class ClientArena
{
public:
	ClientArena(Client* pClient);
	~ClientArena();

	void Update(GLib::Input* pInput, float dt);
	void Draw(GLib::Graphics* pGraphics);

	void OnObjectAdded(GLib::Object3D* pObject);
	void OnObjectRemoved(GLib::Object3D* pObject);

	void RemovePlayer(int id);
	void SetSelectedPlayer(Player* pPlayer);
	void PollSelection(GLib::Input* pInput);
	void ResetPlayers();

	void SetLocalModule(PlayerModule* pModule);
	Player*	GetLocalPlayer();
	PlayerModule* GetLocalPlayerModule();
	bool IsLocalPlayerSelected();

	GLib::World* GetWorld();
	vector<Player*> GetPlayerList();
	PlayerModule* GetPlayerModule(int id);
private:
	Client*				mClient;	
	GLib::World*		mWorld;
	Player*				mSelectedPlayer;
	PlayerModule*		mPlayer;
	vector<Player*>		mPlayerList;
	BloodPSystem*		mParticleSystem;

	map<int, PlayerModule*> mModuleList;
};