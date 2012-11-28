#pragma once
#include "Actor.h"
#include "RakPeerInterface.h"

namespace GLib {
	class Input;
}

class SkillHandler;
class Client;

class Player : public Actor
{
public:
	Player();
	~Player();

	void Init();
	void Update(float dt);
	void Draw(GLib::Graphics* pGraphics);
	void PollAction(Client* pClient, GLib::Input* pInput);
	
	void SetSystemAdress(RakNet::SystemAddress adress);
	void SetHealth(float health);

	RakNet::SystemAddress GetSystemAdress();
	float GetHealth();
private:
	SkillHandler* mSkillHandler;	// Should it really be here?
	RakNet::SystemAddress mSystemAdress;
	string name;
	float mHealth;
};